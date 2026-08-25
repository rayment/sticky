/*
 * log.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#ifndef STICKY_LOG_LOG_H
#define STICKY_LOG_LOG_H

#include "st_primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

st_bool st_log_init(void);
void    st_log_free(void);

void    st_log_trace(const st_char *fmt, ...);
void    st_log_debug(const st_char *fmt, ...);
void    st_log_info (const st_char *fmt, ...);
void    st_log_warn (const st_char *fmt, ...);
void    st_log_error(const st_char *fmt, ...);

st_bool st_log_file_set_filename(const st_char *filename);
void    st_log_file_flush(void);

#ifdef __cplusplus
}
#endif

#endif // STICKY_LOG_LOG_H
