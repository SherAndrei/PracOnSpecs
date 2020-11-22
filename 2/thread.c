#include "thread.h"

void* thread_func(void * ptr) {
    static pthread_mutex_t m    = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in   = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out  = PTHREAD_COND_INITIALIZER;
    static int t_in  = 0;
    static int t_out = 0;
    static struct Args * first = 0;

    struct Args * cur = (struct Args *) ptr;
    FILE * file = fopen(cur->filename, "r");
    if (!file) {
        printf("Cannot open file %s\n", cur->filename);
        cur->error = -1;
    }
    if (cur->error == 0 &&
        find_max_constant_sequence(file, &(cur->info)) < 0) {
        printf("Incorrect data in file %s\n", cur->filename);
        cur->error = -2;
        fclose(file);
        file = 0;
    }
    //////////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (first == 0) {
        first = cur;
    } else {
        if (cur->error != 0) {
           first->error = cur->error;
        } else if (first->info.length < cur->info.length ||
                  ((first->info.length == cur->info.length) &&
                   (first->info.max_const < cur->info.max_const))) {
           first->info.length    = cur->info.length;
           first->info.max_const = cur->info.max_const;
        }
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
    if (first != cur) {
        cur->info  = first->info;
        cur->error = first->error;
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
    //////////////////////////////////////////////
    if (cur->error != 0) {
        if (file != 0) {
            fclose(file);
        }
        return 0;
    }

    rewind(file);
    if (cur->info.length > 0 &&
        find_values_greater_than_max_const(file, cur->info.max_const, &(cur->result)) < 0) {
        printf("Incorrect data in file %s\n", cur->filename);
        cur->error = -2;
    }
    //////////////////////////////////////////////
    pthread_mutex_lock(&m);
    if (first == 0) {
        first = cur;
    } else {
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
    if (first != cur) {
        cur->result = first->result;
    }
    t_out++;
    if (t_out >= cur->p) {
        t_in = 0;
        first = 0;
        pthread_cond_broadcast(&c_out);
    } else {
        while (t_out < cur->p) {
            pthread_cond_wait(&c_out, &m);
        }
    }
    pthread_mutex_unlock(&m);
    //////////////////////////////////////////////
    fclose(file);
    return 0;
}

