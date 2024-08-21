#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "my_io.h"
#include "my_errors.h"

/**
 * Проверяют строку, можно ли из неё получить целое или вещественное число.
 * Единственный параметр - указатель на начало строки.
 * Возвращают код ошибки.
 * Не проверяют корректность указателя.
 * Не проверяют, что строка корректная.
 * Нужны только для проверки строки в функции cin, поэтому не включены в публичный интерфейс.
 */
int __is_big_decimal(const char*);
int __is_big_float(const char*);

/**
 * Считывает строку.
 * На вход получает указатель на строку.
 * Возвращает код ошибки.
 * Не проверяет корректность указателя.
 * Была использована вместо fgets для более простой обработки ошибки переполнения.
 * Не используется в публичном интерфейсе.
 */
int __strcin(char*);


int cin(struct bignum *first, struct bignum *second)
{
    char buf[__MAX_BUF_SIZE + 1];
    int rc = OK;
    if (rc == OK)
    {
        printf("Введите целое число: ");
        rc = __strcin(buf);
        if (rc == OK)
        {
            rc = __is_big_decimal(buf);
        }
        if (rc == OK)
        {
            rc = convert(first, buf);
        }
    }
    if (rc == OK)
    {
        printf("Введите вещественное число: ");
        rc = __strcin(buf);
        if (rc == OK)
        {
            rc = __is_big_float(buf);
        }
        if (rc == OK)
        {
            rc = convert(second, buf);
        }
    }
    return rc;
}

int __strcin(char *buf)
{
    int rc = OK;
    size_t i = 0;
    char c = getchar();
    while (c && c != '\n' && c != EOF && rc == OK)
    {
        *(buf + i) = c;
        c = getchar();
        ++i;
        if (i > __MAX_BUF_SIZE) 
        {
            rc = BUFFER_OVERFLOW;
            printf("Ошибка: переполнение буфера при чтении.\n");
        }
    }
    if (rc == OK)
    {
        *(buf + i) = 0;
    }
    return rc;
}

int __is_big_decimal(const char *buf)
{
    int rc = OK;
    const char *start = buf;

    for (;isspace(*buf); ++buf);

    if (*buf == '-' || *buf == '+')
        ++buf;

    size_t counter = 0;
    for (; isdigit(*buf) && counter < __MAX_DIGIT_SIZE; ++counter, ++buf);

    if (isdigit(*buf))
    {
        rc = BAD_NUM_SIZE;
        printf("Ошибка: переполнение числа.\n");
    }

    if (counter == 0)
    {
        rc = NUM_NOT_FOUND;
        printf("Ошибка: число не найдено.\n");
    }

    for (; *buf && rc == OK; ++buf)
    {
        if (!isspace(*buf))
        {
            rc = UNEXPECTED_SYMBOL;
            printf("Ошибка: встречен некорректный символ: '%c', позиция символа: %lld.\n", *buf, (long long)(buf - start + 1));
            printf("%s\n", start);
            for (size_t i = 0; i < (size_t)(buf - start); ++i)
                printf("-");
            printf("^\n");
        }
    }

    return rc;
}

int __is_big_float(const char *buf)
{
    const char *start = buf;
    int rc = OK;

    for (;isspace(*buf); ++buf);

    if (*buf == '-' || *buf == '+')
        ++buf;

    size_t counter = 0;
    for (; isdigit(*buf) && counter < __MAX_DIGIT_SIZE; ++counter, ++buf);

    if (*buf == '.')
        ++buf;

    for (; isdigit(*buf) && counter < __MAX_DIGIT_SIZE; ++counter, ++buf);

    if (isdigit(*buf))
    {
        rc = BAD_NUM_SIZE;
        printf("Ошибка: переполнение мантиссы.\n");
    }

    if (isspace(*buf))
        ++buf;

    if (counter == 0)
    {
        rc = NUM_NOT_FOUND;
        printf("Ошибка: мантисса не найдена.\n");
    }

    if (rc == OK && (*buf == 'E' || *buf == 'e'))
    {
        ++buf;
        if (isspace(*buf))
            ++buf;
            
        if (*buf == '-' || *buf == '+')
            ++buf;

        size_t exp_counter = 0;
        for (; isdigit(*buf) && exp_counter < __MAX_EXP_SIZE; ++buf, ++exp_counter);

        if (isdigit(*buf))
        {
            rc = BAD_NUM_SIZE;
            printf("Ошибка: переполнение экспоненты.\n");
        }

        if (exp_counter == 0)
        {
            rc = NUM_NOT_FOUND;
            printf("Ошибка: экспонента не найдена.\n");
        }
    }

    for (;*buf && rc == OK; ++buf)
    {
        if (!isspace(*buf))
        {
            rc = UNEXPECTED_SYMBOL;
            printf("Ошибка: встречен некорректный символ: '%c', позиция символа: %lld.\n", *buf, (long long)(buf - start + 1));
            printf("%s\n", start);
            for (size_t i = 0; i < (size_t)(buf - start); ++i)
                printf("-");
            printf("^\n");
        }
    }

    return rc;
}

void cout(struct bignum *curr)
{
    printf("Result: ");
    size_t size = (curr->size - 1) / __MAX_ELEM_DIGIT_SIZE;
    if (curr->msign)
        printf("-");
    printf("0.%lli", (long long)curr->mnt[size]);
    for (size_t i = size; i > 0; --i)
        printf("%06lli", (long long)curr->mnt[i - 1]);
    printf(" E %d\n", curr->exp);
}