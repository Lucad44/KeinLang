#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "variables.h"

int main(int argc, char *argv[]) {
    const int ret = parse_arguments(argc, argv);
    hashmap_scan(vars, var_iter, NULL);
    return ret;
}
