#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "parser.h"
#include "variables.h"
#include "hashmap.h"

int main(int argc, char *argv[]) {
    const int ret = parse_arguments(argc, argv);
    hashmap_scan(vars, var_iter, NULL);
    hashmap_free(vars);
    return ret;
}
