#include "utils.h"

int fill_info_and_mean(FILE* file, struct FileInfo* cur, double* p_mean) {
    double last, current;
    double sum = 0.;
    int i = 0, is_increasing = 0;
    if (fscanf(file, "%lf", &last) != 1) {
        if (!feof(file))
            return -1;
        return 0;
    }
    sum += last;
    for (i = 1; fscanf(file, "%lf", &current) == 1; ++i) {
        sum += current;
        is_increasing = (last < current);
        last = current;
    }
    if (i == 1) {
        current = last;
    }
    if (!feof(file))
        return -1;

    *p_mean            = sum / ((double) i);
    cur->is_increasing = is_increasing;
    cur->last          = current;
    cur->length        = i;
    return 0;
}


int find_local_min_less_than_mean(FILE* file, const struct FileInfo* last_info,
                               int* p_res, double mean) {
    int amount = 0;
    int is_first = (last_info == 0);
    int is_increasing = 0;
    double last       = 0.;
    double current = 0.;

    if (fscanf(file, "%lf", &last) != 1) {
        if (!feof(file))
            return -1;
        *p_res = 0;
        return 0;
    }

    if (!is_first) {
        if (last_info->last > last &&
           !last_info->is_increasing &&
           last_info->last < mean) {
            amount--;
        }
        is_increasing = last_info->last < last;
    }

    while (fscanf(file, "%lf", &current) == 1) {
        if (last <= current) {
            if (!is_increasing && last < mean)
                amount++;
        }
        is_increasing = last < current;
        last = current;
    }

    if (!is_increasing && last < mean)
        amount++;

    if (!feof(file))
        return -1;

    *p_res = amount;
    return 0;
}
