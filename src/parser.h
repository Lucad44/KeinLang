//
// Created by lucad on 02/09/2024.
//

#ifndef PARSER_H
#define PARSER_H

#define BUFFER_SIZE 1024

#include "variables.h"

void trim(char *str);

bool is_integer(const char *str);

bool is_decimal(const char *str);

int add_to_map(Type type, char *name, char *value);

int parse_file(char *file_name);

int parse_line(const char *line);

bool is_declaration(const char *line);

int parse_variable(const char *line);

Type get_type(const char *declaration);

#endif //PARSER_H
