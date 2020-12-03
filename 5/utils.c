#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include "utils.h"


#define PRINT_VALUE 20
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void formula(struct Array array) {
    int i;
    for (i = 0; i < array.length; i++) {
        array.begin[i] = i;
    }
}

int fill_array(struct Array array, const char* filename) {
    int i;
    if (!filename) {
        formula(array);
    } else {
        FILE* file = fopen(filename, "r");
        if (!file) {
            printf("Cannot open file: %s\n", filename);
            return -1;
        }
        for (i = 0;
             i < array.length && fscanf(file, "%lf", &(array.begin[i])) == 1;
             i++) {}
        if (i < array.length) {
            printf("Incorrect data in file: %s\n", filename);
            return -1;
        }
        fclose(file);
    }
    return 0;
}

void print_array(const struct Array array) {
    int i;
    for (i = 0; i < MIN(array.length, PRINT_VALUE); i++) {
        printf(" %f", array.begin[i]);
    }
    putchar('\n');
}

double get_time() {
  struct rusage buf;
  getrusage(RUSAGE_THREAD, &buf);
  return (double)buf.ru_utime.tv_sec + (double)buf.ru_utime.tv_usec/1000000.;
}

double get_full_time() {
  struct timeval buf;
  gettimeofday(&buf, 0);
  return (double)buf.tv_sec + (double)buf.tv_usec/1000000.;
}

static double desired_num(double prev, double current, double next, double nextnext) {
    return (prev + current + next + nextnext) / 4;
}

static void reduce_if(double* p_sum, int* p_amount,
                     double prev, double current, double next, double nextnext) {
    if (current < desired_num(prev, current, next, nextnext)) {
        (*p_sum) += current;
        (*p_amount)++;
    }
}

void find_mean(struct Args* cur, double prev, double next, double nextnext, int next_length) {
    double sum = 0.;
    int amount = 0.;
    int i = 0;
    int k = cur->k;
    int p = cur->p;
    double* begin = cur->array.begin;
    int length    = cur->array.length;
    if (k != 0) {
        if ((length >  2 && begin[0] < desired_num(prev, begin[0], begin[1], begin[2])) ||
            (k < p - 1 && length == 2 && begin[0] < desired_num(prev, begin[0], begin[1], next))) {
            sum += begin[0];
            amount++;
        }
    }

    for (i = 1; i + 2 < length; i++) {
        reduce_if(&sum, &amount, begin[i - 1], begin[i], begin[i + 1], begin[i + 2]);
    }

    if (k != p - 1) {
        if (length > 2) {
            reduce_if(&sum, &amount, begin[i - 1], begin[i], begin[i + 1], next);
            reduce_if(&sum, &amount, begin[i], begin[i + 1], next, nextnext);
        } else if (length == 2) {
            reduce_if(&sum, &amount, begin[i - 1], begin[i], next, nextnext);
        } else if (k != 0 && (k != p - 2 || (k == p - 2 && next_length > 1))) {
            reduce_if(&sum, &amount, prev, begin[0], next, nextnext);
        }
    }
    if (amount != 0)
        cur->mean = sum / amount;
    cur->result = amount;
}

static void replace_if(double mean, double* p_prev, double* current, double next, double nextnext) {
    if (*current < desired_num(*p_prev, *current, next, nextnext)) {
        *p_prev = *current;
        *current = mean;
    }
}

void replace_elements_with_mean(struct Args* cur,
        double prev, double next, double nextnext, int next_length) {
    double mean  = cur->mean;
    int i = 0;
    int k = cur->k;
    int p = cur->p;
    double* begin = cur->array.begin;
    double prev_ = begin[0];
    int length    = cur->array.length;

    if (k != 0) {
        if ((length >  2 && begin[0] < desired_num(prev, begin[0], begin[1], begin[2])) ||
            (k < p - 1 && length == 2 && begin[0] < desired_num(prev, begin[0], begin[1], next))) {
            begin[0] = mean;
        }
    }

    for (i = 1; i + 2 < length; i++) {
        replace_if(mean, &prev_, begin + i, begin[i + 1], begin[i + 2]);
    }

    if (k != p - 1) {
        if (length > 2) {
            replace_if(mean, &prev_, begin + i, begin[i + 1], next);
            replace_if(mean, &prev_, begin + i + 1, next, nextnext);
        } else if (length == 2) {
            replace_if(mean, &prev_, begin + i, next, nextnext);
        } else if (k != 0 && (k != p - 2 || (k == p - 2 && next_length > 1))) {
            prev_ = prev;
            replace_if(mean, &prev_, begin + 0, next, nextnext);
        }
    }
}
