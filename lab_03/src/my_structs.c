#include <stdio.h>
#include <stdlib.h>

#include "../inc/my_structs.h"
#include "../inc/my_errors.h"

sp_m spm_init(void)
{
    sp_m mdata;
    mdata.rows = 0;
    mdata.columns = 0;
    mdata.el_cnt = 0;
    mdata.r_cnt = 0;
    mdata.cols = NULL;
    mdata.vals = NULL;
    mdata.rows_st = NULL;
    return mdata;
}

us_m usm_init(void)
{
    us_m mdata;
    mdata.rows = 0;
    mdata.columns = 0;
    mdata.vals = NULL;
    return mdata;
}

int create_usual(us_m *us, const sp_m *sp)
{
    int rc = ERR_OK;
    int *tmp = calloc(sp->rows * sp->columns, sizeof(int));
    free(us->vals);
    if (tmp)
    {
        us->vals = tmp;
        us->rows = sp->rows;
        us->columns = sp->columns;
        int itr = 0;
        int itc = 0;
        for (int i = 0; i < sp->rows; ++i)
        {
            for (int j = 0; j < sp->columns; ++j)
            {
                if (itc < sp->el_cnt)
                {
                    if (i == sp->rows_st[itr].row && j == sp->cols[itc])
                    {
                        us->vals[i * us->columns + j] = sp->vals[itc];
                        itc++;
                        if (itr + 1 < sp->rows && sp->rows_st[itr + 1].start == itc)
                            ++itr;
                    }
                }
            }
        }
    }
    else
    {
        rc = ERR_ALLOC_MEMORY;
        printf("Ошибка. Не удалось выделить память под хранение матрицы в обычном виде.\n");
    }
    return rc;
}