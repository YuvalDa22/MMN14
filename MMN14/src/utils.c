#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "defines.h"
#include "assembler.h"
#include "symbols.h"
#include "utils.h"

char g_token_buf[MAX_LINE_LEN];

const struct Instruction g_instructions[] = {
    {  "mov",  0, 2 }, {  "cmp",  1, 2 }, {  "add",  2, 2 }, {  "sub",  3, 2 }, 
    {  "lea",  4, 2 }, {  "clr",  5, 1 }, {  "not",  6, 1 }, {  "inc",  7, 1 }, 
    {  "dec",  8, 1 }, {  "jmp",  9, 1 }, {  "bne", 10, 1 }, {  "red", 11, 1 },
    {  "prn", 12, 1 }, {  "jsr", 13, 1 }, {  "rts", 14, 0 }, { "stop", 15, 0 }
};

/* write one line to object file */
void write_object_file(FILE* fp_obj,
    int counter, int value);

/* get tokens count */
int get_token_count(const char* line) {
    char* _token;
    int _count;

    _count = 0;
    do {
        _token = get_token(line, _count);
        if (*_token == '\0')
            break;
        _count++;
    } while (TRUE);

    return _count;
}

/* get token */
char* get_token(const char* line, int start_index) {
    char _letter;
    int _index_a = 0, _index_b, _index_token = 0, _index_copy;
    int _found;

    g_token_buf[0] = '\0';

    while (TRUE) {
        if (start_index < _index_token)
            break;

        while (TRUE) {
            _letter = line[_index_a];
            if (_letter == ';')
                _letter = '\0';

            if (!isspace(_letter))
                break;

            _index_a++;
        }

        _index_b = _index_a;

        while (TRUE) {
            _letter = line[_index_b];

            if (_letter == ';')
                _letter = '\0';

            _found = isspace(_letter) || _letter == ',' || _letter == ':' || _letter == '\0';

            if (_found) {
                if (_index_a == _index_b && (_letter == ',' || _letter == ':'))
                    _index_b++;
                break;
            }

            _index_b++;
        }

        if (start_index == _index_token) {
            _index_copy = 0;
            while (_index_a < _index_b) {
                _letter = line[_index_a];

                if (_letter == ';')
                    _letter = '\0';

                g_token_buf[_index_copy] = _letter;
                _index_copy++;

                _index_a++;
            }

            g_token_buf[_index_copy] = '\0';
        }

        _index_a = _index_b;
        _index_token++;
    }

    return g_token_buf;
}

/* check if token is reserved */
bool check_reserved(const char* name) {
    int _idx, _count;
    bool _result = FALSE;
    const char* _reserved[] = { ".data", ".entry", ".extern", ".string", "mov", "cmp", "add", "sub", 
        "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop" };
    _count = sizeof(_reserved) / sizeof(char*);
    for (_idx = 0; _idx < _count; _idx++) {
        if (strcmp(_reserved[_idx], name) != 0)
            continue;
        _result = TRUE;
        break;
    }
    return _result;
}

/* check if a label is valid */
bool check_label(const char* name) {
    int _idx, _count;
    const char* _guides[] = { ".data", ".entry", ".extern", ".string" };
    bool _result = TRUE;

    if (get_instruction(name) != NULL)
        return FALSE;

    _count = sizeof(_guides) / sizeof(char*);
    for (_idx = 0; _idx < _count; _idx++) {
        if (strcmp(_guides[_idx], name) != 0)
            continue;
        _result = FALSE;
        break;
    }

    return _result;
}

/* check if an instruction is valid */
bool check_instruction(const char* line, int start_index, int token_count) {
    char* _token;
    const Instruction* _instruction;

    _instruction = NULL;
    _token = get_token(line, start_index);

    _instruction = get_instruction(_token);
    if (_instruction == NULL)
        return FALSE;

    if (_instruction->num_opr == OPR_NUM_0) {
        if (token_count != 1)
            return FALSE;
    }
    else if (_instruction->num_opr == OPR_NUM_1) {
        if (token_count != 2)
            return FALSE;
    }
    else if (_instruction->num_opr == OPR_NUM_2) {
        if (token_count != 4)
            return FALSE;
        _token = get_token(line, start_index + 2);
        if (strcmp(",", _token) != 0)
            return FALSE;
    }

    return TRUE;
}

/* check if guide is valid */
bool check_guide(const char* line, int start_index, int token_count) {
    char* _token;
    _token = get_token(line, start_index);

    if (strcmp(_token, ".data") == 0) {
        if (token_count % 2 != 0)
            return FALSE;

        while (TRUE) {
            token_count -= 2;
            if (token_count <= 0)
                break;

            _token = get_token(line, start_index + 2);
            if (strcmp(_token, ",") != 0)
                return FALSE;

            start_index += 2;
        }
        return TRUE;
    }
    else if (strcmp(_token, ".entry") == 0) {
        if (token_count != 2)
            return FALSE;

        _token = get_token(line, start_index + 1);
        add_entry(_token);

        return TRUE;
    }
    else if (strcmp(_token, ".extern") == 0) {
        if (token_count != 2)
            return FALSE;

        return TRUE;
    }
    else if (strcmp(_token, ".string") == 0) {
        if (token_count != 2)
            return FALSE;

        _token = get_token(line, start_index + 1);
        if (_token[0] != '"' || _token[strlen(_token) - 1] != '"')
            return FALSE;

        return TRUE;
    }

    return FALSE;
}

/* get instruction by name */
Instruction* get_instruction(const char* name) {
    int _idx, _count;
    Instruction* _result = NULL;
    _count = sizeof(g_instructions) / sizeof(Instruction);
    for (_idx = 0; _idx < _count; _idx++) {
        if (strcmp(g_instructions[_idx].name, name) != 0)
            continue;
        _result = (Instruction*) & g_instructions[_idx];
        break;
    }
    return _result;
}

/* get instruction size */
int get_size_of_instruction(const char* line, int start_index, int token_count) {
    const struct Instruction* _instruction;
    int _size;
    char* _token;

    _size = 0;
    _token = get_token(line, start_index);

    _instruction = get_instruction(_token);
    if (_instruction != NULL) {
        switch (_instruction->num_opr) {
        case OPR_NUM_0:
            if (token_count == 1)
                _size = 1;
            break;
        case OPR_NUM_1:
            if (token_count == 2)
                _size = 2;
            break;
        case OPR_NUM_2:
            if (token_count == 4) {
                char *_token_a, *_token_b;
                int _test_a, _test_a1, _test_a2;
                int _test_b, _test_b1, _test_b2;

                _size = 1;

                _token_a = get_token(g_line, start_index + 1);
                _test_a1 = _token_a[0] == 'r';
                _test_a2 = _token_a[0] == '*' && _token_a[1] == 'r';
                _test_a = _test_a1 || _test_a2;

                _token_b = get_token(g_line, start_index + 3);
                _test_b1 = _token_b[0] == 'r';
                _test_b2 = _token_b[0] == '*' && _token_b[1] == 'r';
                _test_b = _test_b1 || _test_b2;

                if (_test_a && _test_b) 
                    _size += 1;
                else 
                    _size += 2;
            }
            break;
        }
    }

    return _size;
}

/* get operand category */
int get_operand_type(const char* operand) {
    int _op_type;
    switch (operand[0]) {
    case 'r':
        _op_type = REGISTER_OPR;
        break;
    case '#':
        _op_type = IMMEDIATE_OPR;
        break;
    case '*':
        _op_type = INDIRECT_OPR;
        break;
    default:
        _op_type = DIRECT_OPR;
        break;
    }
    return _op_type;
}

/* get guide size */
int get_size_of_guide(const char* line, int start_index, int token_count) {
    char* _token;
    int _size = 0;

    _token = get_token(line, start_index);

    if (strcmp(_token, ".data") == 0)
        _size = token_count / 2;
    else if (strcmp(_token, ".string") == 0) {
        _token = get_token(line, start_index + 1);
        _size = (int)strlen(_token) - 1;
    }

    return _size; 
}

/* get register number */
int get_register_number(const char* operand) {
    int _num = -1;

    if (operand[0] == 'r') 
        sscanf(&operand[1], "%d", &_num);
    else if (operand[0] == '*') 
        sscanf(&operand[2], "%d", &_num);

    return _num;
}

/* save operand */
void save_operand(FILE* fpext, FILE* fpmc, int counter, const char* operand, int order) {
    int _type, _immediate, _num_reg, _value;

    _type = get_operand_type(operand);
    if (_type == IMMEDIATE_OPR) {
        sscanf(&operand[1], "%d", &_immediate);
        _value = (_immediate << 3) | ABSOLUTE;
        write_object_file(fpmc, counter, _value);
        g_pc++;
    }
    else if (_type == INDIRECT_OPR || _type == REGISTER_OPR) {
        _num_reg = get_register_number(operand);
        _value = ABSOLUTE;
        switch (order) {
        case 0:
            _value = _value | (_num_reg << 6);
            break;
        case 1:
            _value = _value | (_num_reg << 3);
            break;
        }
        write_object_file(fpmc, counter, _value);
        g_pc++;
    }
    else if (_type == DIRECT_OPR) {
        if (exist_label(operand) != TRUE) {
            _value = EXTERNAL;
            fprintf(fpext, "%s %04d\n", operand, counter);
            write_object_file(fpmc, counter, _value);
        }
        else {
            _value = (label_address(operand) << 3) | RELATIVE;
            write_object_file(fpmc, counter, _value);
        }
        g_pc++;
    }
}

/* output instruction to file */
void save_instruction(const char* line, int _start_index, int token_count, FILE* fpext, FILE* fpmc) {
    char* _token; 
    int _value, _opr_type;
    const Instruction* _instruction;

    _token = get_token(line, _start_index);
    _instruction = get_instruction(_token); 

    if (_instruction != NULL) { 
        switch (_instruction->num_opr) { 
        case OPR_NUM_0: 
            _value = (_instruction->op << 11) | ABSOLUTE; 
            write_object_file(fpmc, g_pc, _value);
            g_pc++; 
            break;
        case OPR_NUM_1: 
            _value = (_instruction->op << 11) | ABSOLUTE; 
            _token = get_token(line, _start_index + 1); 
            _opr_type = get_operand_type(_token); 

            _value = _value | (_opr_type << 3); 

            write_object_file(fpmc, g_pc, _value);
            g_pc++; 

            save_operand(fpext, fpmc, g_pc, _token, 1);
            break;
        case OPR_NUM_2: {
            int _opr_type1, _opr_type2;
            int _test_a, _test_a1, _test_a2; 
            int _test_b, _test_b1, _test_b2; 

            _token = get_token(line, _start_index + 1); 
            _opr_type1 = get_operand_type(_token);

            _token = get_token(line, _start_index + 3); 
            _opr_type2 = get_operand_type(_token);

            _value = (_instruction->op << 11) | ABSOLUTE; 
            _value = _value | (_opr_type1 << 7); 
            _value = _value | (_opr_type2 << 3); 

            write_object_file(fpmc, g_pc, _value);
            g_pc++; 

            _test_a1 = _opr_type1 == INDIRECT_OPR; 
            _test_a2 = _opr_type1 == REGISTER_OPR; 
            _test_a = _test_a1 || _test_a2;

            _test_b1 = _opr_type2 == INDIRECT_OPR;
            _test_b2 = _opr_type2 == REGISTER_OPR;
            _test_b = _test_b1 || _test_b2; 

            if (_test_a && _test_b) { 
                int _num_reg1, _num_reg2; 

                _token = get_token(line, _start_index + 1); 
                _num_reg1 = get_register_number(_token); 

                _token = get_token(line, _start_index + 3);
                _num_reg2 = get_register_number(_token);

                _value = ABSOLUTE; 
                _value = _value | (_num_reg1 << 6); 
                _value = _value | (_num_reg2 << 3); 

                write_object_file(fpmc, g_pc, _value);
                g_pc++; 
            }
            else { 
                _token = get_token(line, _start_index + 1);
                save_operand(fpext, fpmc, g_pc, _token, 0);

                _token = get_token(line, _start_index + 3); 
                save_operand(fpext, fpmc, g_pc, _token, 1);
            }
            break;
        }
        }
    }
}

/*output guide */
void save_guide(const char* line, int start_index, int token_count, FILE* fpmc) {
    char* _token;
    int _index, _limit, _value;

    _token = get_token(line, start_index);

    if (strcmp(_token, ".data") == 0) {
        while (token_count > 0) {
            _token = get_token(line, start_index + 1);

            _value = 0;
            sscanf(_token, "%d", &_value);
            write_object_file(fpmc, g_pc, _value);
            g_pc++;

            start_index += 2;
            token_count -= 2;
        }
    }
    else if (strcmp(_token, ".string") == 0) {
        _token = get_token(line, start_index + 1);

        _limit = (int)strlen(_token) - 1;
        for (_index = 1; _index < _limit; _index++) {
            char letter = _token[_index];
            _value = letter & 0xFF;
            write_object_file(fpmc, g_pc, _value);

            g_pc++;
        }

        write_object_file(fpmc, g_pc, 0);
        g_pc++;
    }
}

/* output object file */
void write_object_file(FILE* fpmc, int counter, int value) {
    fprintf(fpmc, "%d %05o\n", counter, value & 0x7FFF);
}


