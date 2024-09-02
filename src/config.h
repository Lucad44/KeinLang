//
// Created by lucad on 02/09/2024.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <getopt.h>
#include <stddef.h>

const char *version = "0.5";

const char *help =
    "Usage: keinLang [options]\n"
    "\n"
    "  -h, --help                  Show an help message.\n"
    "  -v, --version               Show the current version.\n"
    "  -f, --file-name             [Required] Indicate the keinLang file to parse.\n";

static struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {"file-name", required_argument, NULL, 'f'},
    {NULL, 0, NULL, 0}
};

int parse_arguments(int argc, char *argv[]);

#endif //CONFIG_H
