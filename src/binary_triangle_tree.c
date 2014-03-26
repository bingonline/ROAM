#include "binary_triangle_tree.h"

size_t BTTNode_number_of_leaves(BTTNode *tree)
{
	if (tree == NULL)
		return 0;

	if (tree->left_child == NULL && tree->right_child == NULL) {
		return 1;
	} else {
		return BTTNode_number_of_leaves(tree->left_child) +
		       BTTNode_number_of_leaves(tree->right_child);
	}
}
