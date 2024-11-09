//
// Created by lucad on 21/10/2024.
//

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "variables.h"

static const char *PRINT_LITERAL = "print";

int print_variable_value(const Variable *variable);

int evaluate_expression(const char *p);

int do_print(const char *str);

#endif //FUNCTIONS_H
