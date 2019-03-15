#ifndef KS_COMMON_H
#define KS_COMMON_H

#include <asm/ioctl.h>

// a key/value pair
struct _node {
	char key[10];
	char val[10];

	struct _node* left;
	struct _node* right;
};

// typedef for convenience
typedef struct _node node;

#define KS_GET_VALUE _IOW('q', 1, struct _node*)
#define KS_SET_VALUE _IOW('q', 2, struct _node*)

#endif
