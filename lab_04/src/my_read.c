#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stddef.h>
#include <ctype.h>

#include "my_read.h"
#include "my_errors.h"

int read_num(TYPE_STACK *val, TYPE_STACK minval, TYPE_STACK maxval)
{
    char buf[MAX_BUF_SIZE + 1];
    char tmp;
    int rc = ERR_CONTINUE;
    size_t buf_size;
    *val = 0;
    while (rc == ERR_CONTINUE)
    {
        printf("Введите целое число от %6lld до %6lld: ", (long long)minval, (long long)maxval);
        buf_size = 0;
        tmp = getchar();
        while (tmp != '\n')
        {
            buf[buf_size] = tmp;
            ++buf_size;
            tmp = getchar();
            if (buf_size > MAX_BUF_SIZE)
                while (tmp != '\n')
                    tmp = getchar();
        }
        if (buf_size > MAX_BUF_SIZE)
            printf("Ошибка. Переполнение буфера при вводе.\n");
        else if (buf_size == 0)
            printf("Ошибка. Пустой ввод.\n");
        else
        {
            buf[buf_size] = '\0';
            *val = 0;
            rc = ERR_OK;
            for (size_t i = 0; i < buf_size && rc == ERR_OK; ++i)
            {
                if (isdigit(buf[i]))
                {
                    *val *= 10;
                    *val += (TYPE_STACK)(buf[i] - '0');
                }
                else
                {
                    printf("-----------------------------------------");
                    for (int j = 0; j < i; ++j)
                        printf("-");
                    printf("^\n");
                    printf("Ошибка. Встречен неожиданный символ.\n");
                    rc = ERR_CONTINUE;
                }
            }
            if (rc == ERR_OK)
            {
                if (*val < minval || *val > maxval)
                {
                    printf("Ошибка. Число выходит за пределы допустимого диапазона.\n");
                    rc = ERR_CONTINUE;
                }
            }
        }
    }
    return rc;
}

int read_sign(TYPE_STACK *val)
{
    char buf[3];
    char tmp;
    int rc = ERR_CONTINUE;
    size_t buf_size;
    *val = 0;
    for (size_t it = 0; rc == ERR_CONTINUE; it = (it + 1) % 5)
    {
        if (!(it))
            printf("Допустимые символы: '+', '-', '*'.\n");
        printf("Введите один символ: ");
        buf_size = 0;
        tmp = getchar();
        while (tmp != '\n')
        {
            buf[buf_size] = tmp;
            ++buf_size;
            tmp = getchar();
            if (buf_size > 1)
                while (tmp != '\n')
                    tmp = getchar();
        }
        if (buf_size > 1)
            printf("Ошибка. Переполнение буфера при вводе.\n");
        else if (buf_size == 0)
        {
            printf("Ошибка. Пустой ввод.\n");
            rc = ERR_CONTINUE;
        }
        else
        {
            rc = ERR_OK;
            switch (*buf)
            {
            case '+':
                *val = 0;
                break;
            case '-':
                *val = 1;
                break;
            case '*':
                *val = 2;
                break;
            default:
                rc = ERR_CONTINUE;
                printf("---------------------^\n");
                printf("Ошибка. Встречен неожиданный символ.\n");
            }
        }
    }
    return rc;
}
