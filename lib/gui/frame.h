/*
 * frame.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#ifndef STICKY_GUI_FRAME_H
#define STICKY_GUI_FRAME_H

#include <functional>
#include <memory>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include "gui/component.h"

namespace st::gui
{
    class frame : public root_component
    {
    public:
         frame() noexcept;
        ~frame() noexcept;

        void render() noexcept override;

        void set_auto_resize  (st::bool_t auto_resize)       noexcept;
        void set_closable     (st::bool_t closable)          noexcept;
        void set_collapsable  (st::bool_t collapsable)       noexcept;
        void set_movable      (st::bool_t movable)           noexcept;
        void set_position     (st::float32 x, st::float32 y) noexcept;
        void set_position     (glm::f32vec2 position)        noexcept;
        void set_resizable    (st::bool_t resizable)         noexcept;
        void set_size         (st::float32 w, st::float32 h) noexcept;
        void set_size         (glm::f32vec2 size)            noexcept;
        void set_show_titlebar(st::bool_t show_titlebar)     noexcept;
        void set_title        (std::string_view title)       noexcept;

        using on_close_callback = std::function<void()>;

        void register_on_close(const on_close_callback &callback) noexcept;

        void clear_on_close() noexcept;

    protected:
        // Fires the close callbacks once and then drops them.
        void handle_close() noexcept override;

    private:
        struct frame_impl;
        std::unique_ptr<frame_impl> m_impl;
    };
}

#endif // STICKY_GUI_FRAME_H
