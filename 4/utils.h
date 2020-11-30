#pragma once

struct Array {
    double* begin;
    int length;
};

void formula(struct Array array);
int fill_array(struct Array array, const char* filename);
void print_array(const struct Array array);

double get_time();
double get_full_time();
