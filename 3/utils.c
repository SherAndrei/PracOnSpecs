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

// FIXME
#define eps 1e-15
double fabs(double);

int find_local_min(FILE* file, const struct FileInfo* last_info,
                               const struct FileInfo* curr_info,
                               int* p_res) {
    int amount = 0;
    int is_first = 1;  // (last_info == 0);
    int is_constant   = 0;  // !is_first ? last_info->is_decreasing : 0;
    int is_increasing = 0;  // !is_first ? last_info->is_decreasing : 0;
    double last       = 0.;  // !is_first ? last_info->last          : 0.;
    double current = 0.;
    // int pos = 0;
    // FIXME
    (void) last_info;
    (void) curr_info;

    if (is_first) {
        if (fscanf(file, "%lf", &last) != 1) {
            if (!feof(file))
                return -1;
            *p_res = 0;
            return 0;
        }
    }

    (void) is_constant;
    (void) is_increasing;

    while (fscanf(file, "%lf", &current) == 1) {
        if (last < current) {
            if (!is_increasing)
                amount++;
            is_constant   = 0;
            is_increasing = 1;
        } else if (fabs(last - current) < eps) {
            if (!is_increasing)
                amount++;
            is_constant   = 1;
            is_increasing = 0;
        } else {
            is_constant   = 0;
            is_increasing = 0;
        }
        last = current;
    }

    if (!is_increasing)
        amount++;

    if (!feof(file))
        return -1;
    *p_res = curr_info->length != 1 ? amount : 1;
    return 0;
}
