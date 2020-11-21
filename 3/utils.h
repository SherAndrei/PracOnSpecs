#pragma once
#include <stdio.h>
#include "fileinfo.h"

int find_local_min(FILE* file, const struct FileInfo* last,
                               const struct FileInfo* current,
                               int* p_res);
int find_mean(FILE* file, int* p_len, double* p_mean);
