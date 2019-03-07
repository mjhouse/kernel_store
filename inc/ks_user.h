#ifndef KS_USER_H
#define KS_USER_H

#include "ks_common.h"

node* make_node(char* key, char* val) {
	node* pair = (node*)malloc(sizeof(node));
   	pair->right = 0;
	pair->left = 0;

	pair->key = (char*)calloc(strlen(key), sizeof(char));
	pair->val = (char*)calloc(strlen(val), sizeof(char));
	strcpy(pair->key, key);
	strcpy(pair->val, val);

	return pair;
}

// /*  Make
//     allocate memory for a new data_pair/node and
//     it's key/value. copy the given values into
//     the new node and return.
// */
// node* ks_make(char* key, char* val) {
// 	node* pair = (node*)kmalloc(sizeof(node),GFP_KERNEL);
//
// 	if (!pair) {
// 		ks_free(pair);
// 		return 0;
// 	}
//
//    	pair->right = 0;
// 	pair->left = 0;
//
// 	// kcalloc(len(key) + 1, sizeof(char), GFP_KERNEL);
// 	pair->key = (char*)calloc(len(key) + 1, sizeof(char));
// 	pair->val = (char*)calloc(len(val) + 1, sizeof(char));
//
// 	if (!(pair->val && pair->val)) {
// 		ks_free(pair);
// 		return 0;
// 	}
//
// 	strncpy(pair->key, key, len(key));
// 	strncpy(pair->val, val, len(val));
//
// 	return pair;
// }
//
// /*  Set
//     search the tree looking for a node with the
//     given key and set the value or create a new
//     node.
// */
// void ks_set(char* key, char* val) {
// 	node **cur = &root;
// 	int cmp = 0;
//
// 	while (*cur != 0) {
// 		cmp = strcmp((*cur)->key, key);
//
// 		if (cmp < 0) {
// 			cur = &((*cur)->left);
// 		}
// 		else if (cmp > 0) {
// 			cur = &((*cur)->right);
// 		}
// 		else {
// 			// found an existing entry
// 			strncpy((*cur)->val, val, len(val));
// 			return;
// 		}
//
// 	}
//
// 	// found the right leaf pointer
// 	*cur = ks_make(key, val);
// }

#endif
