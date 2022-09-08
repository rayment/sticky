/*
 * model.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_MODEL_H
#define FR_RAYMENT_STICKY_MODEL_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/collections/linkedlist.h"
#include "sticky/common/defines.h"
#include "sticky/common/types.h"
#include "sticky/video/material.h"
#include "sticky/video/mesh.h"
#include "sticky/video/shader.h"

/**
 * @addtogroup model
 * @{
 */

/**
 * @brief Renderable model.
 *
 * The model is a triad containing a mesh, material and a shader, whereby all
 * three are combined in a render call to display an object on the screen. Each
 * model may have zero or more child models, which may each have a position,
 * rotation and scale different to the parent model. Together, these models all
 * form an ancestral hierarchy which are all rendered together in order at
 * runtime.
 *
 * @since 1.0.0
 */
typedef struct
Smodel_s
{
	Smesh *mesh;
	Smaterial *mat;
	Sshader *shader;
	Slinkedlist *children;
	struct Smodel_s *parent;
} Smodel;

/**
 * @brief Create a new model.
 *
 * Creates a new model and links a mesh, material and shader to it on the heap.
 * By default, all models have no children and no parent.
 *
 * If a <c>NULL</c> mesh, material or shader is linked to the model, it will not
 * respond to any draw calls until all three are linked.
 *
 * @param[in] mesh The mesh to link.
 * @param[in] mat The material to link.
 * @param[in] shader The shader to link.
 * @return A new model.
 * @since 1.0.0
 */
STICKY_API Smodel   *S_model_new(Smesh *, Smaterial *, Sshader *);

/**
 * @brief Free a model from memory.
 *
 * Once this function is called for a given model, that model becomes invalid
 * and may not be used again in any other model function.
 *
 * @warning This function does not free any linked meshes, materials, shaders or
 * child models from memory. Child models should be unlinked and/or free'd
 * separately, and linked meshes, materials and shaders should be free'd after
 * the model is unloaded.
 * @param[in,out] model The model to be free'd.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid model is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void      S_model_delete(Smodel *);

/**
 * @brief Free a model from memory along with all child models.
 *
 * Once this function is called for a given model, that model along with all
 * child ancestors become invalid and may not be used again in any other model
 * function.
 *
 * @warning This function does not free any linked meshes, materials, shaders or
 * parent models from memory. They should be free'd after the model tree is
 * unloaded.
 * @param[in,out] model The model tree to be free'd.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid model is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void      S_model_delete_tree(Smodel *);

/**
 * @brief Link a mesh to a model.
 *
 * Sets the mesh of a model. If a <c>NULL</c> mesh is provided, the current
 * mesh, if any, is unlinked from the model, and the model will no longer be
 * drawn.
 *
 * @param[in,out] model The model to modify.
 * @param[in] mesh The mesh to link.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid model is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void      S_model_set_mesh(Smodel *, Smesh *);

/**
 * @brief Link a material to a model.
 *
 * Sets the material of a model. If a <c>NULL</c> material is provided, the
 * current material, if any, is unlinked from the model, and the model will no
 * longer be drawn.
 *
 * @param[in,out] model The model to modify.
 * @param[in] mat The material to link.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid model is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void      S_model_set_material(Smodel *, Smaterial *);

/**
 * @brief Link a shader to a model.
 *
 * Sets the shader of a model. If a <c>NULL</c> shader is provided, the current
 * shader, if any, is unlinked from the model, and the model will no longer be
 * drawn.
 *
 * @param[in,out] model The model to modify.
 * @param[in] shader The shader to link.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid model is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void      S_model_set_shader(Smodel *, Sshader *);

/**
 * @brief Get the linked mesh from a model.
 *
 * Returns the mesh that is currently linked to a given model, if any. If no
 * mesh is linked, <c>NULL</c> is returned.
 *
 * @param[in] model The model that contains the mesh.
 * @return The mesh of a model, or <c>NULL</c> if no mesh is linked.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid model is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Smesh    *S_model_get_mesh(const Smodel *);

/**
 * @brief Get the linked material from a model.
 *
 * Returns the material that is currently linked to a given model, if any. If no
 * mesh is linked, <c>NULL</c> is returned.
 *
 * @param[in] model The model that contains the material.
 * @return The material of a model, or <c>NULL</c> if no material is linked.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid model is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Smaterial *S_model_get_material(const Smodel *);

/**
 * @brief Get the linked shader from a model.
 *
 * Returns the shader that is currently linked to a given model, if any. If no
 * shader is linked, <c>NULL</c> is returned.
 *
 * @param[in] model The model that contains the shader.
 * @return The shader of a model, or <c>NULL</c> if no shader is linked.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid model is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sshader  *S_model_get_shader(const Smodel *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MODEL_H */

