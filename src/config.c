//
// Created by lucad on 02/09/2024.
//


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "config.h"
#include "parser.h"

int parse_arguments(int argc, char *argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    int c;
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "f:hv", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                printf("%s\n", help);
                return EXIT_SUCCESS;
            case 'v':
                printf("keinLang's current version: %s\n", version);
                return EXIT_SUCCESS;
            case 'f':
                return parse_file(optarg);
            case '?':
                return EXIT_FAILURE;
            default:
                break;
        }
    }
    return EXIT_FAILURE;
}
