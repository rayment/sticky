/*
 * core.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#ifndef STICKY_GUI_CORE_H
#define STICKY_GUI_CORE_H

#include <memory>
#include <SDL3/SDL.h>

#include "video/window.h"

namespace st::gui
{
    class core
    {
    public:
         core(const st::video::window &window) noexcept;
        ~core() noexcept;

        void clear () noexcept;
        void render() noexcept;
        void swap  () noexcept;

        // TODO: Replace SDL_Event with custom event type.
        void process_event(const SDL_Event &e) noexcept;

    private:
        struct core_impl;
        std::unique_ptr<core_impl> m_impl;
    };
}

#endif // STICKY_GUI_CORE_H
