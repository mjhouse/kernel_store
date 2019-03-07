#ifndef KS_MODULE_H
#define KS_MODULE_H

#include "ks_common.h"

// root node of the tree
static node* root = 0;

/*  Free
    free the memory for a single node.
*/
void ks_free(node* n) {
	kfree(n->val);
	kfree(n->key);
	kfree(n);
}

/*  Free_Tree
    recursively free nodes from the tree,
    moving up to the given node.
*/
void ks_free_tree(node* n) {
	if (n == 0) return;
	ks_free_tree(n->right);
	ks_free_tree(n->left);
	ks_free(n);
}

node* ks_make(void) {
	node* pair = (node*)kmalloc(sizeof(node),GFP_KERNEL);
   	pair->right = 0;
	pair->left = 0;
	pair->key = 0;
	pair->val = 0;
	return pair;
}

/*  Add
    move down the tree until a duplicate entry or
    a null leaf position is found, and then copy values
    or create a new node.
*/
node* ks_add(node* n) {
	node **cur = &root;
	int cmp = 0;

	if (!n) return 0;

	ks_add(n->right);
	ks_add(n->left);

	n->right = 0;
	n->left = 0;

	while (*cur != 0) {
		cmp = strcmp((*cur)->key, n->key);

		if (cmp < 0) {
			cur = &((*cur)->left);
		}
		else if (cmp > 0) {
			cur = &((*cur)->right);
		}
		else {
			// found an existing entry
			strncpy((*cur)->val, n->val, strlen(n->val));
			return *cur;
		}

	}

	// found the right leaf pointer
	*cur = n;
	return n;
}

/*  Get
    search the tree looking for a node with the
    given key and return the value. return NULL
    on failure.
*/
char* ks_get(char* key) {
	node *cur = root;
	int cmp = 0;

	while (cur != 0) {

		cmp = strcmp(cur->key, key);

		if (cmp < 0) {
			cur = cur->left;
		}
		else if (cmp > 0) {
			cur = cur->right;
		}
		else {
			break;
		}

	}

	return cur ? cur->val : 0;
}

/*  Delete
    search for a node with the given key, unlink
    it from the tree, shuffle it's children back
    onto the tree, and free it.
*/
void ks_del(char* key) {
	node** cur = &root;
	node* n = 0;
	int cmp = 0;

	while (*cur != 0) {
		cmp = strcmp((*cur)->key, key);

		if (cmp < 0) {
			cur = &((*cur)->left);
		}
		else if (cmp > 0) {
			cur = &((*cur)->right);
		}
		else {
			// found an existing entry
			n = *cur;
			*cur = 0;

			ks_add(n->right);
			ks_add(n->left);

			n->right = 0;
			n->left = 0;

			ks_free(n);

			return;
		}

	}
}

/*  Clear
    recursively delete all nodes and the root.
*/
void ks_clr(void) {
	ks_free_tree(root);
}

// end functions
// ----------------------------------------------------------------------------


#endif
