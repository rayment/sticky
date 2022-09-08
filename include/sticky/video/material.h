/*
 * material.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_MATERIAL_H
#define FR_RAYMENT_STICKY_MATERIAL_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/types.h"
#include "sticky/video/texture.h"

/**
 * @addtogroup material
 * @{
 */

/**
 * @brief Maximum number of textures assignable to a given material.
 *
 * Materials have a maximum number of assignable textures and thus only so many
 * may be used at once for a given model.
 *
 * @since 1.0.0
 */
#define S_MATERIAL_LIMIT 8

/**
 * @brief Render material.
 *
 * The material is a group of textures that together are assigned to a model.
 * When the model is drawn, the textures may be mapped to different parts of
 * a single mesh which will result in a complete image.
 *
 * @warning There is a limit to the number of textures that a given material may
 * use at once. Please see {@link S_MATERIAL_LIMIT} for more information.
 * @since 1.0.0
 */
typedef struct
Smaterial_s
{
	Suint8 assigned;
	const Stexture *tex[S_MATERIAL_LIMIT];
} Smaterial;

/**
 * @brief Create a new material.
 *
 * Creates a new blank material on the heap with no assigned textures.
 *
 * @return A new material.
 * @since 1.0.0
 */
STICKY_API Smaterial *S_material_new(void);

/**
 * @brief Free a material from memory.
 *
 * Once this function is called for a given material, that material becomes
 * invalid and may not be used again in any other material function.
 *
 * @warning This function does not free any linked textures. They should be
 * free'd separately after the material is free'd.
 * @param[in,out] mat The material to be free'd.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid material is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void       S_material_delete(Smaterial *);

/**
 * @brief Assign a texture to a given material index.
 *
 * Takes a texture and assigns it to a specific position in a material. The
 * possible indices are from @f$0@f$ to {@link S_MATERIAL_LIMIT}@f$-1@f$.
 *
 * If a <c>NULL</c> texture is given, any assigned texture is unlinked from the
 * material in the given index.
 *
 * @param[in,out] mat The material to store the texture in.
 * @param[in] idx The index of the texture within the material.
 * @param[in] tex The texture to store.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid material or texture is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void       S_material_set_texture(Smaterial *,
                                             Suint8, const Stexture *);

void _S_material_attach(Smaterial *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MATERIAL_H */

