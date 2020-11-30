#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
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
