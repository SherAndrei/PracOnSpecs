#pragma once
#include <stdio.h>

struct FileInfo {
    int error;
    double last;
    int length;
    int is_increasing;
};

int fill_info_and_mean(FILE* file, struct FileInfo * current, double* p_mean);
int find_local_min_less_than_mean(FILE* file, const struct FileInfo* last,
                               int* p_res, double mean);
