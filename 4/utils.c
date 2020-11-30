#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "utils.h"

#define PRINT_VALUE 20
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void formula(double* array, int size) {
    int i;
    for (i = 0; i < size; i++) {
        array[i] = i % 5;
    }
}

int fill_array(double* array, int size, const char* filename) {
    int i;
    if (!filename) {
        formula(array, size);
    } else {
        FILE* file = fopen(filename, "r");
        if (!file) {
            printf("Cannot open file: %s\n", filename);
            return -1;
        }
        for (i = 0;
             i < size && fscanf(file, "%lf", &(array[i])) == 1;
             i++) {}
        if (i < size) {
            printf("Incorrect data in file: %s\n", filename);
            return -1;
        }
        fclose(file);
    }
    return 0;
}

void print_array(const double* array, int size) {
    int i;
    for (i = 0; i < MIN(size, PRINT_VALUE); i++) {
        printf(" %f", array[i]);
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
