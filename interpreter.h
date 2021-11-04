// See LICENSE for copyright/license information

#ifndef TULI__INTERPRETER_H
#define TULI__INTERPRETER_H

#include "common.h"
#include "controller.h"

void tuli_SetMem4B(struct tuli_Controller* ctr, tuli_Int bytes) {
	long long start = tuli_TIToLL(ctr->regL);
	*tuli_GetMem(ctr, start) = (unsigned char)(bytes & 0xff);
	bytes >>= 8;
	*tuli_GetMem(ctr, start+1) = (unsigned char)(bytes & 0xff);
	bytes >>= 8;
	*tuli_GetMem(ctr, start+2) = (unsigned char)(bytes & 0xff);
	bytes >>= 8;
	*tuli_GetMem(ctr, start+3) = (unsigned char)(bytes & 0xff);
}

void tuli_Interpreter(struct tuli_Controller* ctr) {
	while (1) {
		unsigned char* instr = tuli_GetMem(ctr, tuli_TIToLL(ctr->regI));
		switch (*instr) {
			case '<':
				tuli_SetMem4B(ctr, ctr->regT);
				break;
			case '=':
				tuli_SetMem4B(ctr, ctr->regU);
				break;
			case '@':
				*tuli_GetMem(ctr, tuli_TIToLL(ctr->regL)) = 
					(unsigned char)(ctr->regT & 0xff);
				break;
			case 'A':
				*tuli_GetMem(ctr, tuli_TIToLL(ctr->regL)) = 
					(unsigned char)(ctr->regU & 0xff);
				break;
			case 'I':
				ctr->regI = ctr->regT;
				break;
			case 'J':
				ctr->regI = ctr->regU;
				break;
			case 'K':
				ctr->regI = ctr->regL;
				break;
			case 'L':
				ctr->regL = ctr->regT;
				break;
			case 'M':
				ctr->regL = ctr->regU;
				break;
			case 'U':
				ctr->regU = ctr->regT;
				break;
			case '[':
				tuli_SetMem4B(ctr, ctr->regI);
				break;
			case '\\':
			{
				long long start = tuli_TIToLL(ctr->regL);
				ctr->regT = 
					((tuli_Int)*tuli_GetMem(ctr, start)) +
					(((tuli_Int)*tuli_GetMem(ctr, start+1)) << 1) + 
					(((tuli_Int)*tuli_GetMem(ctr, start+2)) << 2) + 
					(((tuli_Int)*tuli_GetMem(ctr, start+3)) << 3);
				break;
			}
			case '_':
				*tuli_GetMem(ctr, tuli_TIToLL(ctr->regL)) = 
					(unsigned char)(ctr->regI & 0xff);
				break;
			case '`':
				ctr->regT = (tuli_Int)*tuli_GetMem(ctr, tuli_TIToLL(ctr->regL));
				break;
			case 'c':
				++ctr->regT;
				break;
			case 'f':
			{
				tuli_Int tmp = ctr->regL;
				tmp = (((tmp & 0xaaaaaaaa) >> 1) | ((tmp & 0x55555555) << 1));
				tmp = (((tmp & 0xcccccccc) >> 2) | ((tmp & 0x33333333) << 2));
				tmp = (((tmp & 0xf0f0f0f0) >> 4) | ((tmp & 0x0f0f0f0f) << 4));
				tmp = (((tmp & 0xff00ff00) >> 8) | ((tmp & 0x00ff00ff) << 8));
				ctr->regT = ~(ctr->regT | ((tmp >> 16) | (tmp << 16)));
				break;
			}
			case 'i':
				ctr->regT = ctr->regI;
				break;
			case 'j':
				ctr->regU = ctr->regI;
				break;
			case 'k':
				ctr->regL = ctr->regI;
				break;
			case 'l':
				ctr->regT = ctr->regL;
				break;
			case 'm':
				ctr->regU = ctr->regL;
				break;
			case 'n':
			{
				unsigned char c = getchar();
				ctr->regT = tuli_LLToTI(c);
				break;
			}
			case 'o':
				putchar( tuli_TIToLL(ctr->regT) & 0x1ff );
				break;
			case 'r':
				ctr->regT = (ctr->regT >> 1) | (ctr->regT << 31);
				break;
			case 's':
			{
				tuli_Int tmp = ctr->regT;
				ctr->regT = ctr->regL;
				ctr->regL = tmp;
				break;
			}
			case 't':
				return;
			case 'u':
				ctr->regT = ctr->regU;
				break;
			case 'v':
			{
				if (!ctr->regT) {
					break;
				}
				tuli_Int tmp = ctr->regT;
				unsigned char shift = 0;
				while (!(tmp & 0x80000000)) {
					tmp <<= 1;
					++shift;
				}
				ctr->regT = ~tmp >> shift;
				break;
			}
		}
		ctr->regI = tuli_TIInc(ctr->regI);
	}
}

#endif
