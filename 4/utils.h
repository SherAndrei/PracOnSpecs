#pragma once
#include <stdio.h>

#define LOG_DBL(x) printf("%s: %f\n", #x, x)
#define LOG_INT(x) printf("%s: %d\n", #x, x)

struct Array {
    double* begin;
    int length;
};

void formula(struct Array array);
int fill_array(struct Array array, const char* filename);
void print_array(const struct Array array);

double get_time();
double get_full_time();
