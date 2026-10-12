/*
 * app_bindings.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include "lua/app_bindings.h"

namespace
{
    st::bool_t requested = false;
    st::int32  code_requested = 0;
} // namespace

void
st::lua::app::quit(st::int32 code) noexcept
{
    if (requested)
        return;
    requested = true;
    code_requested = code;
}

st::bool_t
st::lua::app::quit_requested() noexcept
{
    return requested;
}

st::int32
st::lua::app::exit_code() noexcept
{
    return code_requested;
}
