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
    return 0;
}
