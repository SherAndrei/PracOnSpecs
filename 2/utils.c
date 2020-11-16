#include <math.h>
#include "utils.h"

#define EPS 1e-15

int find_max_constant_sequence(FILE* file, struct Info* p_inf) {
    double last, current;
    int counter = 0;
    int len = 0;
    double max = 0.;
    int is_constant = 0;
    if (fscanf(file, "%lf", &last) != 1) {
        if (feof(file)) {
            p_inf->length    = 0;
            p_inf->max_const = 0;
            return 0;
        } else {
            return -1;
        }
    }
    while (fscanf(file, "%lf", &current) == 1) {
        is_constant = (fabs(last - current) < EPS);
        if (is_constant) {
            counter++;
        } else {
            if (counter > len) {
                len = counter + 1;
                max = last;
            }
            counter = 0;
        }
        last = current;
    }
    if (!feof(file)) {
        return -1;
    }
    if (is_constant) {
        len = counter + 1;
        max = last;
    }
    p_inf->length    = len;
    p_inf->max_const = max;
    return 0;
}

int find_values_greater_than_max_const(FILE* file, double max_const, int* amount) {
    double current;
    int result = 0;

    while (fscanf(file, "%lf", &current) == 1) {
        if (current > max_const) {
            result++;
        }
    }
    if (!feof(file)) {
        return -1;
    }
    *amount = result;
    return 0;
}
