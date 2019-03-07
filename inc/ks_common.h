#ifndef KS_COMMON_H
#define KS_COMMON_H

#include <asm/ioctl.h>

// a key/value pair
struct data_pair {
	char* key;
	char* val;
};

// typedef for convenience
typedef struct data_pair node;

#define KS_GET_VALUE _IOR('q', 1, node*)
#define KS_SET_VALUE _IOW('q', 2, node*)

#endif
