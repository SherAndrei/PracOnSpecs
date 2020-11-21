#pragma once
#include <stdio.h>
#include "fileinfo.h"

int find_local_min(FILE* file, const struct FileInfo* last,
                               int* p_res);
