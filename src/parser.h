//
// Created by lucad on 02/09/2024.
//

#ifndef PARSER_H
#define PARSER_H

#define BUFFER_SIZE 1024

#include "variables.h"

const char *skip_whitespace(const char *str);

void trim(char *str);

bool is_integer(const char *str);

bool is_decimal(const char *str);

bool is_char(const char *str);

bool is_string(const char *str);

bool is_bool(const char *str);

int add_to_map(Type type, const char *name, const char *value);

int parse_file(char *file_name);

int parse_line(char *line);

char *get_condition(const char *token);

char *get_body(char **token_ptr);

bool is_declaration(const char *line);

bool is_print_statement(const char *line);

bool is_if_statement(const char *line);

bool is_while_statement(const char *line);

int parse_variable(const char *line);

int parse_if(const char *condition, char *body);

int parse_while(const char *condition, char *body);

int write_to_file(char *body, char *uuid_str);

void generate_uuid_v4(char *uuid_str);

Type get_type(const char *declaration);

#endif //PARSER_H
