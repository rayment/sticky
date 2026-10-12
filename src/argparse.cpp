/*
 * argparse.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 31/08/2026
 */

#include <getopt.h>
#include <iostream>

#include <fmt/format.h>

#include "argparse.h"
#include "st_macros.h"

struct st::argparse::argparse_impl
{
    st_int32   argc;
    st_char  **argv;
    st_size    num_non_args;
    st_char  **non_args;
    st_bool    should_exit;
    st_int32   exit_code;
};

st::argparse::argparse(st_int32 argc,
                       st_char **argv) noexcept
    : m_impl(std::make_unique<argparse_impl>())
{

    m_impl->exit_code = EXIT_SUCCESS;
    m_impl->argc = argc;
    m_impl->argv = argv;

    option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"version", no_argument, nullptr, 'v'},
    };

    while (true)
    {
        int c = getopt_long(argc, argv, "hv", long_options, nullptr);
        if (c == -1)
            break;
        switch (c)
        {
        default:
        case 0:
        case 'h':
            print_help();
            m_impl->should_exit = true;
            break;
        case 'v':
            print_version();
            m_impl->should_exit = true;
            break;
        }
    }

    ST_ASSERT(optind <= argc);
    m_impl->num_non_args = static_cast<st_size>(argc - optind);
    m_impl->non_args = argv + optind;
}

st::argparse::~argparse() noexcept = default;

void
st::argparse::print_help() const noexcept
{
    std::cout << fmt::format("Usage: {} [options]", m_impl->argv[0]) << '\n';
    std::cout << "Options:" << '\n';
    ;
    std::cout << "  -h, --help               Display this help message." << '\n';
    std::cout << "  -v, --version            Display version information." << '\n';
}

void
st::argparse::print_version() const noexcept
{
    std::cout << fmt::format("{} {}", PROGRAM_TITLE, VERSION_STRING) << '\n';
}

st_size
st::argparse::num_non_args() const noexcept
{
    return m_impl->num_non_args;
}

st_char **
st::argparse::non_args() const noexcept
{
    return m_impl->non_args;
}

st_bool
st::argparse::should_exit() const noexcept
{
    return m_impl->should_exit;
}

st_int32
st::argparse::exit_code() const noexcept
{
    return m_impl->exit_code;
}
