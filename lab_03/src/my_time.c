#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include "../inc/my_time.h"
#include "../inc/my_input.h"
#include "../inc/my_errors.h"
#include "../inc/my_memory.h"
#include "../inc/my_multiple.h"

int gettime(sp_m *sp, sp_m *vec, us_m *us, us_m *uvec, int mode)
{
    int rc = ERR_OK;
    printf("Ввод числа повторений при замере эффективности.\n");
    printf("На экран будет выведено среднее время работы.\n");
    printf("Для случая матрицы, хранящейся в одном массиве, память меняться не будет.\n");
    printf("Для случая матрицы, хранящейся в трёх массивах, будет выведена максимальная используемая память.\n");
    size_t maxmem = 0, cmem = 0;
    int n, pr1, pr2;
    double spt = 0, ust = 0;
    clock_t start, stop;
    rc = read_num(&n, 1, 1000);
    if (rc == ERR_OK)
    {
        sp_m dst = spm_init();
        us_m udst = usm_init();
        dst.rows = udst.rows = 1;
        dst.columns = udst.columns = sp->columns;
        rc = struct_alloc(&dst);
        if (rc == ERR_OK)
        {
            udst.vals = calloc(udst.columns, sizeof(int));
            if (udst.vals)
            {
                if (mode == RANDOM)
                {
                    printf("Ввод процента нулевых элементов от общего числа для матрицы ");
                    printf("(число может быть округлено вниз в зависимости от матрицы).\n");
                    rc = read_num(&pr1, 0, 100);
                    if (!(rc))
                    {
                        printf("Ввод процента нулевых элементов от общего числа для вектора ");
                        printf("(число может быть округлено вниз в зависимости от вектора).\n");
                        rc = read_num(&pr2, 0, 100);
                    }
                }
                for (int i = 0; rc == ERR_OK && i < n; ++i)
                {
                    printf("Замер времени: [%d/%d]\r", i + 1, n);
                    fflush(stdout);
                    if (mode == RANDOM)
                    {
                        //struct_free(sp);
                        sp->el_cnt = 0;
                        sp->r_cnt = 0;
                        rc = read_struct(sp, pr1 + 4);
                        if (!(rc))
                            rc = create_usual(us, sp);
                        //struct_free(vec);
                        vec->el_cnt = 0;
                        vec->r_cnt = 0;
                        if (!(rc))
                            rc = read_struct(vec, pr2 + 4);
                        if (!(rc))
                            rc = create_usual(uvec, vec);
                    }
                    if (!(rc))
                    {
                        rc = struct_alloc(&dst);
                        if (!(rc))
                        {
                            cmem = sizeof(dst) + sizeof(*sp) + sizeof(*vec);
                            start = clock();
                            mult_special(&dst, vec, sp);
                            stop = clock();
                            spt += (double)(stop - start) / CLOCKS_PER_SEC;
                            cmem += 2 * dst.columns * sizeof(int) + sizeof(*(dst.rows_st));
                            cmem += 2 * sp->el_cnt * sizeof(int) + sp->r_cnt * sizeof(*(dst.rows_st));
                            cmem += 2 * vec->el_cnt * sizeof(vec->cols) + sizeof(*(vec->rows_st));
                            if (cmem > maxmem)
                                maxmem = cmem;
                            struct_realloc(&dst);
                        }
                    }
                    if (!(rc))
                    {
                        start = clock();
                        mult_usual(&udst, uvec, us);
                        stop = clock();
                        ust += (double)(stop - start) / CLOCKS_PER_SEC;
                    }
                }
                if (rc == ERR_OK)
                {
                    printf("Измерение эффективности успешно завершено.\n");
                    printf("Полученные результаты: \n");
                    printf("Для обычной матрицы:\n");
                    printf("Время работы:        %lf сек.\n", ust / n);
                    size_t size = sizeof(udst) + udst.columns * sizeof(int);
                    size += sizeof(*us) + us->rows * us->columns * sizeof(int);
                    size += sizeof(*uvec) + uvec->rows * uvec->columns * sizeof(int);
                    printf("Используемая память: %llu байт.\n", (unsigned long long)size);
                    printf("Для матрицы, хранящейся в виде 3-х векторов:\n");
                    printf("Время работы:        %lf сек.\n", spt / n);
                    printf("Используемая память: %llu байт.\n", (unsigned long long)maxmem);
                }
                else
                    printf("Измерение эффективности было прервано.\n");
                free(udst.vals);
            }
        }
        struct_free(&dst);
    }
    return rc;
}