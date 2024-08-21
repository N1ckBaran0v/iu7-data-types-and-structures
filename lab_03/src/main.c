#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#include "../inc/my_errors.h"
#include "../inc/my_input.h"
#include "../inc/my_multiple.h"
#include "../inc/my_output.h"
#include "../inc/my_memory.h"
#include "../inc/my_time.h"

int main(int argc, char **argv)
{
    srand(time(0));
    int cc = ERR_OK;
    sp_m matr = spm_init();
    sp_m vec = spm_init();
    sp_m dst = spm_init();
    us_m umatr = usm_init();
    us_m uvec = usm_init();
    us_m udst = usm_init();
    bool was_init_us = false;
    bool was_us = false;
    bool was_spec = false;
    int act, mode;
#ifdef DEBUG
    if (argc == 2 && strcmp(argv[1], "--time") == 0)
    {
        printf("Подсчёт эффективности на случайных данных.\n");
        printf("Ввод числа строк в матрице/количества элементов в векторе столбце.\n");
        cc = read_num(&(matr.rows), 1, MAX_NUM_SIZE);
        if (cc == ERR_OK)
        {
            printf("Ввод числа столбцов в матрице.\n");
            cc = read_num(&(matr.columns), 1, MAX_NUM_SIZE);
        }
        if (cc == ERR_OK)
        {
            vec.columns = matr.rows;
            vec.rows = 1;
            gettime(&matr, &vec, &umatr, &uvec, RANDOM);
        }
        return cc;
    }
#endif
    printf("Эта программа считывает разреженные матрицу и вектор-строку, а также перемножает и выводит их.\n");
    while (!(cc))
    {
        if (matr.columns == 0)
            printf("Внимание! Матрицы не проинициализированы!\n");
        printf("1 - Ввод матрицы и вектора-строки.\n");
        if (matr.rows)
        {
            printf("2 - Перемножить вектор-строку и матрицу, хранящиеся в виде 3-х массивов.\n");
            printf("3 - Стандартное перемножение вектора-строки и матрицы.\n");
            printf("4 - Замер эффективности работы при каждом из алгоритмов.\n");
            printf("5 - Вывод матрицы.\n");
            printf("6 - Вывод вектора-строки.\n");
            printf("7 - Вывод результата перемножения.\n");
        }
        printf("Для выхода введите \"EXIT\" (без кавычек).\n");
        cc = read_num(&act, 1, 7 - 6 * (matr.rows == 0));
        if (!(cc))
        {
            switch (act)
            {
            case 1:
                cc = read_structs(&vec, &matr);
                if (!(cc))
                {
                    was_init_us = false;
                    was_us = false;
                    was_spec = false;
                    struct_free(&dst);
                    cc = create_usual(&umatr, &matr);
                    if (!(cc))
                        cc = create_usual(&uvec, &vec);
                    if (cc)
                    {
                        free(umatr.vals);
                        free(uvec.vals);
                    }
                    else
                        was_init_us = true;
                }
                break;
            case 2:
                if (!was_spec)
                {
                    dst.rows = 1;
                    dst.columns = matr.columns;
                    cc = struct_alloc(&dst);
                    if (!(cc))
                    {
                        mult_special(&dst, &vec, &matr);
                        struct_realloc(&dst);
                    }
                    if (!(cc))
                        was_spec = true;
                }
                break;
            case 3:
                if (was_init_us)
                {
                    if (!was_us)
                    {
                        udst.rows = 1;
                        udst.columns = umatr.columns;
                        udst.vals = calloc(umatr.columns, sizeof(int));
                        if (udst.vals)
                            mult_usual(&udst, &uvec, &umatr);
                        else
                        {
                            cc = ERR_ALLOC_MEMORY;
                            printf("Ошибка. Не удалось выделить память.\n");
                        }
                        if (!(cc))
                            was_us = true;
                    }
                }
                else
                    printf("Доступ ограничен.\n");
                break;
            case 4:
                if (was_init_us)
                    gettime(&matr, &vec, &umatr, &uvec, NO_RANDOM);
                else
                    printf("Доступ ограничен.\n");
                break;
            case 5:
                printf("1 - Вывод всех элементов.\n");
                printf("2 - Вывод всех элементов по матрице, хранящейся в виде 3-х массивов.\n");
                printf("3 - Вывод только ненулевых элементов.\n");
                printf("Ввод номера действия.\n");
                cc = read_num(&mode, 1, 3);
                if (!(cc))
                {
                    switch (mode)
                    {
                    case 1:
                        if (was_init_us)
                            out_normal(&umatr);
                        else
                            printf("Доступ ограничен.\n");
                        break;
                    case 2:
                        out_by_spec(&matr);
                        break;
                    default:
                        out_spec(&matr);
                        break;
                    }
                }
                break;
            case 6:
                printf("1 - Вывод всех элементов.\n");
                printf("2 - Вывод всех элементов по вектору-строке, хранящейся в виде 3-х массивов.\n");
                printf("3 - Вывод только ненулевых элементов.\n");
                printf("Ввод номера действия.\n");
                cc = read_num(&mode, 1, 3);
                if (!(cc))
                {
                    switch (mode)
                    {
                    case 1:
                        if (was_init_us)
                            out_normal(&uvec);
                        else
                            printf("Доступ ограничен.\n");
                        break;
                    case 2:
                        out_by_spec(&vec);
                        break;
                    default:
                        out_spec(&vec);
                        break;
                    }
                }
                break;
            case 7:
                if (!was_spec)
                {
                    dst.rows = 1;
                    dst.columns = matr.columns;
                    cc = struct_alloc(&dst);
                    if (!(cc))
                    {
                        mult_special(&dst, &vec, &matr);
                        cc = struct_realloc(&dst);
                    }
                    if (!(cc))
                        was_spec = true;
                }
                if (!(cc))
                {
                    if (dst.el_cnt == 0)
                        printf("Результат перемножения состоит только из нулевых элементов.\n");
                    else
                    {
                        printf("1 - Вывод всех элементов.\n");
                        printf("2 - Вывод всех элементов по вектору-строке, хранящейся в виде 3-х массивов.\n");
                        printf("3 - Вывод только ненулевых элементов.\n");
                        printf("Ввод номера действия.\n");
                        cc = read_num(&mode, 1, 3);
                        if (!(cc))
                        {
                            switch (mode)
                            {
                            case 1:
                                if (was_init_us)
                                {
                                    if (!was_us)
                                    {
                                        udst.rows = 1;
                                        udst.columns = umatr.columns;
                                        udst.vals = calloc(umatr.columns, sizeof(int));
                                        if (udst.vals)
                                            mult_usual(&udst, &uvec, &umatr);
                                        else
                                        {
                                            cc = ERR_ALLOC_MEMORY;
                                            printf("Ошибка. Не удалось выделить память.\n");
                                        }
                                        if (!(cc))
                                            was_us = true;
                                    }
                                    if (was_us)
                                        out_normal(&udst);
                                }
                                else
                                    printf("Доступ ограничен.\n");
                                break;
                            case 2:
                                out_by_spec(&dst);
                                break;
                            default:
                                out_spec(&dst);
                                break;
                            }
                        }
                    }
                }
                break;
            }
            cc = ERR_OK;
        }
    }
    free(umatr.vals);
    free(uvec.vals);
    free(udst.vals);
    struct_free(&matr);
    struct_free(&vec);
    struct_free(&dst);
    return 0;
}