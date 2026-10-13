/*
 * main.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <cstdlib>
#include <filesystem>
#include <functional>
#include <iterator>
#include <string_view>
#include <vector>

#include <fmt/format.h>

#if ST_MODULE_VIDEO
#include "gui/core.h"
#endif
#include "io/filesystem.h"
#include "io/log.h"
#include "lua/app_bindings.h"
#include "os/popup.h"
#include "st_init.h"
#include "util/config.h"
#if ST_MODULE_VIDEO
#include "video/window.h"
#endif

#include "argparse.h"
#if ST_MODULE_VIDEO
#include "loading_screen.h"
#include "main_menu.h"
#endif
#include "modloader.h"

using boot_progress = std::function<void(std::string_view, st::float32)>;

// Settings live next to the executable rather than in the user profile.
static std::filesystem::path
config_path()
{
    return st::io::filesystem::executable_dir() / "config.json";
}

// Runs a phase for every mod, dropping the mods that fail it.
template <typename Phase>
static void
run_phase(st::mod_map &mods,
          const char  *phase_name,
          Phase        run)
{
    for (auto it = mods.begin(); it != mods.end();)
    {
        if (run(*it->second))
        {
            ++it;
            continue;
        }
        st::io::log::error(fmt::format("Failed to run {} phase for mod '{}'.", phase_name, it->first));
        it = mods.erase(it);
    }
}

// Every mod finishes its init phase and declares its settings before any runtime phase starts, so a mod can read the
// settings of every other mod from the first line of its runtime code.
static void
start_mods(st::mod_map         &mods,
           st::util::config    &config,
           const boot_progress &progress)
{
    const st::float32 mod_count = static_cast<st::float32>(mods.size());
    st::float32       mods_done = 0.0f;

    run_phase(mods, "init",
              [&](st::mod &mod)
              {
                  const auto on_init_progress = [&](std::string_view status, st::float32 fraction)
                  { progress(status, (mods_done + fraction) / mod_count); };
                  const st::bool_t ok = mod.execute_init(on_init_progress);
                  mods_done += 1.0f;
                  return ok;
              });

    for (auto &mod : mods)
    {
        mod.second->bind_config(config);
    }

    run_phase(mods, "runtime", [](st::mod &mod) { return mod.execute_runtime(); });
}

static st::bool_t
save_config(const st::util::config &config,
            st::bool_t              loaded)
{
    if (!loaded)
    {
        st::io::log::error(fmt::format("Settings were not saved to '{}' because the file could not be read. "
                                       "Fix or remove the file to restore saving.",
                                       config_path().u8string()));
        return false;
    }
    return config.save(config_path());
}

#if ST_MODULE_VIDEO
[[nodiscard]] static std::vector<st::config_setting>
collect_settings(const st::mod_map &mods)
{
    std::vector<st::config_setting> settings;
    for (const auto &mod : mods)
    {
        auto declared = mod.second->config_settings();
        settings.insert(settings.end(), std::make_move_iterator(declared.begin()),
                        std::make_move_iterator(declared.end()));
    }
    return settings;
}

static void
run_video_loop(st::video::window &win,
               st::gui::core     &gui,
               st::mod_map       &mods)
{
    win.reset_accumulator();
    while (!win.close_requested() && !st::lua::app::quit_requested())
    {
        win.accumulate();
        win.poll();

        win.simulate(
            [&](st::float64 dt)
            {
                for (auto &mod : mods)
                {
                    mod.second->execute_function("on_fixed_step", {dt});
                }
            });

        for (auto &mod : mods)
        {
            mod.second->execute_function("on_step", {win.delta_time()});
        }

        win.clear();

        gui.clear();
        gui.render();
        gui.swap();

        win.swap();
    }
}
#endif // ST_MODULE_VIDEO

static int
run(st::int32    argc,
    st::char_t **argv)
{
    st::argparse args(argc, argv);

    if (args.should_exit())
        return args.exit_code();

    if (!st::lib_init())
        return EXIT_FAILURE;

    st::io::log::info(fmt::format("{} {}", PROGRAM_TITLE, VERSION_STRING));
    st::io::log::info(fmt::format("Copyright (c) {} {}.", COPYRIGHT_OWNER, COPYRIGHT_STRING));

    {
#if ST_MODULE_VIDEO
        // The window owns the GL context, so it must be created before mods
        // load textures and destroyed after the mods that own them.
        st::video::window win;
        if (!win.is_valid())
        {
            static_cast<void>(st::lib_free());
            return EXIT_FAILURE;
        }
        win.set_title(PROGRAM_TITLE " " VERSION_STRING);
        st::gui::core gui(win);
        win.attach_gui(gui); // TODO: This should be irrelevant once the event bus is set up.
        win.show();

        st::loading_screen  loading(win, gui);
        const boot_progress on_progress = [&](std::string_view status, st::float32 fraction)
        { loading.update(status, fraction); };
        on_progress("Discovering mods...", 0.0f);
#else
        const boot_progress on_progress = [](std::string_view, st::float32) {};
#endif

        // Declared before the mods, which hold pointers into it, so that it is destroyed after them.
        st::util::config config;
        const st::bool_t config_loaded = config.load(config_path());

        st::modloader ml;
        st::mod_map   mods = args.safe_mode() ? st::mod_map{} : ml.load_mods();

        if (args.safe_mode())
            st::os::popup::warn("Safe mode", "Safe mode is enabled. No mods have been loaded.");

        start_mods(mods, config, on_progress);
        static_cast<void>(save_config(config, config_loaded));
        on_progress("Starting...", 1.0f);

        for (auto &mod : mods)
        {
            mod.second->execute_function("on_start");
        }

#if ST_MODULE_VIDEO
        st::main_menu menu(win, config, collect_settings(mods),
                           [&config, config_loaded] { return save_config(config, config_loaded); });
        run_video_loop(win, gui, mods);
#endif

        for (auto &mod : mods)
        {
            mod.second->execute_function("on_quit");
        }

        if (config_loaded)
            static_cast<void>(save_config(config, config_loaded));
    }

    st::io::log::info("Goodbye.");

    if (!st::lib_free())
        return EXIT_FAILURE;

    return st::lua::app::exit_code();
}

int
main(st::int32    argc,
     st::char_t **argv)
{
    return run(argc, argv);
}
