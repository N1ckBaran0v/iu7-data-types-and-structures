/**
 * Этот модуль содержит функции сортировки и компараторы к ним.
 */
#ifndef __MY_SORTS_H__

#define __MY_SORTS_H__

#include <stddef.h>

typedef int (*f_comp)(const void*, const void*);

/**
 * Сортировка пузырьком.
 * Принимает на вход указатель на массив, размеры массива и элемента и указатель на компаратор.
 * Корректность указателей не проверяется.
 * Корректность размеров не проверяется.
 */
int easy_sort(void *buf, size_t nmemb, size_t size, f_comp comparator);

/**
 * Сортировка слиянием.
 * Входные параметры и ограничения те же что и в предыдущей функции.
 */
int fast_sort(void *buf, size_t nmemb, size_t size, f_comp comparator);

/**
 * Компаратор записей.
 * Принимает на вход два указателя.
 * Корректность указателей не проверяется.
 */
int comp_stud(const void *first, const void *second);
/**
 * Компаратор ключей.
 * Входные параметры и ограничения те же что и в предыдущей функции.
 */
int comp_keys(const void *first, const void *second);

#endif //__MY_SORTS_H__