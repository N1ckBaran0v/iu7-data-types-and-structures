/**
 * Данный модуль сдержит функции для выделения, освобождения, 
 * сокращения памяти для разреженной матрицы, а также функцию копирования.
 */
#ifndef __MY_MEMORY_H__
#define __MY_MEMORY_H__

#include "my_structs.h"

#define MAX_NUM_SIZE 1000

/**
 * Выделение памяти.
 * На вход принимает указатель на матрицу.
 * Возвращает код ошибки.
 * Корректность указателя не проверяется.
 */
int struct_alloc(sp_m *mdata);

/**
 * Сокращение памяти.
 * На вход принимает указатель на матрицу.
 * Возвращает код ошибки.
 * Корректность указателя не проверяется.
 */
int struct_realloc(sp_m *mdata);

/**
 * Копирование структуры.
 * На вход принимает указатели на 2 матрицы.
 * Возвращает код ошибки.
 * Корректность указателей не проверяется.
 */
void struct_cpy(sp_m *dst, const sp_m *src);

/**
 * Освобождение памяти.
 * На вход принимает указатель на матрицу.
 * Если указатели в структуре указывают не на динамическую память, выделенную
 * одной из 3 функций (malloc, callol, realloc), то поведение не определено.
 */
void struct_free(sp_m *mdata);

#endif // __MY_MEMORY_H__