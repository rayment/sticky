/*
 * frame.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

/**
 * @file
 * @brief A window inside the application window.
 */

#ifndef STICKY_GUI_FRAME_H
#define STICKY_GUI_FRAME_H

#include <functional>
#include <memory>
#include <string_view>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include "gui/component.h"

/** @addtogroup gui
 *  @{
 */

namespace st::gui
{
    /**
     * @brief A movable window that holds other components.
     *
     * A frame is closable, movable, resizable and collapsible by default, and shows a titlebar. It fades in when first
     * rendered and fades out when the user closes it. A st::gui::menubar child is drawn across its top.
     */
    class frame : public root_component
    {
    public:
        frame() noexcept;
        ~frame() noexcept;

        void render() noexcept override;

        /** @brief Sets whether the frame always fits its contents. @param auto_resize Whether to fit. */
        void set_auto_resize(st::bool_t auto_resize) noexcept;
        /** @brief Sets whether the titlebar has a close button. @param closable Whether closable. */
        void set_closable(st::bool_t closable) noexcept;
        /**
         * @brief Sets whether the user can collapse the frame to its titlebar.
         * @param collapsible Whether collapsible.
         */
        void set_collapsible(st::bool_t collapsible) noexcept;
        /**
         * @brief Sets the smallest size the user can resize to.
         * @param w Width in pixels.
         * @param h Height in pixels.
         */
        void set_min_size(st::float32 w,
                          st::float32 h) noexcept;
        /** @brief Sets the smallest size the user can resize to. @param size Size in pixels, negatives become 0. */
        void set_min_size(glm::f32vec2 size) noexcept;
        /** @brief Sets whether the user can drag the frame by its titlebar. @param movable Whether movable. */
        void set_movable(st::bool_t movable) noexcept;
        /** @brief Moves the top-left corner on the next frame. @param x Left in pixels. @param y Top in pixels. */
        void set_position(st::float32 x,
                          st::float32 y) noexcept;
        /** @brief Moves the top-left corner on the next frame. @param position Position in pixels. */
        void set_position(glm::f32vec2 position) noexcept;
        /** @brief Sets whether the user can resize the frame. @param resizable Whether resizable. */
        void set_resizable(st::bool_t resizable) noexcept;
        /** @brief Resizes the frame on the next frame. @param w Width in pixels. @param h Height in pixels. */
        void set_size(st::float32 w,
                      st::float32 h) noexcept;
        /** @brief Resizes the frame on the next frame. @param size Size in pixels. */
        void set_size(glm::f32vec2 size) noexcept;
        /** @brief Sets whether the titlebar is drawn. @param show_titlebar Whether to draw it. */
        void set_show_titlebar(st::bool_t show_titlebar) noexcept;
        /** @brief Sets the title. @param title Title in UTF-8. */
        void set_title(std::string_view title) noexcept;

        /** @brief Called once when the frame closes. */
        using on_close_callback = std::function<void()>;

        /** @brief Adds a close callback. @param callback The callback. */
        void register_on_close(const on_close_callback &callback) noexcept;

        /** @brief Removes every close callback. */
        void clear_on_close() noexcept;

        /** @brief Fades the frame out and then closes it. Input is ignored while it fades. */
        void fade_out() noexcept;

    protected:
        /** @brief Fires the close callbacks once and then drops them. */
        void handle_close() noexcept override;

    private:
        struct frame_impl;
        std::unique_ptr<frame_impl> m_impl;
    };
} // namespace st::gui

/** @} */

#endif // STICKY_GUI_FRAME_H
