#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "assembler.h"
#include "utils.h"
#include "symbols.h"


int main(int argc, char* argv[]) {
    FILE* fpas, * fpam, * fpent, * fpext, * fpmc;
    int count_errors;
    bool empty_ent, empty_ext;
    char file_name[512];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file-prefix>\n", argv[0]);
        return -1;
    }

    init_symbols();

    /* open as file */
    sprintf(file_name, "%s.as", argv[1]);
    fpas = fopen(file_name, "r");

    /* open am file */
    sprintf(file_name, "%s.am", argv[1]);
    fpam = fopen(file_name, "w+");

    empty_ent = FALSE;
    empty_ext = FALSE;

    count_errors = process_first_phase(fpas, fpam);

    if (count_errors == 0) {
        fseek(fpam, 0, SEEK_SET);

        sprintf(file_name, "%s.ent", argv[1]);
        fpent = fopen(file_name, "w+");

        sprintf(file_name, "%s.ext", argv[1]);
        fpext = fopen(file_name, "w+");

        sprintf(file_name, "%s.ob", argv[1]);
        fpmc = fopen(file_name, "w+");

        process_second_phase(fpam, fpent, fpext, fpmc);

        if (ftell(fpent) == 0)
            empty_ent = TRUE;

        if (ftell(fpext) == 0)
            empty_ext = TRUE;

        fclose(fpent);
        fclose(fpext);
        fclose(fpmc);
    }

    fclose(fpam);
    fclose(fpas);

    free_symbols();

    if (empty_ent) {
        sprintf(file_name, "%s.ent", argv[1]);
        remove(file_name);
    }

    if (empty_ext) {
        sprintf(file_name, "%s.ext", argv[1]);
        remove(file_name);
    }

    return 0;
}

