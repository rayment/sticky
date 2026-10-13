/*
 * popup.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

/**
 * @file
 * @brief Native message boxes for messages that must reach the user.
 */

#ifndef STICKY_OS_POPUP_H
#define STICKY_OS_POPUP_H

#include "st_types.h"

/** @addtogroup os
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @brief C binding of st::os::popup::info(). */
void st_os_popup_info(const st_char *title,
                      const st_char *message);
/** @brief C binding of st::os::popup::warn(). */
void st_os_popup_warn(const st_char *title,
                      const st_char *message);
/** @brief C binding of st::os::popup::error(). */
void st_os_popup_error(const st_char *title,
                       const st_char *message);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <string>

/**
 * @brief Blocking native message boxes.
 *
 * Every message is also written to the default log at the matching level. While popups are disabled, messages are only
 * logged, which keeps headless runs such as the unit tests from blocking on a dialog.
 */
namespace st::os::popup
{
    /**
     * @brief Enables or disables the message boxes. They are enabled by default.
     * @param enabled Whether message boxes are shown.
     */
    void set_enabled(st::bool_t enabled) noexcept;

    /** @brief Whether message boxes are shown. */
    [[nodiscard]] st::bool_t is_enabled() noexcept;

    /**
     * @brief Shows an information message and blocks until the user dismisses it.
     * @param title Title of the message box, in UTF-8.
     * @param message Body of the message box, in UTF-8.
     */
    void info(const std::string &title,
              const std::string &message) noexcept;
    /** @brief Shows a warning message. @copydetails info(const std::string &, const std::string &) */
    void warn(const std::string &title,
              const std::string &message) noexcept;
    /** @brief Shows an error message. @copydetails info(const std::string &, const std::string &) */
    void error(const std::string &title,
               const std::string &message) noexcept;

    /** @brief Shows an information message titled "libsticky". @param message Body of the message box, in UTF-8. */
    void info(const std::string &message) noexcept;
    /** @brief Shows a warning message titled "libsticky". @param message Body of the message box, in UTF-8. */
    void warn(const std::string &message) noexcept;
    /** @brief Shows an error message titled "libsticky". @param message Body of the message box, in UTF-8. */
    void error(const std::string &message) noexcept;
} // namespace st::os::popup
#endif // __cplusplus

/** @} */

#endif // STICKY_OS_POPUP_H
