#ifndef TERRAIN_PATCH_HPP
#define TERRAIN_PATCH_HPP

#include "heightmap.h"
#include "binary_triangle_tree.h"

#include "math/vec3.hpp"

class TerrainPatch
{
private:
	Heightmap *m_map;

	size_t m_worldX, m_worldY;

	// left and right 'variance' trees
	float *m_leftVariance;
	float *m_rightVariance;
	size_t m_varianceSize;

	// amount of error allowed
	float m_varianceLimit; 

	// root nodes for left and right BTTs
	BTTNode *m_leftRoot;
	BTTNode *m_rightRoot;

	// number of leaves on left and right BTTs
	size_t m_leftLeaves;
	size_t m_rightLeaves;

	// BinaryTriangleTree node pool
	BTTNode *m_triPool;
	size_t m_poolSize;
	size_t m_poolNext;

public:
	/**
	 * Initialise terrain patch.
	 *
	 * @param filename to read the map from
	 * @param x offset on world
	 * @param y offset on world
	 */
	TerrainPatch(const char *fn, int offset_x = 0, int offset_y = 0);

	/**
	 * Debug print the terrain patch data
	 */
	void print() const;

	/**
	 * Compute variance trees for the given patch.
	 *
	 * This is called once for patches, and after every modification on heightmap.
	 *
	 * @param tessellation max levels
	 */
	void computeVariance(int maxTessellationLevels = 14);

	/**
	 * Resets the tessellation for the next frame.
	 */
	void reset();

	/**
	 * Tessellate/triangulate the terrain patch 
	 *
	 * @param viewer position
	 * @param allowed error margin
	 */
	void tessellate(const Vec3f &view, float errorMargin = 0.025);

	/**
	 * Get the tesselation result into vertices array
	 *
	 * size of the given params should be at least
	 * (left_num_leaves + right_num_leaves)*(number of elements per triangle)
	 * as no bounds are tested
	 *
	 * @param vertices
	 * @param colors
	 * @param normals
	 */
	void getTessellation(float *vertices, float *colors, float *normals);

	size_t amountOfLeaves() const;

private:
	// PRIVATE FUNCTIONS

	/**
	 * Allocate a BTT node from the triangle pool.
	 *
	 */
	BTTNode *allocateNode();

	/**
	 * Split the Given binary triangle node with following rules:
	 * @ http://www.gamasutra.com/view/feature/131596/realtime_dynamic_level_of_detail_.php?print=1
	 *
	 *  1. The Node is part of a Diamond - Split the node and its Base Neighbor.
	 *  2. The Node is on the edge of the mesh - Trivial, only split the node.
	 *  3. The Node is not part of a Diamond - Force Split the Base Neighbor.
	 */
	void split(BTTNode *node);

	void tessellateRecursive(
		BTTNode *node, const Vec3f &view, float errorMargin,
		int left_x, int left_y, int right_x, int right_y, int apex_x, int apex_y,
		float *variance, int variance_idx);

	void computeVarianceRecursive(
		int maxTessellationLevels, int level, float *varianceTree, int idx, Heightmap *map,
		int left_x,  int left_y,  float left_z,
		int right_x, int right_y, float right_z,
		int apex_x,  int apex_y,  float apex_z);

	void getTessellationRecursive(
		BTTNode *node, Heightmap *map,
		float *vertices, float *colors, float *normals, int *idx,
		int left_x, int left_y, int right_x, int right_y, int apex_x, int apex_y);

};

inline size_t TerrainPatch::amountOfLeaves() const
{
	return m_leftLeaves + m_rightLeaves;
}

#endif // TERRAIN_PATCH_H
