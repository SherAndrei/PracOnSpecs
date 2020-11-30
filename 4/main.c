#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "thread.h"

int main(int argc, const char* argv[]) {
    int p, n, k, length;
    double* array;
    double fulltime;
    struct Args * a;
    pthread_t* tids;
    // struct Range* ranges;
    if (!((argc == 3 || argc == 4) &&
        (sscanf(argv[1], "%d", &n) == 1) &&
        (sscanf(argv[2], "%d", &p) == 1) &&
        n > 0 && p > 0)) {
        printf("Usage: %s <n> <p> [filename]\n", argv[0]);
        return -1;
    }
    if (n < p)
        p = n;
    length = n / p;

    array  = (double*)      malloc(n *sizeof(double));
    a      = (struct Args*) malloc(sizeof(struct Args) * p);
    // ranges = (struct Range*)malloc(sizeof(struct Range) * p);
    tids   = (pthread_t*)   malloc(sizeof(pthread_t) * p);
    if (!array || !a || !tids) {
    // || !ranges) {
        free(array), free(a), free(tids);
        //  free(ranges);
        printf("Cannot allocate memory\n");
        return -1;
    }

    if (fill_array(array, n, (argc == 4 ? argv[3] : NULL)) < 0) {
        free(array), free(a), free(tids);
        // free(ranges);
        return -2;
    }
    printf("Original =");
    print_array(array, n);

    for (k = 0; k < p; k++) {
        // ranges[k].begin = array + k * length;
        // ranges[k].end   = array + ((k != p - 1) ? (k + 1) * length : n);
        a[k].prev        = a + k - 1;
        a[k].range.begin = array + k * length;
        a[k].range.end   = array + ((k != p - 1) ? (k + 1) * length : n);
        a[k].next        = a + k + 1;
        a[k].k      = k;
        a[k].p      = p;
        a[k].time   = 0.;
        a[k].result = 0;
    }
    a[0].prev = 0;
    a[p - 1].next = 0;

    fulltime = get_full_time();

    for (k = 1; k < p; k++) {
        if (pthread_create(tids + k, 0, &thread_func, a + k)) {
            printf("Cannot create thread %d\n", k);
        }
    }
    thread_func(a + 0);

    for (k = 1; k < p; k++) {
        pthread_join(tids[k], 0);
    }

    fulltime = get_full_time() - fulltime;

    printf("Result   =");
    print_array(array, n);
    printf("Answer: %d\n", a[0].result);
    printf("####################\n");
    printf("Full time: %.4f\n", fulltime);
    printf("--------------------\n");
    for (k = 0; k < p; k++) {
        printf("%d thread took %.4f\n", k + 1, a[k].time);
        printf("--------------------\n");
    }
    free(array), free(a), free(tids);
    // free(ranges);
    return 0;
}
