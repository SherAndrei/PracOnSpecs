#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "integral.h"

/* Все параметры для простоты задаются константами */
static double a = 0.;   /* левый конец интервала */
static double b = 1.;   /* правый конец интервала */
static int n = 100000000; /* число точек разбиения */

/* Канал вывода из главного процесса в порожденные.
   from_root[0] - для чтения (в порожденных процессах), 
   from_root[1] - для записи (в главном процессе). */
static int from_root[2];

/* Канал вывода из порожденных процессов в главный.
   to_root[0] - для чтения (в порожденных процессах),
   to_root[1] - для записи (в главном процессе). */
static int to_root[2];

/* Функция, работающая в процессе с номером my_rank,
   при общем числе процессов p. */
static void process_function (int my_rank, int p)
{
  char byte;
  /* длина отрезка интегрирования для текущего процесса*/
  double len = (b - a) / p;
  /* число точек разбиения для текущего процесса */
  int local_n = n / p;
  /* левый конец интервала для текущего процесса */
  double local_a = a + my_rank * len;
  /* правый конец интервала для текущего процесса */
  double local_b = local_a + len;
  /* значение интеграла в текущем процессе */
  double integral;

  /* Вычислить интеграл в каждом из процессов */
  integral = integrate (local_a, local_b, local_n);

  /* Ждать сообщения от главного процесса */
  if (read (from_root[0], &byte, 1) != 1)
    {
      /* Ошибка чтения */
      fprintf (stderr, "Error reading in process %d, pid = %d\n",
               my_rank, getpid ());
      return;
    }

  /* Передать результат главному процессу */
  if (write (to_root[1], &integral, sizeof (double)) != sizeof (double))
    {
      /* Ошибка записи */
      fprintf (stderr, "Error writing in process %d, pid = %d\n",
               my_rank, getpid ());
      return;
    }
}

int main (int argc, char * argv[])
{
  /* Идентификатор запускаемого процесса */
  pid_t pid;
  /* Общее количество процессов */
  int p;
  int i;
  char byte;
  double integral = 0.;
  double total = 0.;    /* результат: интеграл */

  /* Получаем количество процессов */
  if (argc != 2 || (p = (int) strtol (argv[1], 0, 10)) <= 0)
    {
      printf ("Usage: %s <instances>\n", argv[0]);
      return 1;
    }

  /* Создаем каналы */
  if (pipe (from_root) == -1 || pipe (to_root) == -1)
    {
      fprintf (stderr, "Cannot pipe!\n");
      return 2;
    }

  /* Запускаем процессы */
  for (i = 0; i < p ; i++)
    {
      /* Клонировать себя */
      pid = fork ();
      if (pid == -1)
        {
          fprintf (stderr, "Cannot fork!\n");
          return 3 + i;
        }
      else if (pid == 0)
        {
          /* Процесс - потомок */
          /* Закрываем ненужные направления обмена */
          close (from_root[1]);
          close (to_root[0]);

          /* Проводим вычисления */
          process_function (i, p);

          /* Закрываем каналы */
          close (from_root[0]);
          close (to_root[1]);
          /* Завершаем потомка */
          return 0;
        }
      /* Цикл продолжает процесс - родитель */
    }

  /* Закрываем ненужные направления обмена */
  close (from_root[0]);
  close (to_root[1]);

  /* Получаем результаты */
  for (i = 0; i < p ; i++)
    {
      /* Сигнализируем процессу */
      byte = (char) i;
      if (write (from_root[1], &byte, 1) != 1)
        {
          /* Ошибка записи */
          fprintf (stderr, "Error writing in root process\n");
          return 100;
        }
      /* Считываем результат */
      if (read (to_root[0], &integral, sizeof (double)) != sizeof (double))
        {
          /* Ошибка чтения */
          fprintf (stderr, "Error reading in root process\n");
          return 101;
        }
      total += integral;
    }

  /* Закрываем каналы */
  close (from_root[1]);
  close (to_root[0]);

  printf ("Integral from %lf to %lf = %.18lf\n", a, b, total);

  return 0;
}
