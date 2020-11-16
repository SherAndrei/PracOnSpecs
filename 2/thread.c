#include <stdlib.h>
#include <pthread.h>
#include "utils.h"

void* thread_func(void * ptr);

struct Args {
    const char* filename;
    int k;
    int p;
    struct Info info;
    int error;
    int result;
};

void* thread_func(void * ptr) {
    static pthread_mutex_t m    = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t cond  = PTHREAD_COND_INITIALIZER;
    static int t_in  = 0;
    static int t_out = 0;
    static int length = 0;
    static int result = 0;
    static double max_const = 0.;

    struct Args *a = (struct Args *) ptr;
    FILE* file = fopen(a->filename, "r");
    if (!file) {
        printf("Error opening file %s in thread %d out of %d\n", a->filename, a->k, a->p);
        a->error = -1;
    }
    if (!a->error && find_max_constant_sequence(file, &(a->info)) < 0) {
        printf("Incorrect data in %s in thread %d out of %d\n"
               "\twhile searching max const\n", a->filename, a->k, a->p);
        a->error = -2;
        fclose(file);
    }
    if (!a->error)
        rewind(file);
    // Сохраняем длину и максимальное значение
    /////////////////////////////////
    pthread_mutex_lock(&m);
    if (!a->error && length <= a->info.length) {
       length    = a->info.length;
       max_const = a->info.max_const;
    }
    t_in++;
    if (t_in >= a->p) {
        t_out = 0;
        pthread_cond_broadcast(&cond);
    } else {
        while (t_in < a->p) {
            pthread_cond_wait(&cond, &m);
        }
    }
    t_out++;
    if (t_out >= a->p) {
        t_in = 0;
        pthread_cond_broadcast(&cond);
    } else {
        while (t_out < a->p) {
            pthread_cond_wait(&cond, &m);
        }
    }
    pthread_mutex_unlock(&m);
    //////////////////////////////////////////

    if (!a->error && (a->info.length > 0)
                  && find_values_greater_than_max_const(file, max_const, &(a->result)) < 0) {
        printf("Incorrect data in %s in thread %d out of %d\n"
               "\twhile searching greater values\n", a->filename, a->k, a->p);
        a->error = -2;
        fclose(file);
    }

    // Сохраняем результат в первый
    ///////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (!a->error)
        result += a->result;
    t_in++;
    if (t_in >= a->p) {
        t_out = 0;
        pthread_cond_broadcast(&cond);
    } else {
        while (t_in < a->p) {
            pthread_cond_wait(&cond, &m);
        }
    }
    if (!a->error)
        a->result = result;
    t_out++;
    if (t_out >= a->p) {
        t_in = 0;
        pthread_cond_broadcast(&cond);
    } else {
        while (t_out < a->p) {
            pthread_cond_wait(&cond, &m);
        }
    }
    pthread_mutex_unlock(&m);
    ///////////////////////////////////////////
    if (!a->error)
        fclose(file);
    return 0;
}

int main(int argc, const char* argv[]) {
    const int p = argc - 1;
    int k;
    struct Args * a;
    pthread_t* tids;

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
        a[k - 1].info.length = 0;
        a[k - 1].info.max_const = 0.;
        a[k - 1].result = 0;
        a[k - 1].error = 0;
    }
    for (k = 0; k < p; k++) {
        if (pthread_create(tids + k, 0, &thread_func, a + k)) {
            printf("Cannot create thread %d\n", k);
        }
    }
    for (k = 0; k < p; k++) {
        pthread_join(tids[k], 0);
    }
    if (a[0].error == 0) {
        printf("Result: %d\n", a[0].result);
    } else {
        free(a);
        free(tids);
        return -1;
    }
    free(a);
    free(tids);
    return 0;
}
