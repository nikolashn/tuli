// See LICENSE for copyright/license information

#ifndef TULI_CONTROLLER_H
#define TULI_CONTROLLER_H

#include "common.h"

// Memory is allocated in blocks
// Size of block: a multiple of TULI_BLOCK_SIZE
// Memory must also be aligned to TULI_BLOCK_SIZE
struct tuli_MemBlock {
	unsigned char* mem;
	long long virtStart;
	long long virtEnd;
};

struct tuli_Controller {
	tuli_Int regT;
	tuli_Int regU;
	tuli_Int regL;
	tuli_Int regI;
	struct tuli_MemBlock* blocks;
	long long blockCnt;
};

unsigned char* tuli_GetMem(struct tuli_Controller* ctr, long long virtAddr) {
	struct tuli_MemBlock* block = ctr->blocks;
	struct tuli_MemBlock* newBlock = 0;

	while (block < ctr->blocks + ctr->blockCnt) {
		if (!block->mem) {
			// First empty block
			if (!newBlock) {
				newBlock = block;
			}
		}
		else if (block->virtStart <= virtAddr && virtAddr < block->virtEnd) {
			return block->mem + (virtAddr - block->virtStart);
		}

		++block;
	}

	if (!newBlock) {
		ctr->blocks = realloc(ctr->blocks, 
			2 * ctr->blockCnt * sizeof(*ctr->blocks));
		if (!ctr->blocks) {
			tuli_ErrOutOfMem();
		}
		newBlock = ctr->blocks + ctr->blockCnt;
		memset(newBlock, 0, ctr->blockCnt * sizeof(*ctr->blocks));
		ctr->blockCnt <<= 1;
	}

	newBlock->mem = malloc(TULI_BLOCK_SIZE);
	if (!newBlock->mem) {
		tuli_ErrOutOfMem();
	}
	newBlock->virtStart = virtAddr & ~(TULI_BLOCK_SIZE-1);
	newBlock->virtEnd = newBlock->virtStart + TULI_BLOCK_SIZE;
	return newBlock->mem + (virtAddr - block->virtStart);
}

#endif
