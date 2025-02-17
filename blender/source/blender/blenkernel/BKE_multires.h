/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software  Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2007 by Nicholas Bishop
 * All rights reserved.
 */

#ifndef __BKE_MULTIRES_H__
#define __BKE_MULTIRES_H__

/** \file
 * \ingroup bke
 */

#include "BLI_compiler_compat.h"

#include "../vr/vr_build.h"
#if WITH_VR
#ifdef __cplusplus
extern "C"
{
#endif
#endif

enum MultiresModifiedFlags;

struct Depsgraph;
struct DerivedMesh;
struct MDisps;
struct Mesh;
struct ModifierData;
struct Multires;
struct MultiresModifierData;
struct Object;
struct Scene;
struct SubdivCCG;

struct MLoop;
struct MLoopTri;
struct MPoly;
struct MVert;

/* Delete mesh mdisps and grid paint masks */
void multires_customdata_delete(struct Mesh *me);

void multires_set_tot_level(struct Object *ob, struct MultiresModifierData *mmd, int lvl);

void multires_mark_as_modified(struct Depsgraph *depsgraph,
                               struct Object *object,
                               enum MultiresModifiedFlags flags);

void multires_flush_sculpt_updates(struct Object *ob);
void multires_force_sculpt_rebuild(struct Object *ob);
void multires_force_external_reload(struct Object *ob);

/* internal, only called in subsurf_ccg.c */
void multires_modifier_update_mdisps(struct DerivedMesh *dm, struct Scene *scene);
void multires_modifier_update_hidden(struct DerivedMesh *dm);

void multiresModifier_set_levels_from_disps(struct MultiresModifierData *mmd, struct Object *ob);

typedef enum {
  MULTIRES_USE_LOCAL_MMD = 1,
  MULTIRES_USE_RENDER_PARAMS = 2,
  MULTIRES_ALLOC_PAINT_MASK = 4,
  MULTIRES_IGNORE_SIMPLIFY = 8,
} MultiresFlags;

struct DerivedMesh *multires_make_derived_from_derived(struct DerivedMesh *dm,
                                                       struct MultiresModifierData *mmd,
                                                       struct Scene *scene,
                                                       struct Object *ob,
                                                       MultiresFlags flags);

struct MultiresModifierData *find_multires_modifier_before(struct Scene *scene,
                                                           struct ModifierData *lastmd);
struct MultiresModifierData *get_multires_modifier(struct Scene *scene,
                                                   struct Object *ob,
                                                   bool use_first);
int multires_get_level(const struct Scene *scene,
                       const struct Object *ob,
                       const struct MultiresModifierData *mmd,
                       bool render,
                       bool ignore_simplify);
struct Mesh *BKE_multires_create_mesh(struct Depsgraph *depsgraph,
                                      struct Scene *scene,
                                      struct MultiresModifierData *mmd,
                                      struct Object *ob);
void multiresModifier_del_levels(struct MultiresModifierData *mmd,
                                 struct Scene *scene,
                                 struct Object *object,
                                 int direction);
void multiresModifier_base_apply(struct MultiresModifierData *mmd,
                                 struct Scene *scene,
                                 struct Object *ob);
void multiresModifier_subdivide(struct MultiresModifierData *mmd,
                                struct Scene *scene,
                                struct Object *ob,
                                int updateblock,
                                int simple);
void multiresModifier_sync_levels_ex(struct Scene *scene,
                                     struct Object *ob_dst,
                                     struct MultiresModifierData *mmd_src,
                                     struct MultiresModifierData *mmd_dst);

void multires_stitch_grids(struct Object *);

/* Related to the old multires */
void multires_free(struct Multires *mr);
void multires_load_old(struct Object *ob, struct Mesh *me);
void multires_load_old_250(struct Mesh *);

void multiresModifier_scale_disp(struct Depsgraph *depsgraph,
                                 struct Scene *scene,
                                 struct Object *ob);
void multiresModifier_prepare_join(struct Depsgraph *depsgraph,
                                   struct Scene *scene,
                                   struct Object *ob,
                                   struct Object *to_ob);

int multires_mdisp_corners(struct MDisps *s);

/* update multires data after topology changing */
void multires_topology_changed(struct Mesh *me);

/**** interpolation stuff ****/
void old_mdisps_bilinear(float out[3], float (*disps)[3], const int st, float u, float v);
int mdisp_rot_face_to_crn(struct MVert *mvert,
                          struct MPoly *mpoly,
                          struct MLoop *mloops,
                          const struct MLoopTri *lt,
                          const int face_side,
                          const float u,
                          const float v,
                          float *x,
                          float *y);

/* Reshaping, define in multires_reshape.c */

bool multiresModifier_reshapeFromObject(struct Depsgraph *depsgraph,
                                        struct MultiresModifierData *mmd,
                                        struct Object *dst,
                                        struct Object *src);
bool multiresModifier_reshapeFromDeformModifier(struct Depsgraph *depsgraph,
                                                struct MultiresModifierData *mmd,
                                                struct Object *ob,
                                                struct ModifierData *md);
bool multiresModifier_reshapeFromCCG(const int tot_level,
                                     struct Mesh *coarse_mesh,
                                     struct SubdivCCG *subdiv_ccg);

/* Subdivision integration, defined in multires_subdiv.c */

struct SubdivSettings;
struct SubdivToMeshSettings;

void BKE_multires_subdiv_settings_init(struct SubdivSettings *settings,
                                       const struct MultiresModifierData *mmd);

/* TODO(sergey): Replace this set of boolean flags with bitmask. */
void BKE_multires_subdiv_mesh_settings_init(struct SubdivToMeshSettings *mesh_settings,
                                            const struct Scene *scene,
                                            const struct Object *object,
                                            const struct MultiresModifierData *mmd,
                                            const bool use_render_params,
                                            const bool ignore_simplify);

/* General helpers. */

/* For a given partial derivatives of a ptex face get tangent matrix for
 * displacement.
 *
 * Corner needs to be known to properly "rotate" partial derivatives when the
 * matrix is being constructed for quad. For non-quad the corner is to be set
 * to 0. */
BLI_INLINE void BKE_multires_construct_tangent_matrix(float tangent_matrix[3][3],
                                                      const float dPdu[3],
                                                      const float dPdv[3],
                                                      const int corner);

int BKE_multires_sculpt_level_get(const struct MultiresModifierData *mmd);

#include "intern/multires_inline.h"

#if WITH_VR
#ifdef __cplusplus
}
#endif
#endif

#endif /* __BKE_MULTIRES_H__ */
