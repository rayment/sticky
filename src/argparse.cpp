/*
 * argparse.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 31/08/2026
 */

#include <cstdlib>
#include <getopt.h>
#include <iostream>

#include <fmt/format.h>

#include "argparse.h"
#include "st_macros.h"

struct st::argparse::argparse_impl
{
    st::int32    argc = 0;
    st::char_t **argv = nullptr;
    st::size     num_non_args = 0;
    st::char_t **non_args = nullptr;
    st::bool_t   should_exit = false;
    st::bool_t   safe_mode = false;
    st::bool_t   developer_mode = false;
    st::int32    exit_code = EXIT_SUCCESS;
};

st::argparse::argparse(st::int32    argc,
                       st::char_t **argv) noexcept :
    m_impl(std::make_unique<argparse_impl>())
{
    m_impl->argc = argc;
    m_impl->argv = argv;

    option long_options[] = {
        {"developer", no_argument, nullptr, 'd'},
        {"help", no_argument, nullptr, 'h'},
        {"safe-mode", no_argument, nullptr, 's'},
        {"version", no_argument, nullptr, 'v'},
        {nullptr, 0, nullptr, 0},
    };

    while (true)
    {
        int c = getopt_long(argc, argv, "dhsv", long_options, nullptr);
        if (c == -1)
            break;
        switch (c)
        {
        case 'h':
            print_help();
            m_impl->should_exit = true;
            break;
        case 'd':
            m_impl->developer_mode = true;
            break;
        case 's':
            m_impl->safe_mode = true;
            break;
        case 'v':
            print_version();
            m_impl->should_exit = true;
            break;
        default:
            print_help();
            m_impl->should_exit = true;
            m_impl->exit_code = EXIT_FAILURE;
            break;
        }
    }

    m_impl->num_non_args = optind < argc ? static_cast<st::size>(argc - optind) : 0;
    m_impl->non_args = argv + optind;
}

st::argparse::~argparse() noexcept = default;

void
st::argparse::print_help() const noexcept
{
    std::cout << fmt::format("Usage: {} [options]", m_impl->argv[0]) << '\n';
    std::cout << "Options:" << '\n';
    std::cout << "  -d, --developer          Enable developer mode." << '\n';
    std::cout << "  -h, --help               Display this help message." << '\n';
    std::cout << "  -s, --safe-mode          Start without loading any mods." << '\n';
    std::cout << "  -v, --version            Display version information." << '\n';
}

void
st::argparse::print_version() const noexcept
{
    std::cout << fmt::format("{} {}", PROGRAM_TITLE, VERSION_STRING) << '\n';
}

st::size
st::argparse::num_non_args() const noexcept
{
    return m_impl->num_non_args;
}

st::char_t **
st::argparse::non_args() const noexcept
{
    return m_impl->non_args;
}

st::bool_t
st::argparse::should_exit() const noexcept
{
    return m_impl->should_exit;
}

st::bool_t
st::argparse::developer_mode() const noexcept
{
    return m_impl->developer_mode;
}

st::bool_t
st::argparse::safe_mode() const noexcept
{
    return m_impl->safe_mode;
}

st::int32
st::argparse::exit_code() const noexcept
{
    return m_impl->exit_code;
}
