#pragma once
#include <pthread.h>
#include "utils.h"

struct Args {
    int k;
    int p;
    double time;
    struct Args* prev;
    double prevprev_;
    double prev_;
    double next_;
    double nextnext_;
    struct Array array;
    struct Args* next;
    int result;
};

void* thread_func(void* ptr);
