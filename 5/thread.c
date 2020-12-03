#include "thread.h"

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
    double prev = -1.,
           next = -1.,
           nextnext = -1.;
    int k = arg->k;
    int p = arg->p;
    thread_time = get_time();

    ////////////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (k == 0) {
        if (length > 1) {
            prev     = begin[0];
        }
    } else {
        other_length = arg->prev->array.length;
        prev         = arg->prev->array.begin[other_length - 1];
    }
    other_length = 0;
    if (k != p - 1) {
        other_length = arg->next->array.length;
        next         = *(arg->next->array.begin + 0);
        if (length > 1 || other_length > 1) {
            nextnext = *(arg->next->array.begin + 1);
        } else if (length == 1 && k != p - 2) {
            nextnext = *(arg->next->next->array.begin);
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

    find_mean(arg, prev, next, nextnext, other_length);

    ////////////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (first == 0) {
        first = arg;
    } else {
        // result represents amount of desired elements
        if (first->result + arg->result != 0) {
            first->mean = (first->mean * first->result
                           + arg->mean * arg->result) / (first->result + arg->result);
        }
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
        arg->mean   = first->mean;
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

    replace_elements_with_mean(arg, prev, next, nextnext, other_length);

    arg->time = get_time() - thread_time;
    return 0;
}
