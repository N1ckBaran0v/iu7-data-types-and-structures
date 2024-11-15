/**
 * Этот модуль содержит функции добавления и удаления записей из таблицы.
 */
#ifndef __MY_RESIZE_H__

#define __MY_RESIZE_H__

#include <stdio.h>
#include <stddef.h>

#include "my_record.h"
#include "my_create.h"

/**
 * Добавление записи в таблицу.
 * Принимает на вход указатели на таблицу, размер и потоки ввода и вывода.
 * Корректность указателей не проверяется.
 * Корректность размера не проверяется.
 */
int add(student *arr, size_t *size, iofiles *io);

/**
 * Удаление записей из таблицы.
 * Принимает на вход указатели на таблицу, размер и потоки ввода и вывода.
 * Корректность указателей не проверяется.
 * Корректность размера не проверяется.
 */
int del(student *arr, size_t *size, iofiles *io);

#endif //__MY_RESIZE_H__