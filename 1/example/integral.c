#include "integral.h"

/* Интегрируемая функция */
static double f (double x)
{
  return x;
}

/* Вычислить интеграл по отрезку [a, b] с числом точек
   разбиения n методом трапеций. */
double integrate (double a, double b, int n)
{
  double res;   /* результат */
  double h;     /* шаг интегрирования */
  int i;

  h = (b - a) / n;
  res = 0.5 * (f (a) + f (b)) * h;
  for (i = 1; i < n; i++)
    res += f (a + i * h) * h;
  return res;
}
