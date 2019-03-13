#ifndef KS_COMMON_H
#define KS_COMMON_H

#include <asm/ioctl.h>

// a key/value pair
struct _node {
	char key[10];
	char val[10];
};

// typedef for convenience
typedef struct _node node;

// root node of the tree
static node* root = 0;

#define KS_GET_VALUE _IOW('q', 1, struct _node*)
#define KS_SET_VALUE _IOW('q', 2, struct _node*)

/*
void ks_free(node* n) {
	// kfree(n); 
	free(n->val);
	free(n->key);
	free(n);

	n = 0;
}

void ks_free_tree(node* n) {
	if (n == 0) return;

	ks_free_tree(n->right);
	ks_free_tree(n->left);

	ks_free(n);
}

node* ks_make(char* key, char* val) {
	node* pair = (node*)malloc(sizeof(node));
	
	if (!pair) return 0;
	
	pair->key = (char*)calloc(len(key) + 1, sizeof(char));
	pair->val = (char*)calloc(len(val) + 1, sizeof(char));
	
	if (!(pair->key && pair->val)) {
		ks_free(pair);
		return 0;
	}
	
	strncpy(pair->key, key, len(key));
	strncpy(pair->val, val, len(val));
	
	pair->right = 0;
	pair->left = 0;
	
	return pair;
}

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
			strncpy((*cur)->val, n->val, len(n->val));
			return *cur;
		}
	
	}
	
	*cur = n;
	return n;
}

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

void ks_del(char* key) {
}

void ks_clr() {
	ks_free_tree(root);
}
*/

#endif
