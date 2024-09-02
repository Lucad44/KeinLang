//
// Created by lucad on 02/09/2024.
//

#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>
#include "hashmap.h"

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_BOOL
} VarType;

typedef struct {
    VarType type;
    char *name;
    union {
        int i;
        float f;
        double d;
        char c;
        char *s;
        bool b;
    } val;
} Variable;

extern struct hashmap *vars;

int var_compare(const void *a, const void *b, void *udata);

uint64_t var_hash(const void *item, uint64_t seed0, uint64_t seed1);

#endif //VARIABLES_H
