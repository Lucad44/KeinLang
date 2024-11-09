//
// Created by lucad on 21/10/2024.
//

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "hashmap.h"
#include "parser.h"
#include "variables.h"
#include "hashmap/hashmap.h"

int print_variable_value(const Variable *variable) {
    if (variable->type == TYPE_INT) {
        printf("%d", variable->val.i);
    } else if (variable->type == TYPE_FLOAT) {
        printf("%f", variable->val.f);
    } else if (variable->type == TYPE_DOUBLE) {
        printf("%lf", variable->val.d);
    } else if (variable->type == TYPE_CHAR) {
        printf("%c", variable->val.c);
    } else if (variable->type == TYPE_STRING) {
        printf("%s", variable->val.s);
    } else if (variable->type == TYPE_BOOL) {
        printf("%d", variable->val.b);
    } else {
        fprintf(stderr, "Unknown variable type\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int evaluate_expression(const char *p) {
    while (*p != ')' && *p != '\0') {
        p = skip_whitespace(p);
        if (*p == '\"') {
            p++;
            while (*p && (*p != '\"' || *(p - 1) == '\\')) {
                putchar(*p);
                p++;
            }
            if (*p == '\"') p++;
        } else if (*p == '+') {
            p = skip_whitespace(++p);
        } else {
            char name[BUFFER_SIZE] = {0};
            const char *name_start = p;
            while (*p && *p != ')' && *p != ' ' && *p != '+') p++;
            strncpy(name, name_start, p - name_start);
            name[p - name_start] = '\0';
            const Variable *variable = hashmap_get(vars, &(Variable) { .name=name });
            if (variable == NULL) {
                fprintf(stderr, "Variable %s not found\n", name);
                return EXIT_FAILURE;
            }
            if (print_variable_value(variable) != EXIT_SUCCESS) {
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}


int do_print(const char *str) {
    const char *p = str;
    p = skip_whitespace(p);
    if (strncmp(p, PRINT_LITERAL, strlen(PRINT_LITERAL)) != 0) {
        fprintf(stderr, "Invalid print statement\n");
        return EXIT_FAILURE;
    }
    p = skip_whitespace(p + strlen(PRINT_LITERAL));
    if (*p != '(') {
        fprintf(stderr, "Expected '('\n");
        return EXIT_FAILURE;
    }
    p++;
    if (evaluate_expression(p) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    putchar('\n');
    return EXIT_SUCCESS;
}
