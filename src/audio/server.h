/*
 * server.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_AUDIO_SERVER_H
#define STICKY_AUDIO_SERVER_H

#include "st_primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

st_bool st_audio_server_init(void);
st_bool st_audio_server_free(void);

#ifdef __cplusplus
}
#endif

#endif // STICKY_AUDIO_SERVER_H
