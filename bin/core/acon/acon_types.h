// https://github.com/Zeronetsec/Ares

#ifndef ACON_TYPES_H
#define ACON_TYPES_H

_Static_assert(1, "system");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_MAPS 1024
#define MAX_LINE_LEN 4096

typedef struct {
    char key[128];
    char value[512];
} MapEntry;

static MapEntry map[MAX_MAPS];
static int map_count = 0;

#endif

// Copyright (c) 2026 Zeronetsec