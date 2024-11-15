/**
 * Данный модуль содержит функции ввода.
 */
#ifndef __MY_INPUT_H__
#define __MY_INPUT_H__

#include "my_model.h"

/**
 * Чтение целого беззнакового числа.
 * Принимает на вход указатель на считываемое число, а также минимальное и максимальное значения.
 * Корректность чисел и указателя не проверяется.
 */
void read_num(int *val, int minval, int maxval);

/**
 * Чтение параметров моделирования.
 * Принимает на вход указатель на структуру, содержащую времена моделирования.
 * Корректность указателя не проверяется.
 */
void read_params(params_t *times);

#endif //__MY_INPUT_H__