// See LICENSE for copyright/license information

#include "common.h"
#include "controller.h"
#include "interpreter.h"

char usageStr[] =
	"Usage: tuli <file>\n"
;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Error: Not enough arguments\n%s", usageStr);
		return 0;
	}

	FILE* codeFile = fopen(argv[1], "rb");
	if (!codeFile) {
		printf("Error: Cannot find file %s\n", argv[1]);
		return -1;
	}
	
	struct tuli_Controller* ctr = calloc(1, sizeof(*ctr));
	if (!ctr) {
		tuli_ErrOutOfMem();
	}
	ctr->blockCnt = 0x100;
	ctr->blocks = calloc(ctr->blockCnt, sizeof(*ctr->blocks));
	if (!ctr->blocks) {
		tuli_ErrOutOfMem();
	}

	struct stat st;
	stat(argv[1], &st);
	long long codeBlockSize = 
		(!!(st.st_size & (TULI_BLOCK_SIZE-1))) * TULI_BLOCK_SIZE +
			(st.st_size & ~(TULI_BLOCK_SIZE-1));

	ctr->blocks[0].mem = malloc(codeBlockSize);
	if (!ctr->blocks[0].mem) {
		tuli_ErrOutOfMem();
	}
	ctr->blocks[0].virtStart = 0;
	ctr->blocks[0].virtEnd = codeBlockSize;

	fread(ctr->blocks[0].mem, 1, st.st_size, codeFile);
	
	fclose(codeFile);
	tuli_Interpreter(ctr);

	for (int i = 0; i < ctr->blockCnt; i++) {
		free(ctr->blocks[i].mem);
	}
	free(ctr->blocks);
	free(ctr);

	return 0;
}

