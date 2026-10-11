#include <cstdlib>

#include <fmt/format.h>

#include "io/filesystem.h"
#include "io/log.h"
#include "lua/runtime.h"
#include "st_init.h"
#include "video/window.h"

#include "argparse.h"
#include "modloader.h"

int
main(st_int32 argc,
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
        if (!st::io::filesystem::exists("mod/core/main.lua"))
        {
            st::os::popup::error(fmt::format(
                "Could not locate core mod main.lua. The program installation may be corrupt. Please reinstall {} and try again!",
                PROGRAM_TITLE
            ));
            return EXIT_FAILURE;
        }

        st::modloader ml;
        auto mods = ml.load_mods();

        for (auto it = mods.begin(); it != mods.end();)
        {
            auto &mod = *it;
            auto &lua_rt = mod.second->runtime();

            if (!lua_rt.execute_file("main.lua"))
            {
                st::io::log::error(fmt::format("Failed to load main.lua for mod '{}'.", mod.first));
                it = mods.erase(it);
            }
            else
            {
                ++it;
            }
        }

        st::video::window win;
        win.show();

        for (auto &mod : mods)
        {
            auto &lua_rt = mod.second->runtime();
            lua_rt.execute_function("on_start");
        }

        win.reset_accumulator();
        while (!win.close_requested())
        {
            win.accumulate();
            win.poll();

            win.simulate([&](st::float64 dt)
            {
                for (auto &mod : mods)
                {
                    auto &lua_rt = mod.second->runtime();
                    lua_rt.execute_function("on_fixed_step", { dt });
                }
            });

            for (auto &mod : mods)
            {
                auto &lua_rt = mod.second->runtime();
                lua_rt.execute_function("on_step", { win.delta_time() } );
            }

            win.clear();
            win.swap();
        }

        for (auto &mod : mods)
        {
            auto &lua_rt = mod.second->runtime();
            lua_rt.execute_function("on_quit");
        }
    }

    st::io::log::info("Goodbye.");

    if (!st::lib_free())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
