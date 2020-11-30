#include "thread.h"

static void evaluate_left(double* prevprev, double *prev, double* current) {
    double current_;
    current_  = *current;
    *current  = *prevprev / 2;
    *prevprev = *prev;
    *prev     = current_;
}

// static void evaluate(double* prevprev, double *prev, double* current, double* nextnext) {
//     double current_;
//     current_  = *current;
//     *current  = (*prevprev + *nextnext) / 2;
//     *prevprev = *prev;
//     *prev     = current_;
// }

void* thread_func(void* ptr) {
    double thread_time;
    static pthread_mutex_t m     = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in   = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out  = PTHREAD_COND_INITIALIZER;
    static int t_in  = 0;
    static int t_out = 0;
    static struct Args * first = 0;

    struct Args * arg   = (struct Args*)ptr;
    double* begin  = arg->array.begin;
    int length = arg->array.length;
    int other_length = 0;
    double prevprev = 0.,
           prev = 0.,
           next = 0.,
           nextnext = 0.;
    int k = arg->k;
    int p = arg->p;
    int i = 0;
    thread_time = get_time();

    ////////////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (k == 0) {
        if (length > 1) {
            prevprev = begin[0];
            prev     = begin[1];
        }
    } else {
        other_length = arg->prev->array.length;
        if (length > 1) {
            prevprev = arg->prev->array.begin[other_length - 2];
            prev     = arg->prev->array.begin[other_length - 1];
        } else if (length == 1) {
            if (k > 1)
                prevprev = arg->prev->prev->array.begin[other_length - 1];
            prev     = arg->prev->array.begin[other_length - 1];
        }
    }

    t_in++;
    if (t_in >= arg->p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_in < arg->p) {
            pthread_cond_wait(&c_in, &m);
        }
    }

    t_out++;
    if (t_out >= arg->p) {
        t_in  = 0;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < arg->p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
    ////////////////////////////////////////////////

    if (length != 1) {
        i = (k != 0) ? 0 : 2;
        for (; i < length; i++) {
            evaluate_left(&prevprev, &prev, begin + i);
        }
        i = (k != 0) ? i : i - 2;
    } else {
        if (k > 1)
            evaluate_left(&prevprev, &prev, begin + 0);
        i = (k > 1);
    }
    arg->result += i;

    ////////////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (first == 0) {
        first = arg;
    } else {
        first->result += arg->result;
    }
    t_in++;
    if (t_in >= arg->p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_in < arg->p) {
            pthread_cond_wait(&c_in, &m);
        }
    }
    if (first != arg) {
        arg->result = first->result;
    }
    t_out++;
    if (t_out >= arg->p) {
        t_in  = 0;
        first = 0;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < arg->p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
    ////////////////////////////////////////////////
    arg->time = get_time() - thread_time;
    return 0;
}
