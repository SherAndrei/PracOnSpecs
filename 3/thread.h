#pragma once
#include <pthread.h>
#include "utils.h"
#include "fileinfo.h"

struct Args {
    const char* name;
    int k;
    int p;
    struct FileInfo* begin;
    struct FileInfo* last;
    struct FileInfo* current;
    int all_len;
    int result;
    int error;
};

void* thread_func(void* ptr);
