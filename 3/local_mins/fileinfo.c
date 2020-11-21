#include "fileinfo.h"

int fill_info(FILE* file, struct FileInfo* current) {
    double first, last;
    int i = 0;
    int is_increasing = 0;
    if (fscanf(file, "%lf", &first) != 1) {
        if (!feof(file))
            return -1;
        current->length = 0;
        current->last = 0.;
        current->is_increasing = 0;
        return 0;
    }
    for (i = 1; fscanf(file, "%lf", &last) == 1; i++) {
        is_increasing = (first < last);
    }
    if (i == 1) {
        last = first;
    }
    if (!feof(file))
        return -1;

    current->is_increasing = is_increasing;
    current->last = last;
    current->length = i;
    return 0;
}
