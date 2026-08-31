/*
 * st_init.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 31/08/2026
 */

#ifndef STICKY_ST_INIT_H
#define STICKY_ST_INIT_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include "st_types.h"

namespace st
{

    [[nodiscard]] st_bool lib_init() noexcept;
    [[nodiscard]] st_bool lib_free() noexcept;

}

#endif // STICKY_ST_INIT_H
