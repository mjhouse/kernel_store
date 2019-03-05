#ifndef KERNAL_STORE_UTILITIES_H
#define KERNAL_STORE_UTILITIES_H

int len(char* str) {
	int i;
	for (i = 0; str[i] != '\0'; ++i);

	return i;
}

#endif