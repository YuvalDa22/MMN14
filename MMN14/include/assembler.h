#ifndef _ASSEMBLER_H_
#define _ASSEMBLER_H_

#include <stdio.h>
#include "macros.h"

/* global variables */
extern char g_line[MAX_LINE_LEN]; /* buffer to store a line of input */
extern int g_line_num; /* current line number */
extern int g_pc; /* program counter */
extern int g_code_count; /* count of code lines */
extern int g_data_count; /* count of data lines */
extern int g_err_count; /* count of errors */

/* instruction struct */
typedef struct Instruction {
    const char* name;   /* instruction name */
    int op;             /* instrcution operator code */
    int num_opr;        /* number of operands in instruction */
} Instruction;

/* initialize symbols */
void init_symbols();

/* processes one stage */
int process_first_phase(FILE* fp_as, FILE* fp_obj);

/* procsses two stage */
void process_second_phase(FILE* fp_as, FILE* fp_ent, FILE* fp_ext, FILE* fp_obj);

/* free memory for symbols */
void free_symbols();

#endif /* _ASSEMBLER_H_ */