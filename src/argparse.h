/*
 * argparse.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 31/08/2026
 */

#ifndef STICKY_ARGPARSE_H
#define STICKY_ARGPARSE_H
#include "st_types.h"

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <memory>

#include "st_types.h"

namespace st
{

    class argparse
    {
    public:
        explicit argparse(st_int32 argc, st_char **argv) noexcept;
        ~argparse() noexcept;

        [[nodiscard]] st_size   num_non_args() const noexcept;
        [[nodiscard]] st_char **non_args    () const noexcept;
        [[nodiscard]] st_bool   should_exit () const noexcept;
        [[nodiscard]] st_int32  exit_code   () const noexcept;

        void print_help   () const noexcept;
        void print_version() const noexcept;

    private:
        struct argparse_impl;
        std::unique_ptr<argparse_impl> m_impl;
    };

} // namespace st

#endif // STICKY_ARGPARSE_H
