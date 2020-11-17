#pragma once
#include <pthread.h>
#include <stdlib.h>
#include "utils.h"

struct Args {
    const char* filename;
    int k;
    int p;
    struct MeanInfo info;
    double result;
    int error;
};

void* thread_func(void* ptr);

