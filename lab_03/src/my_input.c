#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "../inc/my_input.h"
#include "../inc/my_errors.h"
#include "../inc/my_memory.h"

#define MAX_BUF_SIZE 4

/**
 * Добавление числа в конец матрицы.
 * На вход принимает указатель на матрицу, вставляемое значение и место для вставки.
 * Корректность указателя не проверяется.
 * Корректность значения и мест для вставки не проверяется.
 * Не включена в публичный интерфейс.
 */
void add_elem(sp_m *mdata, int val, int i, int j);

int read_num(int *dst, const int minlim, const int maxlim)
{
    char buf[MAX_BUF_SIZE + 1];
    char tmp;
    int rc = TRY_AGAIN;
    int buf_size;
    int sign = 1;
    *dst = 0;
    while (rc == TRY_AGAIN)
    {
        printf("Введите целое число от %5d до %5d: ", minlim, maxlim);
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
            if (strcmp(buf, "EXIT") == 0)
            {
                printf("Ввод был прекращён.\n");
                rc = STUPID_USER;
            }
            else
            {
                *dst = 0;
                rc = ERR_OK;
                for (int i = 0; i < buf_size && rc == ERR_OK; ++i)
                {
                    if (isdigit(buf[i]) || buf[i] == '-' || buf[i] == '+')
                    {
                        if (buf[i] == '-')
                            sign = -1;
                        else if (isdigit(buf[i]))
                        {
                            *dst *= 10;
                            *dst += (int)(buf[i] - '0');
                        }
                    }
                    else
                    {
                        printf("---------------------------------------");
                        for (int j = 0; j < i; ++j)
                            printf("-");
                        printf("^\n");
                        printf("Ошибка. Встречен неожиданный символ.\n");
                        rc = TRY_AGAIN;
                    }
                }
                if (rc == ERR_OK)
                {
                    *dst *= sign;
                    if (*dst < minlim || *dst > maxlim)
                    {
                        printf("Ошибка. Число выходит за пределы допустимого диапазона.\n");
                        rc = TRY_AGAIN;
                    }
                }
            }
        }
    }
    return rc;
}

int read_structs(sp_m *vector, sp_m *mdata)
{
    int rc = ERR_OK;
    sp_m new_vector = spm_init();
    sp_m new_mdata = spm_init();
    int mode;
    printf("1 - Чтение с клавиатуры.\n");
    printf("2 - Чтение из файла.\n");
    printf("Выберите номер действия.\n");
    rc = read_num(&mode, 1, 2);
    if (rc == ERR_OK)
    {
        if (mode == 2)
        {
            printf("Название файла должно содержать не более 20 символов.\n");
            printf("Не рекомендуется использовать в названии файла символы не из английского алфавита.\n");
            printf("В противном случае может возникнуть ошибка при открытии файла.\n");
            char buf[21];
            size_t it;
            do
            {
                printf("Введите название файла: ");
                rc = ERR_OK;
                it = 0;
                buf[it] = getchar();
                while (buf[it] != '\n' && it < 21)
                {
                    ++it;
                    if (it > 20)
                    {
                        rc = ERR_BUF_OVERFLOW;
                        printf("Ошибка.  Переполнение буфера.\n");
                        while (buf[20] != '\n')
                            buf[20] = getchar();
                    }
                    else
                        buf[it] = getchar();
                }
                if (it < 21)
                    buf[it] = '\0';
            } while (rc != ERR_OK);
            FILE *fin = fopen(buf, "r");
            if (fin)
                rc = read_from_file(&new_mdata, &new_vector, fin);
            else
            {
                rc = FOPEN_ERROR;
                printf("Ошибка. Не удалось открыть файл.\n");
            }
        }
        else
        {
            printf("Ввод можно прервать на любой стадии, если при вводе ввести \"EXIT\" (без кавычек).\n");
            printf("Ввод числа строк в матрице/количества элементов в векторе столбце.\n");
            rc = read_num(&(new_mdata.rows), 1, MAX_NUM_SIZE);

            if (rc == ERR_OK)
            {
                printf("Ввод числа столбцов в матрице.\n");
                rc = read_num(&(new_mdata.columns), 1, MAX_NUM_SIZE);
            }

            if (rc == ERR_OK)
            {
                printf("1 - Ввод всех элементов.\n");
                printf("2 - Выборочный ввод элементов.\n");
                printf("3 - Заполнение случайными числами.\n");
                printf("Ввод номера действия.\n");
                rc = read_num(&mode, 1, 3);
            }

            if (rc == ERR_OK)
            {
                if (mode < 3)
                    printf("Ввод элементов матрицы.\n");
                rc = read_struct(&new_mdata, mode);
            }

            if (rc == ERR_OK)
            {
                new_vector.rows = 1;
                new_vector.columns = new_mdata.rows;
                if (mode < 3)
                    printf("Ввод элементов вектора-строки.\n");
                rc = read_struct(&new_vector, mode);
            }
        }
    }
    if (rc == ERR_OK)
    {
        struct_cpy(vector, &new_vector);
        struct_cpy(mdata, &new_mdata);
    }
    return rc;
}

int read_struct(sp_m *mdata, const int mode)
{
    int rc = ERR_OK;
    int new_tmp, i, j, pr;
    rc = struct_alloc(mdata);
    if (rc == ERR_OK)
    {
        switch (mode)
        {
        case 1:
            for (int i = 0; rc == ERR_OK && i < mdata->rows; ++i)
            {
                for (int j = 0; rc == ERR_OK && j < mdata->columns; ++j)
                {
                    printf("Ввод элемента с позиции (%d, %d).\n", i + 1, j + 1);
                    rc = read_num(&new_tmp, -MAX_NUM_SIZE, MAX_NUM_SIZE);
                    if (rc == ERR_OK)
                        add_elem(mdata, new_tmp, i, j);
                }
            }
            break;
        case 2:
            printf("Внимание! Вводить позиции можно только в порядке возрастания, при выборе очередной позиции ");
            printf("для ввода те позиции, которые были до текущей, становятся недоступными для выбора.\n");
            printf("Для корректного прекращения ввода при вводе значения введите 0.\n");
            new_tmp = 1;
            i = 1;
            j = 1;
            while (rc == ERR_OK && new_tmp)
            {
                printf("Ввод номера строки.\n");
                rc = read_num(&i, i, mdata->rows);
                if (rc == ERR_OK && i <= mdata->rows)
                {
                    printf("Ввод номера столбца.\n");
                    rc = read_num(&j, j, mdata->columns);
                    if (rc == ERR_OK && j <= mdata->columns)
                    {
                        printf("Ввод элемента с позиции (%d, %d).\n", i, j);
                        rc = read_num(&new_tmp, -MAX_NUM_SIZE, MAX_NUM_SIZE);
                        if (rc == ERR_OK && new_tmp)
                        {
                            add_elem(mdata, new_tmp, i - 1, j - 1);
                        }
                    }
                    else if (rc == ERR_OK)
                        new_tmp = 0;
                }
                else if (rc == ERR_OK)
                    new_tmp = 0;
                ++j;
                if (j > mdata->columns)
                {
                    j = 1;
                    ++i;
                }
                if (i > mdata->rows)
                    new_tmp = 0;
            }
            break;
        default:
            if (mode == 3)
            {
                printf("Ввод процента нулевых элементов от общего числа ");
                printf("(число может быть округлено вниз в зависимости от матрицы).\n");
                rc = read_num(&pr, 0, 100);
            }
            else
                pr = mode - 4;
            if (rc == ERR_OK)
            {
                int nuls = (int)(((double)(mdata->rows * mdata->columns) / 100) * pr);
                int nnuls = mdata->rows * mdata->columns - nuls;
                for (int i = 0; rc == ERR_OK && i < mdata->rows; ++i)
                {
                    for (int j = 0; rc == ERR_OK && j < mdata->columns; ++j)
                    {
                        if ((nnuls == 0) || (nuls && rand() % 100 < pr))
                        {
                            new_tmp = 0;
                            nuls--;
                        }
                        else
                        {
                            new_tmp = rand() % (2 * MAX_NUM_SIZE) - MAX_NUM_SIZE + 1;
                            nnuls--;
                        }
                        add_elem(mdata, new_tmp, i, j);
                    }
                }
            }
            break;
        }
    }
    if (rc == ERR_OK)
        struct_realloc(mdata);
    return rc;
}

int read_from_file(sp_m *matr, sp_m *vec, FILE *fin)
{
    int rc = ERR_OK;
    if (fscanf(fin, "%d", &(matr->rows)) != 1)
    {
        rc = FREAD_ERROR;
        printf("Ошибка при чтении размера матрицы.\n");
    }
    else if (matr->rows < 1 || matr->rows > MAX_NUM_SIZE)
    {
        rc = FREAD_ERROR;
        printf("Ошибка при чтении размера матрицы.\n");
    }

    if (rc == ERR_OK)
    {
        if (fscanf(fin, "%d", &(matr->columns)) != 1)
        {
            rc = FREAD_ERROR;
            printf("Ошибка при чтении размера матрицы.\n");
        }
        else if (matr->columns < 1 || matr->columns > MAX_NUM_SIZE)
        {
            rc = FREAD_ERROR;
            printf("Ошибка при чтении размера матрицы.\n");
        }
        else
        {
            vec->rows = 1;
            vec->columns = matr->rows;
            rc = struct_alloc(matr);
        }

        if (rc == ERR_OK)
            rc = struct_alloc(vec);
    }

    int tmp = 0;
    for (int i = 0; rc == ERR_OK && i < matr->rows; ++i)
    {
        for (int j = 0; rc == ERR_OK && j < matr->columns; ++j)
        {
            if (fscanf(fin, "%d", &tmp) != 1)
            {
                rc = FREAD_ERROR;
                printf("Ошибка при чтении элемента матрицы.\n");
            }
            else if (tmp < 0 || tmp > MAX_NUM_SIZE)
            {
                rc = FREAD_ERROR;
                printf("Ошибка при чтении элемента матрицы.\n");
            }
            else
                add_elem(matr, tmp, i, j);
        }
    }
    for (int i = 0; rc == ERR_OK && i < matr->rows; ++i)
    {
        if (fscanf(fin, "%d", &tmp) != 1)
        {
            rc = FREAD_ERROR;
            printf("Ошибка при чтении элемента вектора.\n");
        }
        else if (tmp < 0 || tmp > MAX_NUM_SIZE)
        {
            rc = FREAD_ERROR;
            printf("Ошибка при чтении элемента вектора.\n");
        }
        else
            add_elem(vec, tmp, 0, i);
    }
    return rc;
}

void add_elem(sp_m *mdata, int val, int i, int j)
{
    if (val)
    {
        mdata->vals[mdata->el_cnt] = val;
        mdata->cols[mdata->el_cnt] = j;
        if (mdata->r_cnt == 0 || mdata->rows_st[mdata->r_cnt - 1].row != i)
        {
            mdata->rows_st[mdata->r_cnt].row = i;
            mdata->rows_st[mdata->r_cnt].start = mdata->el_cnt;
            ++(mdata->r_cnt);
        }
        ++(mdata->el_cnt);
    }
}