#include "thread.h"

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

