#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>

struct args {
    int p;
    int k;
};

void* thread_func(void * ptr) {
    struct args* a = (struct args*) ptr;
    int k = a->k;
    int p = a->p;
    printf("Hello from thread %d out of %d\n", k, p);
    return 0;
}

int main(int argc, char* argv[]) {
    int p, k;
    struct args * a;
    pthread_t* tids;
    if (argc != 2 || sscanf(argv[1], "%d", &p) != 1 || p <= 0) {
        printf("Usage: %s p\n", argv[0]);
        return -1;
    }

    a    = (struct args*)malloc(p * sizeof(struct args));
    tids = (pthread_t*)  malloc(p * sizeof(pthread_t));
    if (!a || !tids) {
        free(a);
        free(tids);
        printf("Cannot allocate memory\n");
        return -2;
    }

    for (k = 0; k < p; k++) {
        a[k].k = k;
        a[k].p = p;
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
    free(a);
    free(tids);
}
