#include <stdlib.h>
#include "thread.h"

void* thread_func(void* ptr) {
    static pthread_mutex_t m     = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in   = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out  = PTHREAD_COND_INITIALIZER;
    static int t_in  = 0;
    static int t_out = 0;
    static struct Args * first = 0;

    struct Args * cur = (struct Args *) ptr;
    FILE * file = fopen(cur->name, "r");
    struct FileInfo* iter = 0;

    if (!file) {
        printf("Cannot open file %s\n", cur->name);
        cur->current->error = cur->error = -1;
    }
    if (cur->error == 0 &&
        fill_info_and_mean(file, cur->current, &(cur->mean)) < 0) {
        printf("Error in file %s\n", cur->name);
        cur->current->error = cur->error = -2;
    }
    cur->all_len = cur->current->length;
    ////////////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (first == 0) {
        first = cur;
    } else {
        if (cur->error != 0)
            first->error    = cur->error;
        if (first->all_len + cur->all_len != 0) {
            first->mean = (first->mean * first->all_len
                           + cur->mean * cur->all_len) / first->all_len + cur->all_len;
        }
        first->all_len += cur->all_len;
    }
    t_in++;
    if (t_in >= cur->p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_in < cur->p) {
            pthread_cond_wait(&c_in, &m);
        }
    }
    if (cur != first) {
        cur->error   = first->error;
        cur->mean    = first->mean;
        cur->all_len = first->all_len;
    }
    if (cur->error == 0 && cur->last != 0) {
        if (cur->last->length == 0) {
            for (iter = cur->last;
                iter >= cur->begin && iter->length == 0;
                iter--) {}

            if (iter < cur->begin) {
                cur->last = 0;
            } else {
                cur->last = iter;
            }
        } else if (cur->current->length == 1 &&
                   cur->last->length == 1) {
            cur->current->is_increasing = cur->last->last < cur->current->last;
        }
    }
    t_out++;
    if (t_out >= cur->p) {
        t_in  = 0;
        first = 0;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < cur->p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
    ////////////////////////////////////////////////

    if (cur->error < 0) {
        if (cur->current->error != -1)
            fclose(file);
        return 0;
    }

    rewind(file);
    if (cur->current->length != 0 &&
        find_local_min_less_than_mean(file, cur->last, &(cur->result), cur->mean) < 0) {
        printf("Error in file %s\n", cur->name);
        cur->current->error = cur->error = -2;
    }
    //////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (first == 0) {
        first = cur;
    } else {
        if (cur->error != 0)
            first->error    = cur->error;
        first->result += cur->result;
    }
    t_in++;
    if (t_in >= cur->p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_in < cur->p) {
            pthread_cond_wait(&c_in, &m);
        }
    }
    if (cur != first) {
        cur->error   = first->error;
        cur->result  = first->result;
    }
    t_out++;
    if (t_out >= cur->p) {
        t_in  = 0;
        first = 0;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < cur->p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
    //////////////////////////////////////////

    fclose(file);
    return 0;
}
