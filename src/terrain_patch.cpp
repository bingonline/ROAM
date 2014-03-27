#include "terrain_patch.hpp"
#include "util.h"

#include "gfx/spline.hpp"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TerrainPatch::TerrainPatch(const char *fn, int offset_x, int offset_y)
	: m_map(NULL)
	, m_worldX(offset_x)
	, m_worldY(offset_y)
	, m_leftVariance(NULL)
	, m_rightVariance(NULL)
	, m_varianceSize(0)
	, m_leftRoot(NULL)
	, m_rightRoot(NULL)
	, m_leftLeaves(0)
	, m_rightLeaves(0)
	, m_triPool(0)
	, m_poolSize(100000)
	, m_poolNext(0)
{
	m_map = Heightmap_read(fn);
	if (m_map == NULL) {
		return;
	}

	Heightmap_normalize(m_map);
	Heightmap_calculate_normals(m_map);
	Heightmap_print(m_map);

	m_triPool = new BTTNode[m_poolSize];
	memset(m_triPool, 0, sizeof(BTTNode)*m_poolSize);

	m_leftRoot = allocateNode();
	m_rightRoot = allocateNode();
}

void TerrainPatch::print() const
{
	printf("TerrainPatch {\n");
	printf("  variance_size: %zu\n", this->m_varianceSize);
	printf("  variance_limit: %f\n", this->m_varianceLimit);
	printf("  left_num_leaves: %zu\n", this->m_leftLeaves);
	printf("  right_num_leaves: %zu\n", this->m_rightLeaves);
	printf("}\n");
}

void TerrainPatch::computeVariance(int maxTessellationLevels)
{
	m_varianceSize = 2<<maxTessellationLevels;

	m_leftVariance  = new float[m_varianceSize];
	m_rightVariance = new float[m_varianceSize];
	memset(m_leftVariance,  0, sizeof(float)*m_varianceSize);
	memset(m_rightVariance, 0, sizeof(float)*m_varianceSize);

	computeVarianceRecursive(
		maxTessellationLevels, 0, m_leftVariance, 1, m_map,
		0,              m_map->height-1, Heightmap_get(m_map, 0, m_map->height-1),
		m_map->width-1, 0,               Heightmap_get(m_map, m_map->width-1, 0),
		0,              0,               Heightmap_get(m_map, 0, 0));
	computeVarianceRecursive(
		maxTessellationLevels, 0, m_rightVariance, 1, m_map,
		m_map->width-1, 0,               Heightmap_get(m_map, m_map->width-1, 0),
		0,              m_map->height-1, Heightmap_get(m_map, 0, m_map->height-1),
		m_map->width-1, m_map->height-1, Heightmap_get(m_map, m_map->width-1, m_map->height-1));
}

void TerrainPatch::reset()
{
	m_leftRoot->left_child = m_leftRoot->right_child = NULL;
	m_rightRoot->left_child = m_rightRoot->right_child = NULL;

	m_leftRoot->right_neighbor = m_leftRoot->left_neighbor = NULL;
	m_rightRoot->right_neighbor = m_rightRoot->left_neighbor = NULL;

	m_leftRoot->base_neighbor = m_rightRoot;
	m_rightRoot->base_neighbor = m_leftRoot;

	m_poolNext = 2;
}

void TerrainPatch::tessellate(const Vec3f &view, float errorMargin)
{
	tessellateRecursive(
		m_leftRoot, view, errorMargin,
		0,              m_map->height-1,
		m_map->width-1, 0,
		0,              0,
		m_leftVariance, 1);
	tessellateRecursive(
		m_rightRoot, view, errorMargin,
		m_map->width-1, 0,
		0,              m_map->height-1,
		m_map->width-1, m_map->height-1,
		m_rightVariance, 1);

	m_leftLeaves = BTTNode_number_of_leaves(m_leftRoot);
	m_rightLeaves = BTTNode_number_of_leaves(m_rightRoot);
}

void TerrainPatch::getTessellation(float *vertices, float *colors, float *normals)
{
	int idx = 0;
	getTessellationRecursive(
		m_leftRoot, m_map, vertices, colors, normals, &idx,
		0,                 m_map->height-1,
		m_map->width-1, 0,
		0,                 0);
	getTessellationRecursive(
		m_rightRoot, m_map, vertices, colors, normals, &idx,
		m_map->width-1, 0,
		0,              m_map->height-1,
		m_map->width-1, m_map->height-1);
}

BTTNode *TerrainPatch::allocateNode()
{
	BTTNode *tri;

	if (m_poolNext >= m_poolSize)
		return NULL;

	tri = &m_triPool[m_poolNext++];
	tri->left_child = tri->right_child = NULL;

	return tri;
}

void TerrainPatch::split(BTTNode *node)
{
	if (node->left_child)
		return;

	if (node->base_neighbor && node->base_neighbor->base_neighbor != node)
		split(node->base_neighbor);

	node->left_child = allocateNode();
	node->right_child = allocateNode();

	if (!node->left_child || !node->right_child) {
		return;
	}

	node->left_child->base_neighbor = node->left_neighbor;
	node->left_child->left_neighbor = node->right_child;

	node->right_child->base_neighbor  = node->right_neighbor;
	node->right_child->right_neighbor = node->left_child;

	// link left neighbor to the new children
	if (node->left_neighbor) {
		if (node->left_neighbor->base_neighbor == node)
			node->left_neighbor->base_neighbor = node->left_child;
		else if (node->left_neighbor->left_neighbor == node)
			node->left_neighbor->left_neighbor = node->left_child;
		else if (node->left_neighbor->right_neighbor == node)
			node->left_neighbor->right_neighbor = node->left_child;
	}

	// link right neighbor to the new children
	if (node->right_neighbor) {
		if (node->right_neighbor->base_neighbor == node)
			node->right_neighbor->base_neighbor = node->right_child;
		else if (node->right_neighbor->right_neighbor == node)
			node->right_neighbor->right_neighbor = node->right_child;
		else if (node->right_neighbor->left_neighbor == node)
			node->right_neighbor->left_neighbor = node->right_child;
	}

	// link base neighbor to the new children
	if (node->base_neighbor) {
		if (node->base_neighbor->left_child) {
			node->base_neighbor->left_child->right_neighbor = node->right_child;
			node->base_neighbor->right_child->left_neighbor = node->left_child;
			node->left_child->right_neighbor = node->base_neighbor->right_child;
			node->right_child->left_neighbor = node->base_neighbor->left_child;
		} else {
			split(node->base_neighbor);
		}
	} else {
		// edge triangle
		node->left_child->right_neighbor = NULL;
		node->right_child->left_neighbor = NULL;
	}
}

void TerrainPatch::tessellateRecursive(
	BTTNode *node, const Vec3f &view, float errorMargin,
	int left_x, int left_y, int right_x, int right_y, int apex_x, int apex_y,
	float *variance_tree, int variance_idx)
{
	float center_x = (left_x + right_x) * 0.5f;
	float center_y = (left_y + right_y) * 0.5f;
	//float variance = 0;

	if (variance_idx < m_varianceSize) {
		float a = center_x/m_map->width - view.x;
		float b = center_y/m_map->height - view.y;
		float distance = sqrtf(a*a + b*b);
		float variance = (variance_tree[variance_idx])/MIN(distance, 1);

		if (variance > errorMargin) {
			split(node);
			if (node->left_child &&
			   ((abs(left_x - right_x) >= 3) || (abs(left_y - right_y) >= 3)))
			{
				tessellateRecursive(
					node->left_child, view, errorMargin,
					apex_x, apex_y, left_x, left_y, center_x, center_y,
					variance_tree, (variance_idx<<1));
				tessellateRecursive(
					node->right_child, view, errorMargin,
					right_x, right_y, apex_x, apex_y, center_x, center_y,
					variance_tree, (variance_idx<<1)+1);
			}
		}
	}
}

void TerrainPatch::computeVarianceRecursive(
	int maxTessellationLevels, int level, float *varianceTree, int idx, Heightmap *map,
	int left_x,  int left_y,  float left_z,
	int right_x, int right_y, float right_z,
	int apex_x,  int apex_y,  float apex_z)
{
	int center_x = (left_x + right_x) / 2;
	int center_y = (left_y + right_y) / 2;
	float center_z = Heightmap_get(map, center_x, center_y);

	if (level < maxTessellationLevels) {
		computeVarianceRecursive(
			maxTessellationLevels, level+1, varianceTree, (idx<<1), map,
			apex_x, apex_y, apex_z,
			left_x, left_y, left_z,
			center_x, center_y, center_z);
		computeVarianceRecursive(
			maxTessellationLevels, level+1, varianceTree, (idx<<1)+1, map,
			right_x, right_y, right_z,
			apex_x, apex_y, apex_z,
			center_x, center_y, center_z);

		varianceTree[idx] = MAX(varianceTree[(idx<<1)],
		                        varianceTree[(idx<<1)+1]);
	} else {
		varianceTree[idx] = fabs(center_z - ((left_z + right_z)*0.5));
	}
}

void TerrainPatch::getTessellationRecursive(
	BTTNode *node, Heightmap *map,
	float *vertices, float *colors, float *normals, int *idx,
	int left_x, int left_y, int right_x, int right_y, int apex_x, int apex_y)
{
	if (node->left_child) {
		int center_x = (left_x + right_x) / 2;
		int center_y = (left_y + right_y) / 2;

		getTessellationRecursive(
			node->left_child, map, vertices, colors, normals, idx,
			apex_x, apex_y, left_x, left_y, center_x, center_y);
		getTessellationRecursive(
			node->right_child, map, vertices, colors, normals, idx,
			right_x, right_y, apex_x, apex_y, center_x, center_y);
	} else {
		// we're at leaf
		vertices[*idx+0] = (float) left_x / map->width;
		vertices[*idx+1] = (float) left_y / map->height;
		vertices[*idx+2] = Heightmap_get(map, left_x, left_y);
		vertices[*idx+3] = (float) right_x / map->width;
		vertices[*idx+4] = (float) right_y / map->height;
		vertices[*idx+5] = Heightmap_get(map, right_x, right_y);
		vertices[*idx+6] = (float) apex_x / map->width;
		vertices[*idx+7] = (float) apex_y / map->height;
		vertices[*idx+8] = Heightmap_get(map, apex_x, apex_y);

		colors[*idx+0] = 1;
		colors[*idx+1] = 1;
		colors[*idx+2] = 1;
		colors[*idx+3] = 1;
		colors[*idx+4] = 1;
		colors[*idx+5] = 1;
		colors[*idx+6] = 1;
		colors[*idx+7] = 1;
		colors[*idx+8] = 1;

		Heightmap_get_normal(
			map, left_x, left_y,
			&normals[*idx+0], &normals[*idx+1], &normals[*idx+2]);
		Heightmap_get_normal(
			map, right_x, right_y,
			&normals[*idx+3], &normals[*idx+4], &normals[*idx+5]);
		Heightmap_get_normal(
			map, apex_x, apex_y,
			&normals[*idx+6], &normals[*idx+7], &normals[*idx+8]);

		*idx += 9;
	}
}
