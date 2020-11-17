#pragma once
#include <stdio.h>

struct MeanInfo {
    int length;
    double sum;
};

int fill_mean_info(FILE* file, struct MeanInfo* p_mean);
