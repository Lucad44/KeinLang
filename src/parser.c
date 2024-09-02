//
// Created by lucad on 02/09/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int parse_file(char *file_name) {
    FILE *fptr;
    char buffer[MAX_LINE_LEN];
    int idx = 0;
    int ch;
    if ((fptr = fopen(file_name, "r")) == NULL) {
        return EXIT_FAILURE;
    }

    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == ';') {
            buffer[idx] = '\0';
            parse_line(buffer);
            idx = 0;
        } else {
            if (idx < MAX_LINE_LEN - 1) {
                buffer[idx++] = (char) ch;
            } else {
                fclose(fptr);
                return EXIT_FAILURE;
            }
        }
    }
    if (idx > 0) {
        buffer[idx] = '\0';
        parse_line(buffer);
    }

    fclose(fptr);
    return EXIT_SUCCESS;
}

int parse_line(char *line) {
    printf("%s\n", line);
    return 4;
}
