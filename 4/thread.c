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
    double* begin  = arg->array.begin;
    int length = arg->array.length;
    int other_length = 0;
    double prevprev = -1.,
           prev = -1.,
           next = -1.,
           nextnext = -1.;
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
    other_length = 0;
    if (k != p - 1)
        other_length = arg->next->array.length;
    if (k < p - 2) {
        if (length > 1) {
            next     = *(arg->next->array.begin + 0);
            nextnext = *(arg->next->array.begin + 1);
        } else {
            next     = *(arg->next->array.begin + 0);
            if (other_length > 1) {
                nextnext = *(arg->next->array.begin + 1);
            } else {
                nextnext = *(arg->next->next->array.begin);
            }
        }
    } else if (k == p - 2) {
        next     = *(arg->next->array.begin + 0);
        if (other_length > 1) {
            nextnext = *(arg->next->array.begin + 1);
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

    if (length > 3) {
        i = (k != 0) ? 0 : 2;
        for (; i + 2 < length; i++) {
            evaluate(&prevprev, &prev, begin + i, begin + i + 2);
        }
        if (k != p - 1) {
            evaluate(&prevprev, &prev, begin + i + 0, &next);
            evaluate(&prevprev, &prev, begin + i + 1, &nextnext);
            i += 2;
        }
        i = (k != 0) ? i : i - 2;
    } else if (length == 3) {
        if (k == 0 && k != p - 1) {
            evaluate(&prevprev, &prev, begin + 2, &nextnext);
            i = 1;
        } else if (k > 0) {
            evaluate(&prevprev, &prev, begin + 0, begin + 2);
            i = 1;
            if (k != p - 1) {
                evaluate(&prevprev, &prev, begin + 1, &next);
                evaluate(&prevprev, &prev, begin + 2, &nextnext);
                i += 2;
            }
        }
    } else if (length == 2) {
        if (k != 0 && k != p - 1) {
            evaluate(&prevprev, &prev, begin + 0, &next);
            evaluate(&prevprev, &prev, begin + 1, &nextnext);
            i = 2;
        }
    } else {
        if (k > 1 &&
           (k < p - 2 || (k == p - 2 && other_length > 1))) {
            evaluate(&prevprev, &prev, begin + 0, &nextnext);
            i++;
        }
    }
    arg->result = i;

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
