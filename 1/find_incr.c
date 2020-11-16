#include "find_incr.h"

int find_incr(FILE* inp) {
    double last = 0., current = 0.;
    int is_increasing = 0;
    int counter = 0;

    if (fscanf(inp, "%lf", &last) != 1)
        return -2;

    while (fscanf(inp, "%lf", &current) == 1) {
        if (last < current) {
            is_increasing = 1;
        } else if (is_increasing && !(last < current)) {
            counter++;
            is_increasing = 0;
        }
        last = current;
    }
    if (!feof(inp))
        return -2;
    if (is_increasing)
        counter++;

    return counter;
}
