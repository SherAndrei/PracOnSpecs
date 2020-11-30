#pragma once
#include <pthread.h>
#include "utils.h"

struct Range {
    double* begin;
    double* end;
};

struct Args {
    int k;
    int p;
    double time;
    struct Args* prev;
    struct Range range;
    struct Args* next;
    int result;
};

void* thread_func(void* ptr);
