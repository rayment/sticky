/*
 * server.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_AUDIO_SERVER_H
#define STICKY_AUDIO_SERVER_H

#include "st_types.h"

namespace st::audio::server
{

    st::bool_t init(void);
    st::bool_t free(void);

}

#endif // STICKY_AUDIO_SERVER_H
