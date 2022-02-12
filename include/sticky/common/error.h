/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * error.h
 * Error handler and logger header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

/**
 * @file error.h
 * @addtogroup error Errors and logging
 * @{
 */

#ifndef FR_RAYMENT_STICKY_ERROR_H
#define FR_RAYMENT_STICKY_ERROR_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "sticky/common/types.h"
#ifdef DEBUG
#include "sticky/memory/memtrace.h"
#endif /* DEBUG */

static Senum   _S_error;
static Schar  *_S_error_loc;
/**
 * @brief A global enum that stores the current error number.
 * @hideinitializer
 *
 * Whenever a <b>sticky</b> function is called and an exception occurs, so long
 * as the exception is not a critical error (and thus the application killed),
 * <c>SERRNO</c> will be set to one of a known set of values to give indication
 * as to the kind of error that occurred. This can then be read in a
 * human-friendly format using the string obtained from the
 * {@link S_error_string(void)} function.
 *
 * The function that caused the error can be found in the {@link SERRLOC} global
 * variable.
 *
 * The default value is {@link S_NO_ERROR}.
 *
 * @since 1.0.0
 */
#define SERRNO   _S_error
/**
 * @brief A global string that stores the function that the error occurred in
 * for printing context.
 * @hideinitializer
 *
 * This variable can be used alongside the error string generated by
 * {@link S_error_string(void)} much like the standard function
 * <b><c>perror(const char *)</c></b> which will print the culprit function and
 * the error. It will be set alongside {@link SERRNO} and is equivalent to a
 * <b><c>const Schar *</c></b>.
 *
 * The default value is <c>"null"</c>.
 *
 * @since 1.0.0
 */
#define SERRLOC  _S_error_loc

#define _S_ERR_LOC __FILE__, __LINE__
#define _S_SET_ERROR(x, y)  \
	SERRNO   = x;        \
	SERRLOC  = y

/**
 * @addtogroup error_codes Error codes
 * @{
 */

/**
 * @brief No error.
 * @since 1.0.0
 */
#define S_NO_ERROR          0
/**
 * @brief An unknown error occurred.
 * @since 1.0.0
 */
#define S_UNKNOWN_ERROR     1
/* general errors */
/**
 * @brief An error occurred because an invalid value was given.
 * @since 1.0.0
 */
#define S_INVALID_VALUE     101
/**
 * @brief An error occurred because an invalid enum value was given.
 * @since 1.0.0
 */
#define S_INVALID_ENUM      102
/**
 * @brief An error occurred because an invalid operation was requested.
 * @since 1.0.0
 */
#define S_INVALID_OPERATION 103
/**
 * @brief An error occurred because an invalid address was given.
 * @since 1.0.0
 */
#define S_INVALID_ADDRESS   104
/**
 * @brief An error occurred because of an invalid request to access a resource.
 * @since 1.0.0
 */
#define S_INVALID_ACCESS    105
/**
 * @brief An error occurred because an invalid format was given.
 * @since 1.0.0
 */
#define S_INVALID_FORMAT    106
/* system errors */
/**
 * @brief An error occurred because the machine or program ran out of available
 * memory.
 * @since 1.0.0
 */
#define S_OUT_OF_MEMORY     201

/**
 * @}
 */

/**
 * @brief The string representation of the current error pointed to by
 * {@link SERRNO}.
 *
 * Returns a human-readable reprentation of the enum value that {@link SERRNO}
 * refers to. If no error has yet occurred during runtime, {@link SERRNO} will
 * be set to {@link S_NO_ERROR} and thereby the error message will be
 * meaningless.
 *
 * To get the function that the error occurred in, see {@link SERRLOC}.
 *
 * @return The string representation of the current error pointed to by
 * {@link SERRNO}.
 * @since 1.0.0
 */
static inline
const char *
S_error_string(void)
{
	(void) SERRLOC;
	switch (SERRNO)
	{
	case S_NO_ERROR          : return "No error.";
	case S_INVALID_VALUE     : return "Invalid value.";
	case S_INVALID_ENUM      : return "Invalid enum.";
	case S_INVALID_OPERATION : return "Invalid operation.";
	case S_INVALID_ADDRESS   : return "Invalid address.";
	case S_INVALID_ACCESS    : return "Invalid access.";
	case S_INVALID_FORMAT    : return "Invalid format.";
	case S_OUT_OF_MEMORY     : return "Out of memory.";
	default                  : return "Unknown error.";
	}
}

#define _S_LOG_MSG   0
#define _S_LOG_WARN  1
#define _S_LOG_ERROR 2

static inline
void
_S_log(const Schar *location,
       const Suint32 line,
       const Schar *type,
       const char *format,
       const Senum err,
       va_list va)
{
	FILE *out;
	if (err == _S_LOG_ERROR)
	{
		out = stderr;
		fprintf(out, "\n\x1b[1;31m%s\x1b[0m: %s:%d: ", type, location, line);
	}
	else if (err == _S_LOG_WARN)
	{
		out = stderr;
		fprintf(out, "\x1b[1;33m%s\x1b[0m: %s:%d: ", type, location, line);
	}
	else
	{
		out = stdout;
		fprintf(out, "%s: %s:%d: ", type, location, line);
	}
	vfprintf(out, format, va);
	va_end(va);
	if (err == _S_LOG_ERROR)
	{
#ifdef DEBUG_TRACE
		_S_memtrace_stack_trace();
#endif /* DEBUG_TRACE */
		exit(EXIT_FAILURE);
	}
}

static inline
void
_S_log_vararg(const Schar *location,
              const Suint32 line,
              const Schar *type,
              const char *format,
              const Senum err,
              ...)
{
	va_list va;
	va_start(va, err);
	_S_log(location, line, type, format, err, va);
}

static inline
void
_S_assert(const Schar *location,
          const Suint32 line,
          const Sbool condition,
          const char *format,
          ...)
{
	va_list va;
	if (!condition)
	{
		va_start(va, format);
		_S_log(location, line, "ASSRT", format, _S_LOG_ERROR, va);
	}
}

/**
 * @brief Log an error message to standard error output and terminate the
 * program.
 * @hideinitializer
 *
 * Wrapper for the standard function
 * <b><c>fprintf(stderr, const char *, ...)</c></b> and will additionally print
 * the file and location that the log event occurred at, before calling
 * <b><c>exit(EXIT_FAILURE)</c></b> and thereby terminating the program.
 *
 * All formatting rules supported by <b><c>fprintf</c></b> are supported by this
 * function.
 *
 * @param[in] format The text to be printed to <b><c>stderr</c></b>.
 * @param[in] ... Any additional arguments to be placed within the print text.
 * @since 1.0.0
 */
#define S_error(format, ...) \
        _S_log_vararg(_S_ERR_LOC, "ERROR", format, _S_LOG_ERROR, ##__VA_ARGS__)

/**
 * @brief Assert a condition and cause a fatal error if the condition is false.
 * @hideinitializer
 *
 * Checks to see if the argument <c>condition</c> is equal to {@link S_TRUE} and
 * if not, internally call {@link S_error}, log a given error message along with
 * the file and line number that the assertion occurred at, and finally
 * terminate the program.
 *
 * All formatting rules supported by <b><c>printf</c></b> are supported by this
 * function.
 *
 * @param[in] condition The condition that must be equal to {@link S_TRUE} or
 * else a fatal error will occur and the program be terminated.
 * @param[in] format The text to be printed to <b><c>stderr</c></b> in the case
 * that the assertion fails.
 * @param[in] ... Any additional arguments to be placed within the print text.
 * @since 1.0.0
 */
#define S_assert(condition, format, ...) \
        _S_assert(_S_ERR_LOC, condition, format, ##__VA_ARGS__)

/**
 * @brief Log a warning message to standard error output.
 * @hideinitializer
 *
 * Wrapper for the standard function
 * <b><c>fprintf(stderr, const char *, ...)</c></b> and will additionally print
 * the file and location that the log event occurred at.
 *
 * All formatting rules supported by <b><c>fprintf</c></b> are supported by this
 * function.
 *
 * @param[in] format The text to be printed to <b><c>stderr</c></b>.
 * @param[in] ... Any additional arguments to be placed within the print text.
 * @since 1.0.0
 */
#define S_warning(format, ...) \
        _S_log_vararg(_S_ERR_LOC, "WARN ", format, _S_LOG_WARN, ##__VA_ARGS__)


/**
 * @brief Log a message to standard output.
 * @hideinitializer
 *
 * Wrapper for the standard function <b><c>printf(const char *, ...)</c></b> and
 * will additionally print the file and location that the log event occurred at.
 *
 * All formatting rules supported by <b><c>printf</c></b> are supported by this
 * function.
 *
 * @param[in] format The text to be printed to <b><c>stdout</c></b>.
 * @param[in] ... Any additional arguments to be placed within the print text.
 * @since 1.0.0
 */
#define S_log(format, ...) \
        _S_log_vararg(_S_ERR_LOC, "LOG  ", format, _S_LOG_MSG, ##__VA_ARGS__)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_ERROR_H */

