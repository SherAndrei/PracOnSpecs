#pragma once
#include <pthread.h>
#include "utils.h"

struct Range {
    double* begin;
    double* end;
};

struct Args {
    int p;
    double time;
    struct Range* prev;
    struct Range* current;
    struct Range* next;
    int result;
};

void* thread_func(void* ptr);
