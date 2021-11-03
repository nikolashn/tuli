// See LICENSE for copyright/license information

#ifndef TULI__COMMON_H
#define TULI__COMMON_H

#include "stdio.h"
#include "stdlib.h"
#include "sys/stat.h"
#include "string.h"

#define TULI_BLOCK_SIZE 0x1000

typedef unsigned int tuli_Int;

long long tuli_TIToLL(tuli_Int ti) {
	return (long long)(ti ^ 0xaaaaaaaa) - 0xaaaaaaaa;
}

tuli_Int tuli_LLToTI(long long ll) {
	return (tuli_Int)((ll + 0xaaaaaaaa) ^ 0xaaaaaaaa);
}

tuli_Int tuli_TIInc(tuli_Int ti) {
	return ((ti ^ 0xaaaaaaaa) + 1) ^ 0xaaaaaaaa;
}

void tuli_ErrOutOfMem() {
	fprintf(stderr, "Error: Out of memory\n");
	exit(-1);
}

#endif
