#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "thread.h"

int main(int argc, const char* argv[]) {
    int p, k, length;
    struct Array main;
    double fulltime;
    struct Args * a;
    pthread_t* tids;
    if (!((argc == 3 || argc == 4) &&
        (sscanf(argv[1], "%d", &main.length) == 1) &&
        (sscanf(argv[2], "%d", &p) == 1) &&
        main.length > 0 && p > 0)) {
        printf("Usage: %s <n> <p> [filename]\n", argv[0]);
        return -1;
    }
    if (main.length < p)
        p = main.length;
    length = main.length / p;

    main.begin = (double*)  malloc(main.length *sizeof(double));
    a      = (struct Args*) malloc(sizeof(struct Args) * p);
    tids   = (pthread_t*)   malloc(sizeof(pthread_t) * p);
    if (!main.begin || !a || !tids) {
        free(main.begin), free(a), free(tids);
        printf("Cannot allocate memory\n");
        return -1;
    }

    if (fill_array(main, (argc == 4 ? argv[3] : NULL)) < 0) {
        free(main.begin), free(a), free(tids);
        return -2;
    }
    printf("Original =");
    print_array(main);
    for (k = 0; k < p; k++) {
        a[k].prev        = a + k - 1;
        a[k].array.begin  = main.begin + k * length;
        a[k].array.length = ((k != p - 1) ? length : (main.length - length * (p - 1)));
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
    print_array(main);
    printf("Answer: %d\n", a[0].result);
    printf("####################\n");
    printf("Full time: %.4f\n", fulltime);
    printf("--------------------\n");
    for (k = 0; k < p; k++) {
        printf("%d thread took %.4f\n", k + 1, a[k].time);
        printf("--------------------\n");
    }
    free(main.begin), free(a), free(tids);
    return 0;
}
