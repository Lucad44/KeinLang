cmake_minimum_required(VERSION 3.29)
project(keinLang C)

set(CMAKE_C_STANDARD 23)

add_executable(keinLang src/main.c
        src/hashmap/hashmap.c
        src/hashmap/hashmap.h
        src/variables.h
        src/variables.c
        src/parser.c
        src/parser.h
        src/config.c
        src/config.h
        src/functions.c
        src/functions.h
        src/tinyexpr/tinyexpr.c
        src/tinyexpr/tinyexpr.h
)
