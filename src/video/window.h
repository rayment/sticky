/*
 * window.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#ifndef STICKY_VIDEO_WINDOW_H
#define STICKY_VIDEO_WINDOW_H

#include "st_includes.h"
#include "st_primitives.h"
#include "gui/attributes.h"
#include "math/vec3.h"
#include "type/string.h"

#ifdef __cplusplus
extern "C" {
#endif

st_bool     st_window_init(void);
st_bool     st_window_free(void);

st_bool     st_window_is_close_requested(void);
st_bool     st_window_center(void);
st_bool     st_window_close(void);
st_bool     st_window_hide(void);
st_float64  st_window_delta_time(void);
st_bool     st_window_reset_timing(void);
st_bool     st_window_set_clear_color(st_vec3_float32_t color);
st_bool     st_window_set_fixed_step_rate(st_uint32 rate);
st_bool     st_window_set_title(st_string_t title);
st_bool     st_window_show(void);
st_string_t st_window_title(void);

st_bool     st_window_pause_for_fixed_step(void);
st_bool     st_window_update_delta(void);
st_bool     st_window_poll_events(void);
st_bool     st_window_new_frame(void);
st_bool     st_window_render_frame(void);
st_bool     st_window_swap_frame(void);

#ifdef __cplusplus
}
#endif

#endif // STICKY_VIDEO_WINDOW_H
