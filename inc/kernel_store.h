#ifndef KERNEL_STORE_H
#define KERNEL_STORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <include/linux/slab.h>

#include "utility.h"

// ----------------------------------------------------------------------------
// data

// a key/value pair in the binary tree
struct data_pair {
	char* key;
	char* val;

	struct data_pair* right;
	struct data_pair* left;
};

// typedef for convenience
typedef struct data_pair node;

// root node of the tree
static node* root = 0;

// end data
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// functions

/*  Free
    free the memory for a single node.
*/
void ks_free(node* n) {
	// kfree(n); 
	free(n->val);
	free(n->key);
	free(n);
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

/*  Make
    allocate memory for a new data_pair/node and 
    it's key/value. copy the given values into
    the new node and return.
*/
node* ks_make(char* key, char* val) {
	// (node*)kmalloc(sizeof(sizeof(node)), GFP_KERNEL)
	node* pair = (node*)malloc(sizeof(node));
	
	if (!pair) {
		ks_free(pair);
		return 0;
	}

   	pair->right = 0;
	pair->left = 0;
	
	// kcalloc(len(key) + 1, sizeof(char), GFP_KERNEL);
	pair->key = (char*)calloc(len(key) + 1, sizeof(char));
	pair->val = (char*)calloc(len(val) + 1, sizeof(char));
	
	if (!(pair->val && pair->val)) {
		ks_free(pair);
		return 0;
	}
	
	strncpy(pair->key, key, len(key));
	strncpy(pair->val, val, len(val));
	
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
			strncpy((*cur)->val, n->val, len(n->val));
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

/*  Set
    search the tree looking for a node with the
    given key and set the value or create a new
    node.
*/
void ks_set(char* key, char* val) {
	node **cur = &root;
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
			strncpy((*cur)->val, val, len(val));
			return;
		}

	}

	// found the right leaf pointer
	*cur = ks_make(key, val);
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
void ks_clr() {
	ks_free_tree(root);
}

// end functions
// ----------------------------------------------------------------------------

#endif