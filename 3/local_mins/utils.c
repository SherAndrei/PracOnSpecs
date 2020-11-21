#include "utils.h"

int find_local_min(FILE* file, const struct FileInfo* last_info,
                               int* p_res) {
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
           !last_info->is_increasing) {
            amount--;
        }
        is_increasing = last_info->last < last;
    }

    while (fscanf(file, "%lf", &current) == 1) {
        if (last <= current) {
            if (!is_increasing)
                amount++;
        }
        is_increasing = last < current;
        last = current;
    }

    if (!is_increasing)
        amount++;

    if (!feof(file))
        return -1;

    *p_res = amount;
    return 0;
}
