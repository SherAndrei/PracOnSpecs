#pragma once
#include <stdio.h>

struct Info {
    int length;
    double max_const;
};

int find_max_constant_sequence(FILE* file, struct Info* p_inf);
int find_values_greater_than_max_const(FILE* file, double max_const, int* amount);
