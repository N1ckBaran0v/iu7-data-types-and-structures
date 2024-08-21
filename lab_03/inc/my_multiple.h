/**
 * Данный модуль содержит функции умножения строки на матрицу.
 */
#ifndef __MY_MULTIPLE_H__
#define __MY_MULTIPLE_H__

#include "my_structs.h"

/**
 * Обычное умножение.
 * На вход принимает 3 указателя: на результат, на вектор, на матрицу.
 * Корректность указателей не проверяется.
 */
void mult_usual(us_m *dst, const us_m *vector, const us_m *matrix);

/**
 * Умножение для разреженных матриц.
 * На вход принимает 3 указателя: на результат, на вектор, на матрицу.
 * Корректность указателей не проверяется.
 */
void mult_special(sp_m *dst, const sp_m *vector, const sp_m *matrix);

#endif // __MY_MULTIPLE_H__