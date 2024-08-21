/**
 * Содержит структуру и функции для перевода в неё и умножения.
 */

#ifndef __MY_STRUCT_H__

#define __MY_STRUCT_H__

#include <stdint.h>
#include <stdbool.h>

#define __ARR_SIZE 10
#define __MAX_ELEM_DIGIT_SIZE 6
#define __MAX_EXP_SIZE 5
#define __MAX_DIGIT_SIZE 30
#define __MAX_ELEM_SIZE 1000000
#define __MAX_EXP_ABS 99999

/**
 * Хранит в себе большое число
 */
struct bignum 
{
    int32_t mnt[__ARR_SIZE]; // Мантисса
    int32_t exp; // Экспонента
    size_t size; // Количество цифр в мантиссе
    bool msign; // Знак
};

/**
 * Получает из строки bignum.
 * На вход получает ссылку на структуру и ссылку на строку.
 * Возвращает код ошибки.
 * Не проверяет корректность строки и возможность перевода.
 * Не проверяет корректность указателей или на перекрытие памяти.
 */
int convert(struct bignum*, const char*);

/**
 * Перемножает два больших числа и записывает их в третью структуру.
 * На вход получает ссылки на 3 больших числа.
 * Возвращает код ошибки.
 * Предполагает, что все 3 указателя разные и указывают на нужные структуры.
 * Предполагает, что 2 последних указателя содержат уже корректные структуры.
 */
int multiple(struct bignum*, const struct bignum*, const struct bignum*);

#endif // __MY_STRUCT_H__