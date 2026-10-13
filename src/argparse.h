/*
 * argparse.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 31/08/2026
 */

/**
 * @file
 * @brief Command line options of the executable.
 */

#ifndef STICKY_ARGPARSE_H
#define STICKY_ARGPARSE_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <memory>

#include "st_macros.h"
#include "st_types.h"

/** @addtogroup app
 *  @{
 */

namespace st
{

    /**
     * @brief Parses the command line: `--developer`, `--help`, `--safe-mode` and `--version`.
     *
     * `--help`, `--version` and unknown options print to standard output and ask the program to exit.
     */
    class argparse
    {
    public:
        /** @brief Parses the arguments of `main`. @param argc Argument count. @param argv Argument values. */
        explicit argparse(st::int32    argc,
                          st::char_t **argv) noexcept;
        ~argparse() noexcept;

        ST_DELETE_COPY_CTORS(argparse);
        ST_DELETE_MOVE_CTORS(argparse);

        /** @brief The number of arguments that are not options. */
        [[nodiscard]] st::size num_non_args() const noexcept;
        /** @brief The arguments that are not options. */
        [[nodiscard]] st::char_t **non_args() const noexcept;
        /** @brief Whether `--developer` was given. Reserved for developer tooling. */
        [[nodiscard]] st::bool_t developer_mode() const noexcept;
        /** @brief Whether `--safe-mode` was given, which starts without loading any mods. */
        [[nodiscard]] st::bool_t safe_mode() const noexcept;
        /** @brief Whether the program should exit at once with exit_code(). */
        [[nodiscard]] st::bool_t should_exit() const noexcept;
        /** @brief The exit status to use when should_exit() is true: failure for an unknown option. */
        [[nodiscard]] st::int32 exit_code() const noexcept;

        /** @brief Prints the usage to standard output. */
        void print_help() const noexcept;
        /** @brief Prints the program name and version to standard output. */
        void print_version() const noexcept;

    private:
        struct argparse_impl;
        std::unique_ptr<argparse_impl> m_impl;
    };

} // namespace st

/** @} */

#endif // STICKY_ARGPARSE_H
