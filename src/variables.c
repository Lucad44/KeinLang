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

char *var_to_string(const Variable *variable) {
    const char *name_str = "name = ";
    const char *type_str = ", type = ";
    const char *value_str = ", value = ";
    char *type = malloc(MAX_TYPE_LENGTH);
    char *val = malloc(MAX_VALUE_LENGTH);
    if (variable->type == TYPE_INT) {
        strncpy(type, "int", MAX_TYPE_LENGTH);
        snprintf(val, MAX_VALUE_LENGTH, "%d", variable->val.i);
    } else if (variable->type == TYPE_FLOAT) {
        strncpy(type, "float", MAX_TYPE_LENGTH);
        snprintf(val, MAX_VALUE_LENGTH, "%f", variable->val.f);
    } else if (variable->type == TYPE_DOUBLE) {
        strncpy(type, "double", MAX_TYPE_LENGTH);
        snprintf(val, MAX_VALUE_LENGTH, "%lf", variable->val.d);
    } else if (variable->type == TYPE_CHAR) {
        strncpy(type, "char", MAX_TYPE_LENGTH);
        snprintf(val, MAX_VALUE_LENGTH, "%c", variable->val.c);
    } else if (variable->type == TYPE_STRING) {
        strncpy(type, "string", MAX_TYPE_LENGTH);
        snprintf(val, MAX_VALUE_LENGTH, "%s", variable->val.s);
    } else if (variable->type == TYPE_BOOL) {
        strncpy(type, "bool", MAX_TYPE_LENGTH);
        snprintf(val, MAX_VALUE_LENGTH, "%d", variable->val.b);
    } else {
        strncpy(type, "invalid", MAX_TYPE_LENGTH);
        snprintf(val, MAX_VALUE_LENGTH, "invalid");
    }

    size_t len = strlen(name_str) + strlen(variable->name) + strlen(type_str) + strlen(type) +
        strlen(value_str) + strlen(val) + 1;
    char *ret = malloc(len);
    snprintf(ret, len, "%s%s%s%s%s%s%s", name_str, variable->name, type_str, type, value_str, val, "\0");
    free(type);
    free(val);
    return ret;
}

bool var_iter(const void *item, void *udata) {
    const Variable *variable = item;
    char *str = var_to_string(variable);
    printf("%s\n", str);
    free(str);
    return true;
}
