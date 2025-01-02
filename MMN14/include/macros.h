#pragma once

/* Include external constants */
#include "defines.h"

/* macro structure */
typedef struct Macro {
    char name[MAX_LINE_LEN];
    char** instructions; 
    int size;
    int limit; 
} Macro;

/* macro array structure */
typedef struct Macros {
    Macro** macros; 
    int size; 
    int limit; 
} Macros;


/* init macro */
void init_macro(Macro* macro, const char* name);

/* init macro array */
void init_macro_array(Macros* marray);

/* add instruction to macro */
void add_instruction(Macro* macro, const char* line);

/* add macro to macro array */
Macro* add_macro(Macros* marray, const char* name);

/* lookup macro */
Macro* get_macro(Macros* marray, const char* name);

/* free macro */
void free_macro(Macro* macro);

/* free macro array */
void free_macro_array(Macros* marray);

