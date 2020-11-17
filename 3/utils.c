#include "utils.h"

int fill_mean_info(FILE* file, struct MeanInfo* p_mean) {
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
    p_mean->length = length;
    p_mean->sum = sum;
    return 0;
}
