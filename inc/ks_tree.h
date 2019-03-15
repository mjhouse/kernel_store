
#ifndef KS_TREE_H
#define KS_TREE_H

#include<linux/slab.h>

#include "ks_common.h"

// root node of the tree
static node* root = 0;

// free a single node
void ks_free(node* n) {
	kfree(n);
	n = 0;
}

// recursively free memory
void ks_free_tree(node* n) {
	if (n == 0) return;

	ks_free_tree(n->right);
	ks_free_tree(n->left);

	ks_free(n);
}

// make a node
node* ks_make(void) {
	node* pair = (node*)kmalloc(sizeof(struct _node),GFP_KERNEL);

	if (!pair) return 0;

	memset(pair->key,0,10);
	memset(pair->val,0,10);

	return pair;
}

// add a new node
node* ks_add(node* n) {
	node** cur = &root;
	int cmp = 0;

	if (!n) return 0;

	while (*cur != 0) {
		cmp = strcmp((*cur)->key, n->key);

		if (cmp < 0) { // move left
			cur = &((*cur)->left);
		}
		else if (cmp > 0) { // move right
			cur = &((*cur)->right);
		}
		else { // found existing key
			strncpy((*cur)->val, n->val, strlen(n->val));
			return *cur;
		}

	}

	*cur = n;
	return n;
}

// get a value given the key
node* ks_get(char* key) {
	node** cur = &root;
	int cmp = 0;

	while (*cur != 0) {
		cmp = strcmp((*cur)->key, key);

		if (cmp < 0) { // move left
			cur = &((*cur)->left);
		}
		else if (cmp > 0) { // move right
			cur = &((*cur)->right);
		}
		else { // found key
			return *cur;
		}

	}

	return 0;
}

// free the tree
void ks_tree_clr(void) {
	ks_free_tree(root);
}

#endif
