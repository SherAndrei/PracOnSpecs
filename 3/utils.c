#include "utils.h"

int find_mean(FILE* file, int* p_len, double *p_mean) {
    int length = 0;
    double sum = 0.;
    double current = 0.;
    while (fscanf(file, "%lf", &current) == 1) {
        sum += current;
        length++;
    }
    if (!feof(file)) {
        return -1;
    }
    *p_len = length;
    if (length != 0)
        *p_mean = sum / length;

    rewind(file);
    return 0;
}

int fill_first_and_last(FILE* file, double* p_first, double* p_last, int* p_len) {
    double first, last;
    int length = 0;
    if (fscanf(file, "%lf", &first) != 1) {
        if (!feof(file))
            return -1;
        *p_len = 0;
        *p_first = *p_last = 0.;
        return 0;
    }
    do {
        length++;
    } while (fscanf(file, "%lf", &last) == 1);
    if (length == 1) {
        last = first;
    }
    if (!feof(file))
        return -1;

    *p_first = first;
    *p_last  = last;
    *p_len   = length;
    return 0;
}

// int find_local_min(FILE* file, int* p_res, double last, double next) {

// }
