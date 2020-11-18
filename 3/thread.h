#pragma once
#include <pthread.h>
#include <stdlib.h>
#include "utils.h"

struct Args {
    const char* filename;
    int k;
    int p;
    int length;
    double mean;
    int result;
    int error;
};

void* thread_func(void* ptr);

