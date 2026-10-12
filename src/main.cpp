#include <cstdlib>

#include <fmt/format.h>

#if ST_MODULE_AUDIO
#include "audio/client.h"
#include "audio/source.h"
#endif
#if ST_MODULE_VIDEO
#include "gui/core.h"
#endif
#include "io/filesystem.h"
#include "io/log.h"
#include "lua/runtime.h"
#include "os/popup.h"
#include "st_init.h"
#if ST_MODULE_VIDEO
#include "video/window.h"
#endif

#include "argparse.h"
#include "modloader.h"

template <typename Mods>
static void
start_mods(Mods &mods)
{
    for (auto it = mods.begin(); it != mods.end();)
    {
        auto &mod = *it;

        if (!mod.second->execute_init())
        {
            st::io::log::error(fmt::format("Failed to run init phase for mod '{}'.", mod.first));
            it = mods.erase(it);
        }
        else if (!mod.second->execute_runtime())
        {
            st::io::log::error(fmt::format("Failed to run runtime phase for mod '{}'.", mod.first));
            it = mods.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for (auto &mod : mods)
    {
        mod.second->execute_function("on_start");
    }
}

#if ST_MODULE_VIDEO
template <typename Mods>
static void
run_video_loop(st::video::window &win,
               st::gui::core     &gui,
               Mods              &mods)
{
    win.show();

    win.reset_accumulator();
    while (!win.close_requested())
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
        for (auto &mod : mods)
        {
            mod.second->execute_function("on_gui_render");
        }
        gui.render();
        gui.swap();

        win.swap();
    }
}
#endif // ST_MODULE_VIDEO

static int
run(st_int32  argc,
    st_char **argv)
{
    st::argparse args(argc, argv);

    if (args.should_exit())
        return EXIT_SUCCESS;

    if (!st::lib_init())
        return EXIT_FAILURE;

    st::io::log::info(fmt::format("{} {}", PROGRAM_TITLE, VERSION_STRING));
    st::io::log::info(fmt::format("Copyright (c) {} {}.", COPYRIGHT_OWNER, COPYRIGHT_STRING));

    {
#if ST_MODULE_VIDEO
        // The window owns the GL context, so it must be created before mods
        // load textures and destroyed after the mods that own them.
        st::video::window win;
        win.set_title(PROGRAM_TITLE " " VERSION_STRING);
        st::gui::core gui(win);
        win.attach_gui(gui); // TODO: This should be irrelevant once the event bus is set up.
#endif

        st::modloader ml;
        auto          mods = args.safe_mode() ? decltype(ml.load_mods()){} : ml.load_mods();

        if (args.safe_mode())
            st::os::popup::warn("Safe mode", "Safe mode is enabled. No mods have been loaded.");

        start_mods(mods);

#if ST_MODULE_VIDEO
        run_video_loop(win, gui, mods);
#endif

        for (auto &mod : mods)
        {
            mod.second->execute_function("on_quit");
        }
    }

    st::io::log::info("Goodbye.");

    if (!st::lib_free())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int
main(st_int32  argc,
     st_char **argv)
{
    try
    {
        return run(argc, argv);
    }
    catch (...)
    {
        st::io::log::error("Unhandled exception. Exiting.");
    }
    return EXIT_FAILURE;
}
