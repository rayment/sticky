/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * window.h
 * Video window header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_WINDOW_H
#define FR_RAYMENT_STICKY_WINDOW_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/concurrency/mutex.h"
#include "sticky/concurrency/thread.h"
#include "sticky/input/keyboard.h"
#include "sticky/math/transform.h"
#include "sticky/math/vec4.h"
#include "sticky/video/camera.h"
#include "sticky/video/shader.h"
#include "sticky/video/texture.h"

/* forward declaration */
struct Scamera_s;

/**
 * @addtogroup window
 * @{
 */

/**
 * @brief Windowed display mode.
 * @hideinitializer
 *
 * Denotes that a given {@link Swindow} should render in windowed mode.
 *
 * @see {@link S_window_set_display_mode(Swindow *, Senum)} To set the display
 * mode.
 * @since 1.0.0
 */
#define S_WINDOWED         (0)

/**
 * @brief Borderless display mode.
 * @hideinitializer
 *
 * Denotes that a given {@link Swindow} should render in borderless fullscreen
 * mode.
 *
 * @see {@link S_window_set_display_mode(Swindow *, Senum)} To set the display
 * mode.
 * @since 1.0.0
 */
#define S_BORDERLESS       (SDL_WINDOW_FULLSCREEN_DESKTOP)

/**
 * @brief Fullscreen display mode.
 * @hideinitializer
 *
 * Denotes that a given {@link Swindow} should render in fullscreen mode.
 *
 * @see {@link S_window_set_display_mode(Swindow *, Senum)} To set the display
 * mode.
 * @since 1.0.0
 */
#define S_FULLSCREEN       (SDL_WINDOW_FULLSCREEN)

/**
 * @brief Keyboard input mode.
 * @hideinitializer
 *
 * Denotes the keyboard input method that a window may accept as input.
 *
 * @see {@link S_window_set_input_mode(Swindow *, Senum)} To set the input mode.
 * @since 1.0.0
 */
#define S_KEYBOARD 0x1

/**
 * @brief Mouse input mode.
 * @hideinitializer
 *
 * Denotes the mouse input method that a window may accept as input.
 *
 * @see {@link S_window_set_input_mode(Swindow *, Senum)} To set the input mode.
 * @since 1.0.0
 */
#define S_MOUSE    0x2

/**
 * @brief Gamepad input mode.
 * @hideinitializer
 *
 * Denotes the gamepad input method that a window may accept as input.
 *
 * @see {@link S_window_set_input_mode(Swindow *, Senum)} To set the input mode.
 * @since 1.0.0
 */
#define S_GAMEPAD  0x4

/**
 * @brief Compatibility GL profile.
 * @hideinitializer
 *
 * Denotes that a given {@link Swindow} should use the compatibility OpenGL
 * profile. This profile may be required for older systems.
 *
 * @see {@link S_window_set_gl_profile(Swindow *, Senum)} To set the OpenGL
 * profile.
 * @since 1.0.0
 */
#define S_GL_COMPATIBILITY (SDL_GL_CONTEXT_PROFILE_COMPATIBILITY)

/**
 * @brief Core GL profile.
 * @hideinitializer
 *
 * Denotes that a given {@link Swindow} should use the core OpenGL profile. This
 * is the default profile.
 *
 * @see {@link S_window_set_gl_profile(Swindow *, Senum)} To set the OpenGL
 * profile.
 * @since 1.0.0
 */
#define S_GL_CORE          (SDL_GL_CONTEXT_PROFILE_CORE)

/**
 * @brief Display window.
 *
 * The window is the primary viewport for an application and is typically
 * displayed on the screen with a given width and height, or in the case of
 * fullscreen, occupy the screen in its entirety. All rendering performed by
 * <b>sticky</b> will invariably end up appearing in a window, thus it is
 * imperative to use this struct to manage the display output logic.
 *
 * Windows can be customised, and the OpenGL versions and profiles can also be
 * manually set. The lifecycle of a window begins by calling
 * {@link S_window_new(void)}, then opening it (after changing some settings)
 * with {@link S_window_apply(Swindow *)}, then closing it after a period of
 * rending with {@link S_window_close(Swindow *)} and finally freeing it from
 * memory with {@link S_window_delete(Swindow *)}.
 *
 * @warning Due to limitations in the way GL contexts are created in
 * <b>sticky</b>, an <c>Swindow</c> <b>must</b> have been created before any
 * other video function, whether it be the creation or modification of meshes,
 * cameras, textures, etc. be called, or else the function behaviour will be
 * undefined, due to the GL context not yet being created.
 * @warning Only one active window at a time is supported by <b>sticky</b>.
 * Trying to create a second window without having previously called
 * {@link S_window_delete(Swindow *)} on the first will throw an error.
 * @since 1.0.0
 */
typedef struct
Swindow_s
{
	/* primary window data */
	SDL_Window *window;
	SDL_GLContext *context;
	SDL_Surface *icon;
	Senum display_mode, gl_profile;
	Suint8 gl_major, gl_minor;
	Suint16 width, height;
	Svec4 clear_color;
	Sbool centered, resizable, capture_mouse, vsync, hwaccel, doublebuf;
	Schar title[64];
	Senum input_mode;
	Sbool running;
	Suint16 ticks, tick_limit, skip_ticks, next_tick;
	Suint32 delta_time, current_frame, last_frame;
	struct Scamera_s *cam;
	Sbool camresize;
	/* draw data */
	Sshader *dshader;
	Svec4 dcolor;
	Stransform *dtrans;
	const Stexture *dtex;
	/* callbacks */
	void (*on_exit)(struct Swindow_s *);
	void (*on_resize)(struct Swindow_s *);
} Swindow;

/**
 * @addtogroup window_callback
 * @{
 */

/**
 * @brief Window callback function.
 *
 * Defines a callback function that interacts with a window.
 *
 * When a certain event occurs, such as the user attempting to close a window or
 * presses a key on the keyboard (and the keyboard input method is enabled),
 * then the window will handle the event, but may optionally call a callback
 * function as well, which may be defined by the user for their own purposes.
 *
 * @since 1.0.0
 */
typedef void (*Swindow_callback)(Swindow *);

/**
 * @}
 */

/**
 * @brief Create a new window.
 *
 * Creates a new window, allocates it some memory on the heap and prepares it
 * with the default window parameters.
 *
 * The window is <b>not</b> open or ready to render after this function is
 * called. To actually display the window, {@link S_window_apply(Swindow *)}
 * needs to be called.
 *
 * @return A new window.
 * @exception S_INVALID_OPERATION If a window already exists and
 * {@link S_window_delete(Swindow *)} has not yet been called.
 * the function.
 * @since 1.0.0
 */
STICKY_API Swindow *S_window_new(void);

/**
 * @brief Free a window from memory.
 *
 * Once this function is called for a given window, that window becomes invalid
 * and may not be used again in any other window function.
 *
 * @warning This function does not close the window. The window <b>must</b> be
 * closed via. {@link S_window_close(Swindow *)} before calling this function.
 * @param[in,out] window The window to be free'd.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_delete(Swindow *);

/**
 * @brief Apply any window settings and initialise it.
 *
 * Applies any custom settings applied to the window. If this function is called
 * for the first time, the window will additionally be opened, and the OpenGL
 * rendering context shall be created.
 *
 * Note that the following settings cannot be applied during runtime, and will
 * require a program restart to apply the settings:
 *
 * <ul>
 *   <li>{@link S_window_set_gl_profile(Swindow *, Senum)}</li>
 *   <li>{@link S_window_set_gl_version(Swindow *, Suint8, Suint8)}</li>
 *   <li>{@link S_window_set_centered(Swindow *, Sbool)}</li>
 *   <li>{@link S_window_set_double_buffer(Swindow *, Sbool)}</li>
 *   <li>{@link S_window_set_hardware_acceleration(Swindow *, Sbool)}</li>
 * </ul>
 *
 * @param[in,out] window The window to update.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_apply(Swindow *);

/**
 * @brief Close a window.
 *
 * Calling this function will cause {@link S_window_is_running(const Swindow *)}
 * to return {@link S_FALSE} which in turn can be used to exit a game loop. Note
 * that it does not <i>actually</i> close the window.
 *
 * @param[in,out] window The window to close.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_close(Swindow *);

/**
 * @brief Clears a window.
 *
 * Calling this function will clear the window of any geometry. It should the
 * first function called in a game loop, as it also handles calculation of the
 * game ticks.
 *
 * @param[in,out] window The window to clear.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_clear(Swindow *);

/**
 * @brief Polls a window for input or events.
 *
 * This function is mandatory for a given window to be able to respond to OS
 * requests such as focus or display, accept input or close and as such should
 * be called once at the beginning of a game loop.
 *
 * After calling this function, any input-related functions can be used to
 * ascertain if a certain key, mouse button, gamepad button, etc. was pressed.
 *
 * @param[in,out] window The window to poll.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_poll(Swindow *);

/**
 * @brief Clears a window.
 *
 * Rendering to a window occurs on a 'hidden' frame while a 'visible' frame is
 * being shown on the display. When this function is called, the hidden frame
 * is swapped out with the visible one to show the results of the latest render.
 *
 * This function should be called after all render calls for a given frame.
 *
 * @param[in,out] window The window to swap.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_swap(Swindow *);

/**
 * @brief Notify a window of a tick update.
 *
 * When a certain period of time has passed, a tick will occur and the game loop
 * shall begin processing the tick. At the end of a given tick, this function is
 * called to notify the window that the tick has been completed.
 *
 * @param[in,out] window The window to swap.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @see {@link S_window_is_tick(const Swindow *)} To know when a tick should
 * occur.
 * @since 1.0.0
 */
STICKY_API void     S_window_tick(Swindow *);

/**
 * @brief Set the title of a window.
 *
 * The @p title string must be at max 64 chars long, including a null-terminator
 * character at the end of the string. If a <c>NULL</c> title is passed to the
 * function, then the title will be blanked.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window whose title shall be updated.
 * @param[in] title The title string.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_title(Swindow *, const Schar *);

/**
 * @brief Set the size of a window.
 *
 * The @p width and @p height parameters are measured in pixels.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window whose size shall be updated.
 * @param[in] width The new width of the window in pixels.
 * @param[in] height The new height of the window in pixels.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_size(Swindow *, Suint16, Suint16);

/**
 * @brief Allow a window to be resized.
 *
 * By default, windows are not resizable. Calling this function with @p resize
 * set to {@link S_TRUE} will allow the window to be resized by the user.
 *
 * Note that resizing can be turned off but size changes that occur due to
 * entering or exiting fullscreen mode from a window will still be permitted.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] resizable Whether or not to allow the window to be resized.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_resizable(Swindow *, Sbool);

/**
 * @brief Set the mouse-capture rule for a window.
 *
 * If @p capture is set to {@link S_TRUE} then the mouse will automatically be
 * grabbed by the window and made invisible. Any user attempt to move the mouse
 * outside of the window will fail and the mouse will remain invisible until
 * either the window loses focus, the window closes, or this function is called
 * again with {@link S_FALSE}.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] capture Whether or not to capture the mouse inside of the window.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_capture_mouse(Swindow *, Sbool);

/**
 * @brief Set the centering rule for a window.
 *
 * If @p centered is equal to {@link S_TRUE}, then the window will automatically
 * be placed in the center of the screen when opened. Otherwise, the window will
 * be placed arbitrarily.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}. Keep in mind that this function will have
 * no effect if run after the first call to {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] centered Whether or not the window should be centered once opened.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_centered(Swindow *, Sbool);

/**
 * @brief Set the display mode for a window.
 *
 * Setting the display mode to {@link S_WINDOWED} will cause the window to
 * appear as an actual window on the display (this of course depends on the
 * display compositor).
 *
 * Setting the display mode to {@link S_BORDERLESS} will cause the window to
 * appear without a border and be the same size as the screen.
 *
 * Setting the display mode to {@link S_FULLSCREEN} will cause the window to
 * appear fullscreen and capture the entire displays video output.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] mode The display mode to apply to the window.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @exception S_INVALID_ENUM If an invalid display mode not listed above is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_display_mode(Swindow *, Senum);

/**
 * @brief Toggle vsync for a window.
 *
 * Toggle vertical synchronisation for a window. If this is turned off, screen
 * tearing may occur in rendering.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] vsync Whether or not to use vsync in rendering.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_vsync(Swindow *, Sbool);

/**
 * @brief Set the OpenGL profile for a window.
 *
 * Setting the render profile to {@link S_GL_COMPATIBILITY} for a given window
 * will cause it to use the forward compatibility profile.
 *
 * Setting the render profile to {@link S_GL_CORE} for a given window will cause
 * it to use the forward compatibility profile. Note that some operating systems
 * require this to be set due to deprecation of older OpenGL functionality
 * present in forward compatible profiles.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}. Keep in mind that this function will have
 * no effect if run after the first call to {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] profile The GL profile to apply to the window.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @exception S_INVALID_ENUM If an invalid profile not listed above is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_gl_profile(Swindow *, Senum);

/**
 * @brief Set the OpenGL version for a window.
 *
 * Sets the version that OpenGL should use for a given window. Depending on the
 * operating system and graphics card being used, some GL versions may or may
 * not be supported at all.
 *
 * The parameters @p major and @p minor together signify a GL version
 * <c>major.minor</c>. As an example, OpenGL 3.1 would be identified by
 * <c>major=3</c> and <c>minor=1</c>.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}. Keep in mind that this function will have
 * no effect if run after the first call to {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] major The major version number.
 * @param[in] minor The minor version number.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_gl_version(Swindow *, Suint8, Suint8);

/**
 * @brief Set the clear colour for a window.
 *
 * Each time {@link S_window_clear(Swindow *)} is called, the colour set by this
 * function (default is black) will be used to clear the window from any
 * geometry.
 *
 * The colour is defined by four components, @p r referring to red, @p g
 * referring to green, @p b referring to blue and @p a to the alpha value.
 * The floats should be a number between <c>0.0f</c> and <c>1.0f</c>. If using
 * an RGBA system that relies on integer values between 0 and 255, they can
 * easily be clamped between 0 and 1 by performing the operation
 * @f$value/255@f$.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] r The red component of the clear colour.
 * @param[in] g The green component of the clear colour.
 * @param[in] b The blue component of the clear colour.
 * @param[in] a The alpha component of the clear colour.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_clear_color(Swindow *,
                                             Sfloat, Sfloat, Sfloat, Sfloat);

/**
 * @brief Set hardware acceleration for a window.
 *
 * If @p hwaccel is set to {@link S_TRUE}, then hardware acceleration will be
 * enabled for a given window. Otherwise, no acceleration will be taken
 * advantage of for the duration of the program.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}. Keep in mind that this function will have
 * no effect if run after the first call to {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] hwaccel Whether or not to use hardware acceleration.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_hardware_acceleration(Swindow *, Sbool);

/**
 * @brief Set double buffering for a window.
 *
 * If @p doublebuf is set to {@link S_TRUE}, then double buffering will be
 * enabled for a given window. Otherwise, no double buffering will be taken
 * advantage of for the duration of the program.
 *
 * Note that this function will not apply immediately. To do so, use
 * {@link S_window_apply(Swindow *)}. Keep in mind that this function will have
 * no effect if run after the first call to {@link S_window_apply(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] doublebuf Whether or not to enable double buffering for a given
 * window.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_double_buffer(Swindow *, Sbool);

/**
 * @brief Sets the number of ticks per second for a window.
 *
 * Sets the number of times a second that
 * {@link S_window_is_tick(const Swindow *)} should return {@link S_TRUE}.
 * Ticks are independant from frames, though they may affect the framerate if
 * each tick takes too long to compute.
 *
 * If 0 ticks per second are requested, then the default rate of 30 ticks per
 * second will be used instead.
 *
 * @param[in,out] window The window.
 * @param[in] ticks The number of ticks per second.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_ticks_per_second(Swindow *, Suint16);

/**
 * @brief Sets the input mode for a window.
 *
 * Setting the input mode to {@link S_KEYBOARD} will cause the window to only
 * accept input from the keyboard.
 *
 * Setting the input mode to {@link S_MOUSE} will cause the window to only
 * accept input from the mouse.
 *
 * Setting the input mode to {@link S_GAMEPAD} will cause the window to only
 * accept input from a gamepad.
 *
 * Setting the input mode to a combination of any of the above modes by OR'ing
 * them together (eg. a mouse and keyboard combination would be
 * <c>S_MOUSE | S_KEYBOARD</c>) will allow all of those modes of input to be
 * used.
 *
 * The default input mode is both mouse and keyboard.
 *
 * @param[in,out] window The window.
 * @param[in] mode The input mode.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_input_mode(Swindow *, Senum);

/**
 * @addtogroup window_callback
 * @{
 */

/**
 * @brief Sets the exit callback for a window.
 *
 * The exit callback will be called when the window receives a signal to exit
 * from the OS from the user.
 *
 * If a <c>NULL</c> callback is provided, the default behaviour will occur,
 * which is a direct call to {@link S_window_close(Swindow *)}.
 *
 * @param[in,out] window The window.
 * @param[in] callback The exit callback function.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_callback_on_exit(Swindow *,
                                                  Swindow_callback);

/**
 * @brief Sets the resize callback for a window.
 *
 * The resize callback will be called when the window is resized by the user or
 * enters or exits from fullscreen mode.
 *
 * If a <c>NULL</c> callback is provided, the default behaviour will occur,
 * which is nothing.
 *
 * @param[in,out] window The window.
 * @param[in] callback The resize callback function.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_set_callback_on_resize(Swindow *,
                                                    Swindow_callback);

/**
 * @}
 */

/**
 * @brief Get the size of a window.
 *
 * Puts the width and height of a window in pixels into the @p width and @p
 * height parameters respectively. Note that this size is not necessarily the
 * current size of the window (think of fullscreen mode) but rather the size
 * that was specifically requested via.
 * {@link S_window_set_size(Swindow *, Suint16, Suint16)}.
 *
 * If a <c>NULL</c> value is given, that field will not be filled.
 *
 * @param[in] window The window.
 * @param[out] width The width of a given window in pixels.
 * @param[out] height The height of a given window in pixels.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_window_get_size(const Swindow *,
                                      Suint16 *, Suint16 *);

/**
 * @brief Get the delta time of a window.
 *
 * Delta time is the interval of time between the current frame and the last
 * frame to be rendered. Multiplying movement or animation of objects by this
 * value has the advantage of the movement being constant, even during periods
 * of low framerate, rather than slowing down.
 *
 * @param[in] window The window.
 * @return The delta time between the current and last frame.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sfloat   S_window_get_delta_time(const Swindow *);

/**
 * @brief Get the number of video displays available.
 *
 * Video displays are used to output the window contents. This function will
 * query the operating system and return the number of available displays that
 * can be used.
 *
 * @return The number of available video displays.
 * @since 1.0.0
 */
STICKY_API Suint8   S_window_get_num_displays(void);

/**
 * @brief Get display size and refresh rate.
 *
 * This function queries the operating system for a given display index and
 * returns the width and height in pixels, and the refresh rate in Hz (or 0 if
 * the refresh rate is unspecified).
 *
 * If any pointer argument is equal to <c>NULL</c>, then the given value will
 * not be set.
 *
 * @param[in] id The display index. See {@link S_window_get_num_displays(void)}.
 * @param[out] w The display width in pixels.
 * @param[out] h The display height in pixels.
 * @param[out] refresh The display refresh rate in Hz.
 * @exception S_INVALID_VALUE If the given display index is invalid.
 * @since 1.0.0
 */
STICKY_API void     S_window_get_display_info(Suint8, Sint32 *, Sint32 *,
                                              Sint32 *);

/**
 * @brief Check if a window is currently running.
 *
 * Returns {@link S_TRUE} if and only if {@link S_window_apply(Swindow *)} has
 * previously been called, opening a window and running it, and
 * {@link S_window_close(Swindow *)} has <b>not</b> yet been called.
 *
 * This value may be used to drive a game loop, keeping the application running
 * until the window should close.
 *
 * @param[in] window The window.
 * @return {@link S_TRUE} if a given window is running, otherwise
 * {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sbool    S_window_is_running(const Swindow *);

/**
 * @brief Check if a window is ready for a tick.
 *
 * Returns {@link S_TRUE} if enough time has passed and a given window wants to
 * tick. The number of ticks per second may be set via. the function
 * {@link S_window_set_ticks_per_second(Swindow *, Suint16)}.
 *
 * This value may be used to drive a tick-based loop internal to the game loop,
 * performing tick-based operations multiple times per second, independant of
 * the framerate. Keep in mind that the amount of time spent computing a single
 * tick should still be small enough to not affect the framerate.
 *
 * @param[in] window The window.
 * @return {@link S_TRUE} if a given window is expecting a tick, otherwise
 * {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sbool    S_window_is_tick(const Swindow *);

Sbool _S_window_is_init(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_WINDOW_H */

