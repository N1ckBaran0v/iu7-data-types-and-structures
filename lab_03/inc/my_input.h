/**
 * Данный модуль содержит функции чтения структуры.
 */
#ifndef __MY_INPUT_H__
#define __MY_INPUT_H__

#include <stdio.h>

#include "my_structs.h"

#define MAX_NUM_SIZE 1000

/**
 * Чтение одного числа.
 * На вход принимает указатель на число и два числа: минимальное и максимальное значение числа.
 * Возвращает код ошибки.
 * Корректность указателя не проверяется.
 */
int read_num(int *dst, const int minlim, const int maxlim);

/**
 * Чтение вектора-строки и матрицы.
 * На вход принимает указатели на вектор и матрицу.
 * Возвращает код ошибки.
 * Корректность указателей не проверяется.
 */
int read_structs(sp_m *vector, sp_m *mdata);

/**
 * Чтение одной матрицы.
 * На вход принимает указатель на матрицу и режим чтения.
 * Возвращает код ошибки.
 * Корректность указателя не проверяется.
 * Если число лежит в диапазоне от 1 до 3, то запустится вариант ввода в зависимости от числа.
 * Если число лежит в диапазоне от 4 до 104, то запустится генерация значений с (mode - 4)% нулевых элементов.
 * Для других значений поведение не определено.
 */
int read_struct(sp_m *mdata, const int mode);

/**
 * Чтение из файла.
 * На вход принимает указатели на вектор и матрицу и входной поток.
 * Возвращает код ошибки.
 * Корректность указателей не проверяется.
 */
int read_from_file(sp_m *matr, sp_m *vec, FILE *fin);

#endif // __MY_INPUT_H__