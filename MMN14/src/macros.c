#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "macros.h"


/* init macro */
void init_macro(Macro* macro, const char* name) {
    strcpy(macro->name, name);
    macro->instructions = NULL;
    macro->size = 0;
    macro->limit = 0;
}

/* free macro */
void free_macro(Macro* macro) {
    int _idx;
    if (macro == NULL)
        return;
    for (_idx = 0; _idx < macro->size; _idx++) {
        if (macro->instructions[_idx] == NULL)
            continue;
        free(macro->instructions[_idx]);
    }
    free(macro->instructions);
}


/* add instruction to macro */
void add_instruction(Macro* macro, const char* line) {
    char* _temp;
    char** _temp_inst;
    if (macro->size == macro->limit) {
        macro->limit += BLOCK_LEN;
        _temp_inst = realloc(macro->instructions, sizeof(*macro->instructions) * macro->limit);
        if (_temp_inst != NULL) {
            macro->instructions = _temp_inst;
        }
        else {
            printf("Memory allocation fail.\n");
            exit(-1);
        }
    }
    
    _temp = malloc(strlen(line) + 1);
    if(_temp != NULL)
        strcpy(_temp, line);
    else {
        printf("Memory allocation fail.\n");
        exit(-1);
    }
    macro->instructions[macro->size] = _temp;
    macro->size++;
}

/* init macro array */
void init_macro_array(Macros* marray) {
    marray->macros = NULL;
    marray->size = 0;
    marray->limit = 0;
}

/* free macro array */
void free_macro_array(Macros* macros) {
    int _idx;
    if (macros == NULL)
        return;
    for (_idx = 0; _idx < macros->size; _idx++) {
        if (macros->macros[_idx] == NULL)
            continue;
        free_macro(macros->macros[_idx]);
        free(macros->macros[_idx]);
    }
    free(macros->macros);
}

/* get macro */
Macro* get_macro(Macros* macros, const char* name) {
    int _idx;
    Macro* _macro = NULL;

    for (_idx = 0; _idx < macros->size; _idx++) {
        if (strcmp(macros->macros[_idx]->name, name) == 0) {
            _macro = macros->macros[_idx];
            break;
        }
    }
    return _macro;
}

/* add macro to macro array */
Macro* add_macro(Macros* macros, const char* name) {
    int _idx;
    Macro* _macro;
    Macro** _temp_macros;

    for (_idx = 0; _idx < macros->size; _idx++) {
        if (strcmp(macros->macros[_idx]->name, name) == 0) {
            return NULL;
        }
    }

    if (macros->size == macros->limit) {
        macros->limit += BLOCK_LEN;
        _temp_macros = realloc(macros->macros, sizeof(*macros->macros) * macros->limit);
        if (_temp_macros != NULL) {
            macros->macros = _temp_macros;
        }
        else {
            printf("Memory allocation fail.\n");
            exit(-1);
        }
    }
    
    _macro = malloc(sizeof(*_macro));
    if (_macro == NULL) {
        printf("Memory allocation fail.\n");
        exit(-1);
    }

    init_macro(_macro, name);
    macros->macros[macros->size] = _macro;
    macros->size++;
    
    return _macro;
}

