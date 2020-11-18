#pragma once
#include <pthread.h>
#include <stdlib.h>
#include "utils.h"

struct FileInfo {
    const char* name;
    double first;
    double last;
    int length;
    int error;
};

struct Args {
    int k;
    int p;
    struct FileInfo* info;
    int all_len;
    int result;
    int error;
};

void* thread_func(void* ptr);

