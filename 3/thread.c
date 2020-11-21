#include <stdlib.h>
#include "thread.h"

#define LOG_INT(x) printf("%s: %d\n", #x, x)
#define LOG_DBL(x) printf("%s: %f\n", #x, x)

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
        fill_info(file, cur->current) < 0) {
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
        cur->all_len = first->all_len;
    }
    if (cur->error == 0 && cur->current->length == 0) {
        for (iter = cur->current; iter >= cur->begin; iter--) {
            if (iter->length != 0) {
                // cur->current->is_decreasing = iter->is_decreasing;
                // cur->current->last = iter->last;
                cur->current = iter;
            }
        }
        if (iter == cur->begin && iter->length == 0) {
            cur->last = 0;
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
        find_local_min(file, cur->last, cur->current, &(cur->result)) < 0) {
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
