#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// делаем барьеры вручную

// ожидается, что при вызове reduce_sum(p)
// будем ждать, пока все ее не вызовут
// по-другому называется барьер

// при вызове reduce_sum(p, &res, 1);
// 1.дождаться
// 2.получить в res сумму по всем потокам

// идея
// дождаться пока войдут все p
// какая-то логика
// дождаться пока все выйдут
//

void reduce_sum(int p, double *a = NULL, int n = 0) {
    // статик, чтобы была глобальная жизнь
    static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
    // оповещать, что достиглось количество входящик
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    // оповещать, что достиглось количество выходящик
    // static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    // количество вошедших и вышедших потоков
    static int t_in  = 0;
    static int t_out = 0;
    // указатель на массив от первого входящего потока
    static double* p_a = 0;
    int i;
    if (p <= 1) return;
    pthread_mutex_lock(&m);
    if (!p_a) {
        // если мы зашли первыми, то ставим указатель на первый
        // элемент в массиве
        p_a = a;
    } else {
        // ответ образуется в памяти первого потока
        for (i = 0; i < n; i++) {
            p_a[i] += a[i];
        }
    }
    t_in++;
    // дождалис условия, что пришел последний поток
    if (t_in >= p) {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        // отсальные находятся в состоянии ожидания до тех пор
        // пока количество вошедших меньше p
        while (t_in < p) {
            pthread_cond_wait(&c_in, &m);
        }
        // даже если дождались, то двигаться не будут так как на данный момент
        // главный владеет mutexом
    }
    // выход
    if (p_a != a) {
        // то это значит, что мы не в первом потоке,
        // значит надо скопировать ответ
        for (i = 0; i < n; i++)
            a[i] = p_a[i];
    }
    t_out++;
    // аналогично входу
    // если выполняется это условие, то значит работает последний поток
    if (t_out >= p) {
        t_in = 0;
        p_a  = 0;
        pthread_cond_broadcast(&c_in);
    } else {
        while (t_out < p) {
            pthread_cond_wait(&c_in, &m);
        }
    }
    pthread_mutex_unlock(&m);
}

struct Args {
    int p;
    int k;
    double* p_ar;
    int n;
};

void* thread_func(void * ptr) {
    Args* a = (Args*) ptr;
    reduce_sum(a->p, a->p_ar, a->n);
    return 0;
}

constexpr int THREAD_AMOUNT = 10;
constexpr int ARR_SIZE = 100;
int main() {
    int k = 0;
    int length = ARR_SIZE / THREAD_AMOUNT;
    double ar[ARR_SIZE];
    Args * a;
    pthread_t* tids;
    for (k = 0; k < ARR_SIZE; k++) {
        ar[k] = (double) k;
    }

    a    = (Args*)malloc(sizeof(Args) * THREAD_AMOUNT);
    tids = (pthread_t*)malloc(sizeof(pthread_t) * THREAD_AMOUNT);
    if (!a || !tids) {
        free(a);
        free(tids);
        printf("Cannot allocate memory\n");
        return -2;
    }

    for (k = 0; k < THREAD_AMOUNT; k++) {
        a[k].k    = k;
        a[k].p    = THREAD_AMOUNT;
        a[k].n    = length;
        a[k].p_ar = ar + k * length;
        if (pthread_create(tids + k, 0, &thread_func, a + k)) {
            printf("Cannot create thread %d\n", k);
        }
    }

    for (k = 0; k < THREAD_AMOUNT; k++) {
        pthread_join(tids[k], 0);
    }
    for (k = 0; k < ARR_SIZE; k++) {
        if (k % length == 0)
            putchar('\n');
        printf(" %f", ar[k]);
    }
    putchar('\n');
    return 0;
}
