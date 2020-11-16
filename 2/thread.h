#pragma once
#include <pthread.h>
#include <stdlib.h>
#include "utils.h"

struct Args {
    const char* filename;
    int k;
    int p;
    struct Info info;
    int error;
    int result;
};


void* thread_func(void * ptr);
