// #define _GNU_SOURCE
// #include <fenv.h>
#include "thread.h"

int main(int argc, const char* argv[]) {
    const int p = argc - 1;
    int k;
    struct Args * a;
    pthread_t* tids;
    // feenableexcept(FE_ALL_EXCEPT & ~FE_INEXACT);
    if (argc == 1) {
        printf("Usage: %s <files>\n", argv[0]);
        return -1;
    }

    a    = (struct Args*)malloc(sizeof(struct Args) * p);
    tids = (pthread_t*)  malloc(sizeof(pthread_t) * p);
    if (!a || !tids) {
        free(a);
        free(tids);
        printf("Cannot allocate memory\n");
        return -2;
    }

    for (k = 1; k <= p; k++) {
        a[k - 1].filename = argv[k];
        a[k - 1].k = k;
        a[k - 1].p = p;
        a[k - 1].length = 0;
        a[k - 1].mean = 0.;
        a[k - 1].result = 0;
        a[k - 1].error  = 0;
    }
    for (k = 1; k < p; k++) {
        if (pthread_create(tids + k, 0, &thread_func, a + k)) {
            printf("Cannot create thread %d\n", k);
        }
    }
    thread_func(a + 0);

    if (a[0].error == 0) {
        printf("Result: %f\n", a[0].mean);
    } else {
        free(a);
        free(tids);
        return -1;
    }
    free(a);
    free(tids);
    return 0;
}
