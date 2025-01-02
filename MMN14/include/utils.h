#ifndef _UTILS_H_ 
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* get tokens count in line */
int get_token_count(const char* sz_line);

/* get token in line by index */
char* get_token(const char* sz_line, int start_index);

/* check if token is in reserved words */
bool check_reserved(const char* name);

/* check if a label is valid */
bool check_label(const char* name);

/* check if an instruction in the assembly line is valid */
bool check_instruction(const char* sz_line,
	int start_index, int token_count);

/* check if a guide statement in the assembly line is valid */
bool check_guide(const char* sz_line,
	int start_index, int token_count);

/* get instruction by name */
Instruction* get_instruction(const char* name);

/* get instruction size in assembly line */
int get_size_of_instruction(const char* sz_line,
	int start_index, int token_count);

/* get type of operand by name */
int get_operand_type(const char* opr);

/* get register number of operand */
int get_register_number(const char* opr);

/* get guide size in assembly line */
int get_size_of_guide(const char* sz_line,
	int start_index, int token_count);

/* save operand to object file */
void save_operand(FILE* fp_ext, FILE* fp_obj, 
	int counter, const char* opr, int order);

/* save instruction to object file */
void save_instruction(const char* sz_line, int start_index,
	int token_count, FILE* fp_ext, FILE* fp_obj);

/* save guide to object file */
void save_guide(const char* sz_line, int start_index,
	int token_count, FILE* fp_obj);


#endif /* _UTILS_H_ */
