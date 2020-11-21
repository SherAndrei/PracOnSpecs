#pragma once
#include <stdio.h>

struct FileInfo {
    int error;
    double last;
    int length;
    int is_increasing;
};

int fill_info(FILE* file, struct FileInfo * current);
