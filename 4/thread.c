#include "thread.h"

static void evaluate(double* prevprev, double *prev, double* current, double* nextnext) {
    double current_;
    current_  = *current;
    *current  = (*prevprev + *nextnext) / 2;
    *prevprev = *prev;
    *prev     = current_;
}

void* thread_func(void* ptr) {
    double thread_time;
    static pthread_mutex_t m     = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in   = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out  = PTHREAD_COND_INITIALIZER;
    static int t_in  = 0;
    static int t_out = 0;
    static struct Args * first = 0;

    struct Args * arg   = (struct Args*)ptr;
    struct Range* arg_r = arg->current;
    int length = arg_r->end - arg_r->begin;
    double prevprev = 0.,
           prev = 0.;
        //    before_last = 0.,
        //    last = 0.;
    int result = 0;
    int k = 0;
    double* array = arg_r->begin;

    thread_time = get_time();

    ////////////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (length > 3) {
        if (arg->next != 0) {
            k = arg->next->end - arg->next->begin;
            prevprev = array[length - 4];
            prev     = array[length - 3];

            evaluate(&prevprev, &prev, array + length - 2, arg->next->begin);
            result++;
            if (k > 1) {
                evaluate(&prevprev, &prev, array + length - 1, arg->next->begin + 1);
                result++;
            }
        }

        if (arg->prev != 0) {
            prevprev = *(arg->prev->end - 2);
            prev     = *(arg->prev->end - 1);

            evaluate(&prevprev, &prev, array + 0, array + 2);
            evaluate(&prevprev, &prev, array + 1, array + 3);

            result += 2;
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
        // first = 0;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < arg->p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
    ////////////////////////////////////////////////

    if (length > 2) {
        prevprev = array[0];
        prev     = array[1];
    }
    for (k = 2; k + 2 < length; k++) {
        evaluate(&prevprev, &prev, array + k, array + k + 2);
    }
    arg->result += k - 2 + result;

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
