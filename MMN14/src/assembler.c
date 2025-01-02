#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "assembler.h"
#include "utils.h"
#include "symbols.h"

/* Define static variables */
char g_line[MAX_LINE_LEN]; /* buffer to store a line of input */
int g_line_num; /* current line number */
int g_pc; /* program counter */
int g_code_count; /* count of code lines */
int g_data_count; /* count of data lines */
int g_err_count; /* count of errors */


/* handle line in first step */
void handle_in_first_phase(const char* line) {
    char* _token;
    int _category, _token_count, _start_index, _size;

    _start_index = 0;
    _token_count = get_token_count(line);

    while (_token_count >= 2) {
        _token = get_token(line, _start_index + 1);
        if (strcmp(_token, ":") != 0) break;

        _token = get_token(line, _start_index);
        _token_count -= 2;
        _start_index += 2;

        if (!check_label(_token)) {
            printf("line #%d: duplicate label \"%s\"\n", g_line_num, _token);
            g_err_count++;
        }
        else if (!add_label(_token, g_pc)) {
            printf("line #%d: duplicate label \"%s\"\n", g_line_num, _token);
            g_err_count++;
        }
    }

    if (_token_count == 0) 
        return;

    _category = CATEGORY_NONE;
    if (check_instruction(line, _start_index, _token_count)) 
        _category = CATEGORY_CODE;
    else if (check_guide(line, _start_index, _token_count)) 
        _category = CATEGORY_DATA;

    _size = 0;
    if (_category == CATEGORY_CODE) {
        _size = get_size_of_instruction(line, _start_index, _token_count);
        g_code_count += _size;
    }
    else if (_category == CATEGORY_DATA) {
        _size = get_size_of_guide(line, _start_index, _token_count);
        g_data_count += _size;
    }
    else {
        printf("line #%d: syntax error\n", g_line_num);
        g_err_count++;
    }
    g_pc += _size;
}

/* handle line in second step */
void handle_in_second_phase(const char* line, FILE* fpext, FILE* fpmc) {
    char* _token;
    int _category, _token_count, _start_index;

    _start_index = 0;
    _token_count = get_token_count(line);

    while (_token_count >= 2) {
        _token = get_token(line, _start_index + 1);
        if (strcmp(_token, ":") != 0)
            break;
        _token_count -= 2;
        _start_index += 2;
    }

    if (_token_count == 0) return;

    _category = CATEGORY_NONE;
    if (check_instruction(line, _start_index, _token_count))
        _category = CATEGORY_CODE;
    else if (check_guide(line, _start_index, _token_count))
        _category = CATEGORY_DATA;

    if (_category == CATEGORY_CODE)
        save_instruction(line, _start_index, _token_count, fpext, fpmc);
    else if (_category == CATEGORY_DATA)
        save_guide(line, _start_index, _token_count, fpmc);
}

/* handle first step */
int process_first_phase(FILE* fpas, FILE* fpam) {
    char* _token;
    int _index, _token_count;

    Macro *_live_macro, *_macro; 
    Macros macros; 

    g_line_num = 0;
    g_code_count = 0;
    g_data_count = 0;
    g_err_count = 0;

    _live_macro = NULL;

    init_macro_array(&macros); 

    g_pc = 100; 

    while (fgets(g_line, MAX_LINE_LEN, fpas) != NULL) {
        g_line_num++; 

        _token_count = get_token_count(g_line);

        if (_token_count == 1) {
            _token = get_token(g_line, 0);
            if (strcmp(_token, "endmacr") == 0) {
                if (_live_macro == NULL) {
                    printf("line #%d: \"endmacro\" outside macro\n", g_line_num);
                    g_err_count++;
                    continue;
                }
                _live_macro = NULL;
                continue;
            }

            if (!check_reserved(_token)) {
                _macro = get_macro(&macros, _token);

                if (_macro == NULL) {
                    printf("line #%d: undefined macro invocation \"%s\"\n", g_line_num, _token);
                    g_err_count++;
                    continue;
                }

                for (_index = 0; _index < _macro->size; _index++) {
                    handle_in_first_phase(_macro->instructions[_index]);
                    fputs(_macro->instructions[_index], fpam);
                }
                continue;
            }
        }

        if (_token_count == 2) {
            _token = get_token(g_line, 0);
            if (strcmp(_token, "macr") == 0) {
                _token = get_token(g_line, 1);
                _live_macro = add_macro(&macros, _token); 

                if (_live_macro == NULL) {
                    printf("line #%d: duplicate macro name \"%s\"\n", g_line_num, _token);
                    g_err_count++;
                    continue;
                }
                continue;
            }
        }

        if (_live_macro == NULL) {
            handle_in_first_phase(g_line);
            fputs(g_line, fpam);
        } else {
            add_instruction(_live_macro, g_line);
        }
    }

    free_macro_array(&macros);

    return g_err_count;
}

/* handle second step */
void process_second_phase(FILE* fpas, FILE* fpent, FILE* fpext, FILE* fpmc) {
    int _index; 

    g_pc = 100;

    fprintf(fpmc, "%d %d\n", g_code_count, g_data_count);

    while (fgets(g_line, MAX_LINE_LEN, fpas) != NULL) { 
        handle_in_second_phase(g_line, fpext, fpmc);
    }

    for (_index = 0; _index < g_labels.size; _index++)
        update_entry(g_labels.labels[_index].name, g_labels.labels[_index].address);
    
    sort_entries();

    for (_index = 0; _index < g_entries.size; _index++) {
        fprintf(fpent, "%s %d\n", g_entries.entries[_index].name, g_entries.entries[_index].address);
    }
}

/* initialize entry and lable structure */
void init_symbols() {
    init_entries();
    init_labels();
}

/* free memory */
void free_symbols() {
    free_entries();
    free_labels();
}
