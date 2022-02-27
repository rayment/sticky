/*
 * frustum.c
 * Culling frustum math source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/02/2022
 */

#include "sticky/common/types.h"
#include "sticky/math/frustum.h"
#include "sticky/math/vec3.h"

Sfrustum *S_frustum_new(void);
void      S_frustum_delete(Sfrustum *);

Sbool     S_frustum_intersects_point(const Sfrustum *, const Svec3 *);
Sbool     S_frustum_intersects_bounds(const Sfrustum *,
                                      const Svec3 *, const Svec3 *);

