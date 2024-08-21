#include <stdio.h>
#include <stdlib.h>

#include "../inc/my_memory.h"
#include "../inc/my_errors.h"

int struct_alloc(sp_m *mdata)
{
    int rc = ERR_OK;
    void *old_cols = mdata->cols;
    void *old_vals = mdata->vals;
    void *old_rows_st = mdata->rows_st;
    mdata->vals = calloc(mdata->rows * mdata->columns, sizeof(int));
    mdata->cols = malloc(mdata->rows * mdata->columns * sizeof(int));
    mdata->vals = malloc(mdata->rows * mdata->columns * sizeof(int));
    mdata->rows_st = malloc(mdata->rows * sizeof(*(mdata->rows_st)));
    if (!mdata->cols || !mdata->vals || !mdata->rows_st)
    {
        free(mdata->cols);
        free(mdata->vals);
        free(mdata->rows_st);
        mdata->cols = old_cols;
        mdata->vals = old_vals;
        mdata->rows_st = old_rows_st;
        rc = ERR_ALLOC_MEMORY;
        printf("Ошибка. Не удалось выделить память.\n");
    }
    return rc;
}

int struct_realloc(sp_m *mdata)
{
    int rc = ERR_OK;
    if (mdata->el_cnt <= 0 || mdata->r_cnt <= 0)
    {
        free(mdata->cols);
        free(mdata->vals);
        free(mdata->rows_st);
        mdata->cols = NULL;
        mdata->vals = NULL;
        mdata->rows_st = NULL;
    }
    else
    {
        void *old_cols = mdata->cols;
        void *old_vals = mdata->vals;
        void *old_rows_st = mdata->rows_st;
        mdata->cols = realloc(old_cols, mdata->el_cnt * sizeof(int));
        mdata->vals = realloc(old_vals, mdata->el_cnt * sizeof(int));
        mdata->rows_st = realloc(old_rows_st, mdata->r_cnt * sizeof(mdata->rows_st));
        if (!mdata->cols || !mdata->vals || !mdata->rows_st)
        {
            struct_free(mdata);
            free(mdata->cols);
            free(mdata->vals);
            free(mdata->rows_st);
            mdata->cols = old_cols;
            mdata->vals = old_vals;
            mdata->rows_st = old_rows_st;
            rc = ERR_ALLOC_MEMORY;
            printf("Ошибка. Не удалось сократить используемую память.\n");
        }
    }
    return rc;
}

void struct_cpy(sp_m *dst, const sp_m *src)
{
    struct_free(dst);
    dst->rows = src->rows;
    dst->columns = src->columns;
    dst->el_cnt = src->el_cnt;
    dst->r_cnt = src->r_cnt;
    dst->vals = src->vals;
    dst->cols = src->cols;
    dst->rows_st = src->rows_st;
}

void struct_free(sp_m *mdata)
{
    if (mdata)
    {
        free(mdata->cols);
        free(mdata->vals);
        free(mdata->rows_st);
        mdata->cols = NULL;
        mdata->vals = NULL;
        mdata->rows_st = NULL;
        mdata->columns = mdata->rows = mdata->el_cnt = mdata->r_cnt = 0;
    }
}