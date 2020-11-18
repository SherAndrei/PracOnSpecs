// #define _GNU_SOURCE
// #include <fenv.h>
#include "thread.h"

int main(int argc, const char* argv[]) {
    const int p = argc - 1;
    int k;
    struct Args * a;
    pthread_t* tids;
    struct FileInfo * files;
    // feenableexcept(FE_ALL_EXCEPT & ~FE_INEXACT);
    if (argc == 1) {
        printf("Usage: %s <files>\n", argv[0]);
        return -1;
    }
    files = (struct FileInfo*)malloc(sizeof(struct FileInfo) * p);
    a     = (struct Args*)malloc(sizeof(struct Args) * p);
    tids  = (pthread_t*)  malloc(sizeof(pthread_t) * p);
    if (!a || !tids) {
        free(a);
        free(tids);
        printf("Cannot allocate memory\n");
        return -2;
    }

    for (k = 0; k < p; k++) {
        a[k].k = k + 1;
        a[k].p = p;
        files[k].name = argv[k + 1];
        files[k].first    = 0.;
        files[k].last     = 0.;
        files[k].length   = 0;
        files[k].error    = 0;
        a[k].info = files + k;
        a[k].all_len = 0;
        a[k].result = 0;
        a[k].error  = 0;
    }
    for (k = 1; k < p; k++) {
        if (pthread_create(tids + k, 0, &thread_func, a + k)) {
            printf("Cannot create thread %d\n", k);
        }
    }
    thread_func(a + 0);

    for (k = 1; k < p; k++) {
        pthread_join(tids[k], 0);
    }

    if (a[0].error == 0) {
        for (k = 0; k < p; k++) {
            if (files[k].length > 0)
                printf(" %f %f", files[k].first, files[k].last);
        }
        putchar('\n');
    } else {
        free(files);
        free(a);
        free(tids);
        return -1;
    }
    free(files);
    free(a);
    free(tids);
    return 0;
}
