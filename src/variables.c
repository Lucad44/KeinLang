//
// Created by lucad on 02/09/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "variables.h"

struct hashmap *vars = NULL;

static void __attribute__((constructor)) init_vars() {
    vars = hashmap_new(sizeof(Variable), 0, 0, 0, var_hash, var_compare, NULL, NULL);
}

int var_compare(const void *a, const void *b, void *udata) {
    const Variable *va = a;
    const Variable *vb = b;
    return strcmp(va->name, vb->name);
}

uint64_t var_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const Variable *var = item;
    return hashmap_sip(var->name, strlen(var->name), seed0, seed1);
}
