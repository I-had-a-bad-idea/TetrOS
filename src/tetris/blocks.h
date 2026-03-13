#pragma once
#include "kernel/stdint.h"

#define BLOCK_ARRAY_AXIS_SIZE 4

typedef struct {
    bool cells[BLOCK_ARRAY_AXIS_SIZE][BLOCK_ARRAY_AXIS_SIZE];
    int x, y; // top-left corner of block in field coordinates
} Block;

extern Block I;
extern Block O;
extern Block T;
extern Block S;
extern Block Z;
extern Block J;
extern Block L;
