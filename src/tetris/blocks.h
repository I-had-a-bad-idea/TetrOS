#pragma once
#include "kernel/stdint.h"

#define BLOCK_ARRAY_AXIS_SIZE 4

typedef struct {
    bool cells[BLOCK_ARRAY_AXIS_SIZE][BLOCK_ARRAY_AXIS_SIZE];
} Block;

typedef struct {
    Block* block;
    int x, y; // top-left corner of block in field coordinates
} ActiveBlock;

extern const Block I;
extern const Block O;
extern const Block T;
extern const Block S;
extern const Block Z;
extern const Block J;
extern const Block L;
