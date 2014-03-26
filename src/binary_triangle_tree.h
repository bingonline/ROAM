#ifndef BINARY_TRIANGLE_TREE_H
#define BINARY_TRIANGLE_TREE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *           Apex Vertex
 *   *************+*************
 *   *           ***           *
 *   * Left     * * *  Right   *
 *   * Neighbor*  *  * Neihbor *
 *   *        *   *   *        *
 *   *       *    *    *       *
 *   *      *     *     *      *
 *   *     *      *      *     *
 *   *    *       *       *    *
 *   *   * Left   * Right  *   *
 *   *  *  Child  * Child   *  *
 *   * *          *          * *
 *   **           *           **
 *   +************+************+
 * Left        Center        Right
 * Vertex      Vertex        Vertex
 *      *                   *
 *       *                 *
 *        *   Base        *
 *         *  Neigbor    *
 *          *           *
 *           *         *
 *            *       *
 *             *     *
 *              *   *
 *               * *
 *                *
 */
typedef struct BTTNodeT
{
	struct BTTNodeT *left_child;
	struct BTTNodeT *right_child;

	struct BTTNodeT *base_neighbor;
	struct BTTNodeT *left_neighbor;
	struct BTTNodeT *right_neighbor;

} BTTNode;

/**
 * Calculate the number of leaves on given tree
 *
 * @param BTT
 *
 * @return number of leaves
 */
size_t BTTNode_number_of_leaves(BTTNode *tree);

#ifdef __cplusplus
} // extern "C"
#endif


#endif // BINARY_TRIANGLE_TREE_H
