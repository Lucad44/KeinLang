//
// Created by lucad on 02/09/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"

#include <time.h>

#include "variables.h"
#include "functions.h"
#include "hashmap/hashmap.h"
#include "tinyexpr/tinyexpr.h"

const char* skip_whitespace(const char *str) {
    while (isspace(*str)) str++;
    return str;
}

void trim(char *str) {
    str = skip_whitespace(str);
    if (*str == 0)
        return;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;
    *(end + 1) = 0;
}

bool is_integer(const char *str) {
    if (*str == '+' || *str == '-') {
        str++;
    }
    while (*str) {
        if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

bool is_decimal(const char *str) {
    if (*str == '+' || *str == '-') {
        str++;
    }

    bool has_integer_part = false;
    bool has_fractional_part = false;
    while (isdigit(*str)) {
        has_integer_part = true;
        str++;
    }
    if (*str == '.') {
        str++;
        while (isdigit(*str)) {
            has_fractional_part = true;
            str++;
        }
    }
    if (*str == 'e' || *str == 'E') {
        str++;
        if (*str == '+' || *str == '-') {
            str++;
        }
        while (*str) {
            if (!isdigit(*str)) {
                return false;
            }
            str++;
        }
    }
    return *str == '\0' && (has_integer_part || has_fractional_part);
}

bool is_char(const char *str) {
    return strlen(str) == 1;
}

bool is_string(const char *str) {
    return strlen(str) < MAX_VALUE_LENGTH;
}

bool is_bool(const char *str) {
    return strcmp(str, "true") == 0 || strcmp(str, "false") == 0;
}

int add_to_map(const Type type, const char *name, const char *value) {
    if (type == INVALID) {
        fprintf(stderr, "Invalid type\n");
        return EXIT_FAILURE;
    }
    if (*name == '\0') {
        fprintf(stderr, "Empty name\n");
        return EXIT_FAILURE;
    }
    if (*value == '\0') {
        fprintf(stderr, "Empty value\n");
        return EXIT_FAILURE;
    }
    Variable *variable = malloc(sizeof(Variable));

    const size_t name_len = strlen(name) + 1;
    variable->name = malloc(name_len);
    strncpy(variable->name, name, name_len);
    bool success = true;

    variable->type = type;
    if (type == TYPE_INT) {
        if (!is_integer(value)) {
            fprintf(stderr, "Invalid int value\n");
            success = false;
        }
        variable->val.i = atoi(value);
    } else if (type == TYPE_FLOAT || type == TYPE_DOUBLE) {
        if (!is_decimal(value)) {
            fprintf(stderr, "Invalid float value\n");
            success = false;
        }
        if (type == TYPE_FLOAT) {
            variable->val.f = atof(value);
        } else {
            variable->val.d = strtod(value, NULL);
        }
    } else if (type == TYPE_CHAR) {
        if (!is_char(value)) {
            fprintf(stderr, "Invalid char value\n");
            success = false;
        }
        variable->val.c = value[0];
    } else if (type == TYPE_STRING) {
        if (!is_string(value)) {
            fprintf(stderr, "Invalid string value\n");
            success = false;
        }
        const size_t value_len = strlen(value) + 1;
        variable->val.s = malloc(value_len);
        strncpy(variable->val.s, value, value_len);
    } else if (type == TYPE_BOOL) {
        if (!is_bool(value)) {
            fprintf(stderr, "Invalid bool value\n");
            success = false;
        }
        variable->val.b = strcmp(value, "true") == 0;
    }
    hashmap_set(vars, variable);
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

int parse_file(char *file_name) {
    FILE* fptr = fopen(file_name, "r");
    if (fptr == NULL) {
        fprintf(stderr, "Failed to open file %s\n", file_name);
        return EXIT_FAILURE;
    }
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fptr)) {
        char *semicolon = strchr(buffer, ';');
        if (semicolon) {
            *semicolon = '\0';
            parse_line(buffer);
        }
    }
    fclose(fptr);
    return EXIT_SUCCESS;
}

char *get_condition(const char *token) {
    char *condition = strchr(token, '(');
    if (condition) {
        condition++;
        char *end_condition = strchr(condition, ')');
        if (end_condition) {
            *end_condition = '\0';
            return condition;
        }
    }
    return NULL;
}

char *get_body(char **token_ptr) {
    char *start = strchr(*token_ptr, '{');
    if (!start) {
        return NULL;
    }
    start++;
    int brace_cnt = 1;
    char *end = start;
    while (*end && brace_cnt > 0) {
        if (*end == '{') {
            brace_cnt++;
        } else if (*end == '}') {
            brace_cnt--;
        }
        end++;
    }
    if (brace_cnt != 0) {
        return NULL;
    }
    const size_t body_size = end - start - 1;
    char *body = malloc(body_size + 1);
    if (body) {
        strncpy(body, start, body_size);
        body[body_size] = '\0';
    }
    *token_ptr = end;
    return body;
}

int parse_line(char *line) {
    for (const char *token = strtok(line, ";"); token; token = strtok(NULL, ";")) {
        if (is_declaration(token)) {
            parse_variable(token);
        } else if (is_print_statement(token)) {
            do_print(token);
        } else if (is_if_statement(token)) {
            const char *condition = get_condition(token);
            char *body = get_body(&token);
            parse_if(condition, body);
            free(body);
        } else if (is_while_statement(token)) {
            const char *condition = get_condition(token);
            char *body = get_body(&token);
            parse_while(condition, body);
            free(body);
        }
    }
    return EXIT_SUCCESS;
}

bool is_declaration(const char *line) {
    return strstr(line, "int") || strstr(line, "float") || strstr(line, "double") ||
        strstr(line, "char") || strstr(line, "string") || strstr(line, "bool") &&
        strstr(line, "=");
}

bool is_print_statement(const char *line) {
    return strstr(line, "print");
}

bool is_if_statement(const char *line) {
    return strncmp(line, "if", 2) == 0;
}

bool is_while_statement(const char *line) {
    return strncmp(line, "while", 5) == 0;
}

int parse_variable(const char *line) {
    char type[BUFFER_SIZE] = {0};
    char name[BUFFER_SIZE] = {0};
    char value[BUFFER_SIZE] = {0};
    const char* p = line;

    p = skip_whitespace(p);
    const char* type_start = p;
    while (!isspace(*p)) p++;
    strncpy(type, type_start, p - type_start);
    type[p - type_start] = '\0';
    p = skip_whitespace(p);
    const char* name_start = p;
    while (*p != '=' && !isspace(*p)) p++;
    strncpy(name, name_start, p - name_start);
    name[p - name_start] = '\0';
    while (*p != '=' && *p != '\0') p++;
    if (*p == '=') p++;
    p = skip_whitespace(p);
    const char* value_start = p;
    while (*p != '\0' && *p != ';') p++;
    strncpy(value, value_start, p - value_start);
    value[p - value_start] = '\0';
    trim(type);
    trim(name);
    trim(value);
    if ((value[0] == '\"' && value[strlen(value) - 1] == '\"') || (value[0] == '\'' && value[strlen(value) - 1] == '\'')) {
        memmove(value, value + 1, strlen(value) - 2);
        value[strlen(value) - 2] = '\0';
    }
    return add_to_map(get_type(type), name, value);
}

int write_to_file(char *body, char *uuid_str) {
    FILE *fptr = fopen(uuid_str, "w");
    if (fptr == NULL) {
        return EXIT_FAILURE;
    }
    fprintf(fptr, "%s", body);
    fclose(fptr);
    return EXIT_SUCCESS;
}

void generate_uuid_v4(char *uuid_str) {
    unsigned char uuid[16];
    srand((unsigned) time(NULL));
    for (int i = 0; i < 16; i++) {
        uuid[i] = rand() % 256;
    }
    uuid[6] = (uuid[6] & 0x0f) | 0x40;
    uuid[8] = (uuid[8] & 0x3f) | 0x80;
    snprintf(uuid_str, 37, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
            uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
}

int parse_if(const char *condition, char *body) {
    if (te_interp(condition, nullptr) == 0) {
        return EXIT_SUCCESS;
    }
    char uuid_str[37];
    generate_uuid_v4(uuid_str);
    if (write_to_file(body, uuid_str) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    parse_file(uuid_str);
    remove(uuid_str);
    return EXIT_SUCCESS;
}

int parse_while(const char *condition, char *body) {
    char uuid_str[37];
    generate_uuid_v4(uuid_str);
    if (write_to_file(body, uuid_str) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    while (te_interp(condition, nullptr) != 0) {
        parse_file(uuid_str);
    }
    remove(uuid_str);
    return EXIT_SUCCESS;
}

Type get_type(const char *declaration) {
    if (strcmp(declaration, "int") == 0) {
        return TYPE_INT;
    }
    if (strcmp(declaration, "float") == 0) {
        return TYPE_FLOAT;
    }
    if (strcmp(declaration, "double") == 0) {
        return TYPE_DOUBLE;
    }
    if (strcmp(declaration, "char") == 0) {
        return TYPE_CHAR;
    }
    if (strcmp(declaration, "string") == 0) {
        return TYPE_STRING;
    }
    if (strcmp(declaration, "bool") == 0) {
        return TYPE_BOOL;
    }
    return INVALID;
}
