/*
 * main_menu.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

/**
 * @file
 * @brief The main menu and its settings window.
 */

#ifndef STICKY_MAIN_MENU_H
#define STICKY_MAIN_MENU_H

#include <functional>
#include <memory>
#include <vector>

#include "st_macros.h"
#include "st_types.h"
#include "util/config.h"

#include "mod.h"

namespace st::video
{
    class window; // forward-declare
}

/** @addtogroup app
 *  @{
 */

namespace st
{

    /** @brief The first screen of the application, with Settings, Mods and Quit buttons. Shown while it exists. */
    class main_menu
    {
    public:
        /** @brief Persists the config, returning false on failure. */
        using save_callback = std::function<st::bool_t()>;

        /**
         * @brief Creates and shows the menu.
         * @param window The window, which must outlive the menu. Quit closes it.
         * @param config The settings edited by the settings window, which must outlive the menu.
         * @param settings The settings listed in the settings window.
         * @param save Called after the settings window wrote its changes into `config`.
         */
        main_menu(st::video::window          &window,
                  st::util::config           &config,
                  std::vector<config_setting> settings,
                  save_callback               save) noexcept;
        ~main_menu() noexcept;

        ST_DELETE_COPY_CTORS(main_menu);
        ST_DELETE_MOVE_CTORS(main_menu);

    private:
        struct main_menu_impl;
        std::unique_ptr<main_menu_impl> m_impl;
    };

} // namespace st

/** @} */

#endif // STICKY_MAIN_MENU_H
