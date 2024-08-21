#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>
#include <math.h>

#include "my_input.h"

#define EPS 5e-7

void read_num(int *val, int minval, int maxval)
{
    char buf[2];
    char tmp;
    bool cont = true;
    size_t buf_size;
    *val = 0;
    while (cont)
    {
        printf("Введите целое число от %1d до %1d: ", minval, maxval);
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
            printf("Ошибка. Пустой ввод.\n");
        else
        {
            buf[buf_size] = '\0';
            *val = 0;
            cont = false;
            for (size_t i = 0; i < buf_size && !(cont); ++i)
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
                    cont = true;
                }
            }
            if (!(cont))
            {
                if (*val < minval || *val > maxval)
                {
                    printf("Ошибка. Число выходит за пределы допустимого диапазона.\n");
                    cont = true;
                }
            }
        }
    }
}

void read_params(params_t *times)
{
    bool cont = true;
    printf("Настройка времен поступления и обработки заявок.\n");
    printf("Каждый раз необходимо будет ввести 2 числа: начало и конец интервала.\n");
    printf("Числа должны быть неотрицательными, допускаются числа с плавающей точкой.\n");
    printf("Начало интервала всегда меньше конца интервала.\n");
    printf("Среднее время обработки заявки не может быть больше или равно среднему времени прихода заявки этого же типа.\n");
    printf("В противном случае заявки другого типа никогда не будут обработаны.\n");
    printf("Любые посторонние символы (кроме пробельных) недопустимы.\n");
    while (cont)
    {
        printf("Введите интервал времени прихода заявки 1 типа: ");
        if (scanf("%lf %lf", &(times->t11), &(times->t12)) != 2)
            printf("Ошибка при чтении. Попробуйте ещё раз.\n");
        else if (times->t11 >= times->t12)
            printf("Ошибка. Начало интервала должно быть меньше конца интервала.\n");
        else if (times->t11 < 0)
            printf("Ошибка. Границы интервала не могут быть меньше 0.\n");
        else
            cont = false;
        fflush(stdin);
    }
    cont = true;
    while (cont)
    {
        printf("Введите интервал времени прихода заявки 2 типа: ");
        if (scanf("%lf %lf", &(times->t21), &(times->t22)) != 2)
            printf("Ошибка при чтении. Попробуйте ещё раз.\n");
        else if (times->t21 >= times->t22)
            printf("Ошибка. Начало интервала должно быть меньше конца интервала.\n");
        else if (times->t21 < 0)
            printf("Ошибка. Границы интервала не могут быть меньше 0.\n");
        else
            cont = false;
        fflush(stdin);
    }
    cont = true;
    while (cont)
    {
        printf("Введите интервал времени обработки заявки 1 типа: ");
        if (scanf("%lf %lf", &(times->t31), &(times->t32)) != 2)
            printf("Ошибка при чтении. Попробуйте ещё раз.\n");
        else if (times->t31 >= times->t32)
            printf("Ошибка. Начало интервала должно быть меньше конца интервала.\n");
        else if (times->t31 < 0)
            printf("Ошибка. Границы интервала не могут быть меньше 0.\n");
        else
            cont = false;
        fflush(stdin);
    }
    cont = true;
    while (cont)
    {
        printf("Введите интервал времени обработки заявки 2 типа: ");
        if (scanf("%lf %lf", &(times->t41), &(times->t42)) != 2)
            printf("Ошибка при чтении. Попробуйте ещё раз.\n");
        else if (times->t41 >= times->t42)
            printf("Ошибка. Начало интервала должно быть меньше конца интервала.\n");
        else if (times->t41 < 0)
            printf("Ошибка. Границы интервала не могут быть меньше 0.\n");
        else if (times->t42 + times->t41 + EPS > times->t22 + times->t21)
            printf("Ошибка. Введённые значения превышают допустимые.\n");
        else
            cont = false;
        fflush(stdin);
    }
    char c = getchar();
    while (c != '\n')
        c = getchar();
}