/**
 * Данный модуль содержит общие данные для стеков, а также их объявления.
 */
#ifndef __MY_STACK_H__
#define __MY_STACK_H__

#include <stdint.h>

#include "my_errors.h"

#define TYPE_STACK int64_t
#define MAX_STACK_SIZE 1999

/**
 * Стек на массиве.
 * Содержит размер и указатель на буфер с данными.
 */
typedef struct astack astack;

/**
 * Стек на списке.
 * Содержит размер и указатель на первый лист.
 * Лист содержит значение и указатель на лист - следующий элемент.
 */
typedef struct lstack lstack;

#endif // __MY_STACK_H__
