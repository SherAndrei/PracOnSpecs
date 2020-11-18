#pragma once
#include <stdio.h>

// int find_local_min(FILE* file, int* p_res, double last, double next);
int fill_first_and_last(FILE* file, double* p_first, double* p_last, int* p_len);
int find_mean(FILE* file, int* p_len, double* p_mean);
