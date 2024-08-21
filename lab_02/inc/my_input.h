/**
 * Данный модуль содержит функции чтения полей.
 */
#ifndef __MY_INPUT_H__

#define __MY_INPUT_H__

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "my_record.h"

#define NAME_ALPHA 0
#define GROUP_ALPHA 1
#define NUM_BUF_SIZE 4
#define YEAR 0
#define HOUSE 1
#define ROOM 2
#define HOSTEL 3
#define ENROLLED 4
#define CNT_STUD 5
#define INIT_MIN_ARR int16_t min_arr[] = {16, 1, 1, 1, 1961, 1}
#define INIT_MAX_ARR int16_t max_arr[] = {27, 999, 2000, 20, 2022, 50}
#define MIN_ARR min_arr
#define MAX_ARR max_arr
#define MIN_MARK 2
#define MAX_MARK 5
#define EPS 0.001

/**
 * Чтение строки.
 * Принимает на вход потоки ввода и вывода, указатель на строку и алфавит.
 * Корректность указателей не проверяется.
 * При чтении из файла в выходной поток следует передавать NULL.
 * Алфавитом могут быть только константы NAME_ALPHA и GROUP_ALPHA.
 */
int read_str(iofiles *io, char *buf, const int alpha);

/**
 * Чтение целого числа.
 * Принимает на вход потоки ввода и вывода, указатель на значение и назначение числа.
 * Корректность указателей не проверяется.
 * При чтении из файла в выходной поток следует передавать NULL.
 * Назначение числа - одна из констант: YEAR, HOUSE, ROOM, ENROLLED, CNT_STUD.
 */
int read_num(iofiles *io, int16_t *val, const int type);

/**
 * Чтение вещественного числа.
 * Принимает на вход потоки ввода и вывода и указатель на число.
 * Корректность указателей не проверяется.
 * При чтении из файла в выходной поток следует передавать NULL.
 */
int read_float(iofiles *io, float *val);

/**
 * Чтение буфера.
 * Принимает на вход потоки ввода и вывода, указатель на строку, указательн на размер и максимальный размер буфера.
 * Корректность указателей не проверяется.
 * При чтении из файла в выходной поток следует передавать NULL.
 */
int read_buf(iofiles *io, char *buf, size_t *size, size_t max_buf);

#endif //__MY_RESIZE_H__