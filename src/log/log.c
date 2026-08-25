/*
 * log.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "log/log.h"

typedef enum st_log_level {
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} st_log_level_t;

typedef struct st_log_state
{
    FILE *file;
    st_bool initialised;
} st_log_state_t;

static st_log_state_t log_state;

static const st_char *
log_level_str(st_log_level_t level)
{
    switch (level)
    {
    case LOG_LEVEL_TRACE: return "TRACE";
    case LOG_LEVEL_DEBUG: return "DEBUG";
    case LOG_LEVEL_INFO : return "INFO ";
    case LOG_LEVEL_WARN : return "WARN ";
    case LOG_LEVEL_ERROR: return "ERROR";
    default             : return "?????";
    }
}

static void
log_get_timestamp(st_char *buf,
                  st_size len)
{
    time_t now;
    struct tm tm_now;

    now = time(nullptr);

#ifdef _WIN32
    localtime_s(&tm_now, &now);
#else
    localtime_r(&now, &tm_now);
#endif

    strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm_now);
}

static void log_vwrite(st_log_level_t level,
                       const st_char *format,
                       va_list args)
{
    st_char timestamp[32];
    st_char message[4096];

    if (!log_state.initialised)
        return;

    log_get_timestamp(timestamp, sizeof(timestamp));

    vsnprintf(message, sizeof(message), format, args);

    fprintf(stdout,
            "[%s] [%s] %s\n",
            timestamp,
            log_level_str(level),
            message);

    if (log_state.file != nullptr)
    {
        fprintf(log_state.file,
                "[%s] [%s] %s\n",
                timestamp,
                log_level_str(level),
                message);
    }
}

static void
log_write(st_log_level_t level,
          const st_char *format,
          va_list args)
{
    va_list stdout_args;
    va_copy(stdout_args, args);

    log_vwrite(level, format, stdout_args);

    va_end(stdout_args);
}

st_bool
st_log_init(void)
{
    if (log_state.initialised)
        return false;
    log_state.file = nullptr;
    log_state.initialised = true;
    return true;
}

void
st_log_free(void)
{
    if (!log_state.initialised)
        return;

    if (log_state.file != nullptr)
        fclose(log_state.file);
    fflush(stdout);

    log_state.initialised = false;
    log_state.file = nullptr;
}

#ifdef NDEBUG
void st_log_trace(ST_UNUSED const st_char *fmt, ...) {}
void st_log_debug(ST_UNUSED const st_char *fmt, ...) {}
#else
void
st_log_trace(const st_char *fmt,
             ...)
{
    va_list args;

    va_start(args, fmt);
    log_write(LOG_LEVEL_TRACE, fmt, args);
    va_end(args);
}

void
st_log_debug(const st_char *fmt,
             ...)
{
    va_list args;

    va_start(args, fmt);
    log_write(LOG_LEVEL_DEBUG, fmt, args);
    va_end(args);
}
#endif

void
st_log_info(const st_char *fmt,
            ...)
{
    va_list args;

    va_start(args, fmt);
    log_write(LOG_LEVEL_INFO, fmt, args);
    va_end(args);
}

void
st_log_warn(const st_char *fmt,
            ...)
{
    va_list args;

    va_start(args, fmt);
    log_write(LOG_LEVEL_WARN, fmt, args);
    va_end(args);
}

void
st_log_error(const st_char *fmt,
             ...)
{
    va_list args;

    va_start(args, fmt);
    log_write(LOG_LEVEL_ERROR, fmt, args);
    va_end(args);
}
