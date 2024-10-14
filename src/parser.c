//
// Created by lucad on 02/09/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"
#include "variables.h"

void trim(char *str) {
    while (isspace(*str)) str++;
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

    variable->type = type;
    if (type == TYPE_INT) {
        if (!is_integer(value)) {
            fprintf(stderr, "Invalid int value\n");
            return EXIT_FAILURE;
        }
        variable->val.i = atoi(value);
    } else if (type == TYPE_FLOAT || type == TYPE_DOUBLE) {
        if (!is_decimal(value)) {
            fprintf(stderr, "Invalid float value\n");
            return EXIT_FAILURE;
        }
        if (type == TYPE_FLOAT) {
            variable->val.f = atof(value);
        } else {
            variable->val.d = strtod(value, NULL);
        }
    } else if (type == TYPE_CHAR) {
        if (!is_char(value)) {
            fprintf(stderr, "Invalid char value\n");
            return EXIT_FAILURE;
        }
        variable->val.c = value[0];
    } else if (type == TYPE_STRING) {
        if (!is_string(value)) {
            fprintf(stderr, "Invalid string value\n");
            return EXIT_FAILURE;
        }
        const size_t value_len = strlen(value) + 1;
        variable->val.s = malloc(value_len);
        strncpy(variable->val.s, value, value_len);
    } else if (type == TYPE_BOOL) {
        if (!is_bool(value)) {
            fprintf(stderr, "Invalid bool value\n");
            return EXIT_FAILURE;
        }
        variable->val.b = strcmp(value, "true") == 0;
    }
    hashmap_set(vars, &variable);
    return EXIT_SUCCESS;
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

int parse_line(char *line) {
    const char *token = strtok(line, ";");
    while (token != NULL) {
        if (is_declaration(token)) {
            parse_variable(token);
        }
        token = strtok(NULL, ";");
    }
    return EXIT_SUCCESS;
}

bool is_declaration(const char *line) {
    return strstr(line, "int") || strstr(line, "float") || strstr(line, "double") ||
        strstr(line, "char") || strstr(line, "string") || strstr(line, "bool") &&
        strstr(line, "=");
}

int parse_variable(const char *line) {
    char type[BUFFER_SIZE] = {0};
    char name[BUFFER_SIZE] = {0};
    char value[BUFFER_SIZE] = {0};
    const char* p = line;

    // Skip leading whitespace
    while (isspace(*p)) p++;

    // Parse the type
    const char* type_start = p;
    while (!isspace(*p)) p++;
    strncpy(type, type_start, p - type_start);
    type[p - type_start] = '\0';

    // Skip whitespace
    while (isspace(*p)) p++;

    // Parse the name
    const char* name_start = p;
    while (*p != '=' && !isspace(*p)) p++;
    strncpy(name, name_start, p - name_start);
    name[p - name_start] = '\0';

    // Skip to equals sign
    while (*p != '=' && *p != '\0') p++;
    if (*p == '=') p++;

    // Skip whitespace after '='
    while (isspace(*p)) p++;

    // Parse the value
    const char* value_start = p;
    while (*p != '\0' && *p != ';') p++;
    strncpy(value, value_start, p - value_start);
    value[p - value_start] = '\0';

    // Trim type, name, and value
    trim(type);
    trim(name);
    trim(value);

    // Print parsed type, name, and value


    // Check if the value is a string literal (starts and ends with quotes)
    if ((value[0] == '\"' && value[strlen(value) - 1] == '\"') || (value[0] == '\'' && value[strlen(value) - 1] == '\'')) {
        // Create a local copy of value without the first and last quote
        memmove(value, value + 1, strlen(value) - 2);  // Shift to remove first quote
        value[strlen(value) - 2] = '\0';  // Remove last quote by terminating early
    }

    // Print the modified value if it's a string
    //printf("Modified Value: %s\n", value);
   // printf("Type: %s, Name: %s, Value: %s\n", type, name, value);

    // Add to map or perform further processing (assumes these functions exist)
    return add_to_map(get_type(type), name, value);
}

Type get_type(const char *declaration) {
    if (strcmp(declaration, "int") == 0) {
        return TYPE_INT;
    } else if (strcmp(declaration, "float") == 0) {
        return TYPE_FLOAT;
    } else if (strcmp(declaration, "double") == 0) {
        return TYPE_DOUBLE;
    } else if (strcmp(declaration, "char") == 0) {
        return TYPE_CHAR;
    } else if (strcmp(declaration, "string") == 0) {
        return TYPE_STRING;
    } else if (strcmp(declaration, "bool") == 0) {
        return TYPE_BOOL;
    }
    return INVALID;
}
