#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "my_read.h"

int read_num(int *val, int minval, int maxval)
{
    char buf[MAX_BUF_SIZE + 1];
    char tmp;
    int rc = ERR_CONTINUE;
    size_t buf_size;
    *val = 0;
    while (rc == ERR_CONTINUE)
    {
        printf("Введите целое число от %4d до %4d: ", minval, maxval);
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
                    *val += (int)(buf[i] - '0');
                }
                else
                {
                    printf("-------------------------------------");
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
