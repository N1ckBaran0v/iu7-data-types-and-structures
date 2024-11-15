/**
 * Данный модуль содержит функции для подсчета значений.
 */
#ifndef __MY_COUNT_H__
#define __MY_COUNT_H__

#include "my_stack.h"

/**
 * Расчет значения для стека на массиве.
 * На вход принимает указатель на стек на списке и указатель на возвращаемый результат.
 * Возвращает код ошибки.
 * Корректность указателей не проверяется.
 * Размер стека не проверяется.
 */
int count_astack(astack *stack, TYPE_STACK *ans);

/**
 * Расчет значения для стека на списке.
 * Это единственное отличие от предыдущей функции.
 */
int count_lstack(lstack *stack, TYPE_STACK *ans);

#endif // __MY_COUNT_H__
