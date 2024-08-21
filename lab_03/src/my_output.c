#include <stdio.h>

#include "../inc/my_output.h"

void out_normal(us_m *matrix)
{
    if (matrix->columns < 11)
    {
        printf("------");
        for (int i = 0; i < matrix->columns; ++i)
            printf("------------");
        printf("\n|    |");
        for (int i = 0; i < matrix->columns; ++i)
            printf("%11d|", i + 1);
        for (int i = 0; i < matrix->rows; ++i)
        {
            printf("\n------");
            for (int j = 0; j < matrix->columns; ++j)
                printf("------------");
            printf("\n|%4d|", i + 1);
            for (int j = 0; j < matrix->columns; ++j)
                printf("%11d|", matrix->vals[i * matrix->columns + j]);
        }
        printf("\n------");
        for (int i = 0; i < matrix->columns; ++i)
            printf("------------");
        printf("\n");
    }
    else
        printf("Матрица имеет слишком больше 10 столбцов, вывод в таком формате невозможен.\n");
}

void out_by_spec(sp_m *matrix)
{
    int itr = 0;
    int itc = 0;
    int tmp = 0;
    if (matrix->columns < 11)
    {
        printf("------");
        for (int i = 0; i < matrix->columns; ++i)
            printf("------------");
        printf("\n|    |");
        for (int i = 0; i < matrix->columns; ++i)
            printf("%11d|", i + 1);
        for (int i = 0; i < matrix->rows; ++i)
        {
            printf("\n------");
            for (int j = 0; j < matrix->columns; ++j)
                printf("------------");
            printf("\n|%4d|", i + 1);
            for (int j = 0; j < matrix->columns; ++j)
            {
                tmp = 0;
                if (itc < matrix->el_cnt)
                {
                    if (i == matrix->rows_st[itr].row && j == matrix->cols[itc])
                    {
                        tmp = matrix->vals[itc];
                        itc++;
                        if (itr + 1 < matrix->rows && matrix->rows_st[itr + 1].start == itc)
                            ++itr;
                    }
                }
                printf("%11d|", tmp);
            }
        }
        printf("\n------");
        for (int i = 0; i < matrix->columns; ++i)
            printf("------------");
        printf("\n");
    }
    else
        printf("Матрица имеет слишком больше 10 столбцов, вывод в таком формате невозможен.\n");
}

void out_spec(sp_m *matrix)
{
    if (matrix->el_cnt == 0)
        printf("Матрица состоит только из нулевых элементов.\n");
    else
    {
        printf("--------------------------\n");
        printf("|   №|   Значение|Столбец|\n");
        printf("--------------------------\n");
        for (int i = 0; i < matrix->el_cnt; ++i)
        {
            printf("|%4d|%11d|%7d|\n", i + 1, matrix->vals[i], matrix->cols[i] + 1);
            printf("--------------------------\n");
        }
        printf("--------------------\n");
        printf("|   №|Строка|Начало|\n");
        printf("--------------------\n");
        for (int i = 0; i < matrix->r_cnt; ++i)
        {
            printf("|%4d|%6d|%6d|\n", i + 1, matrix->rows_st[i].row + 1, matrix->rows_st[i].start + 1);
            printf("--------------------\n");
        }
        /*printf("----------");
        for (int i = 0; i < matrix->el_cnt; ++i)
            printf("------------");
        printf("\n|        |");
        for (int i = 0; i < matrix->el_cnt; ++i)
            printf("%11d|", i + 1);
        printf("\n----------");
        for (int i = 0; i < matrix->el_cnt; ++i)
            printf("------------");
        printf("\n| Столбец|");
        for (int i = 0; i < matrix->el_cnt; ++i)
            printf("%11d|", matrix->cols[i] + 1);
        printf("\n----------");
        for (int i = 0; i < matrix->el_cnt; ++i)
            printf("------------");
        printf("\n|Значение|");
        for (int i = 0; i < matrix->el_cnt; ++i)
            printf("%11d|", matrix->vals[i]);
        printf("\n----------");
        for (int i = 0; i < matrix->el_cnt; ++i)
            printf("------------");
        printf("\n--------");
        for (int i = 0; i < matrix->r_cnt; ++i)
            printf("-----");
        printf("\n|Строка|");
        for (int i = 0; i < matrix->r_cnt; ++i)
            printf("%4d|", matrix->rows_st[i].row + 1);
        printf("\n--------");
        for (int i = 0; i < matrix->r_cnt; ++i)
            printf("-----");
        printf("\n|Начало|");
        for (int i = 0; i < matrix->r_cnt; ++i)
            printf("%4d|", matrix->rows_st[i].start + 1);
        printf("\n--------");
        for (int i = 0; i < matrix->r_cnt; ++i)
            printf("-----");*/
        printf("\n");
    }
}