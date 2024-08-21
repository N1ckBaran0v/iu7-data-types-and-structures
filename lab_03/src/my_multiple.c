#include "../inc/my_multiple.h"
#include "../inc/my_memory.h"
#include "../inc/my_errors.h"

void mult_usual(us_m *dst, const us_m *vector, const us_m *matrix)
{
    int tmp;
    for (int i = 0; i < matrix->columns; ++i)
    {
        tmp = 0;
        for (int j = 0; j < matrix->rows; ++j)
            tmp += matrix->vals[j * matrix->columns + i] * vector->vals[j];
        dst->vals[i] = tmp;
    }
}

void mult_special(sp_m *dst, const sp_m *vector, const sp_m *matrix)
{
    for (int i = 0; i < matrix->columns; ++i)
        dst->vals[i] = 0;
    int itv = 0;
    int itr = 0;
    int itc = 0;
    for (int i = 0; itv < vector->el_cnt && itr < matrix->r_cnt && i < matrix->rows; ++i)
    {
        if (vector->cols[itv] == i && matrix->rows_st[itr].row == i)
        {
            ++itr;
            for (itc = matrix->rows_st[itr - 1].start; \
                (itr < matrix->r_cnt && itc < matrix->rows_st[itr].start) ||\
                (itr == matrix->r_cnt && itc < matrix->el_cnt); ++itc)
                dst->vals[matrix->cols[itc]] += vector->vals[itv] * matrix->vals[itc];
            ++itv;
        }
        if (itv < vector->el_cnt && vector->cols[itv] == i)
            ++itv;
        if (itr < matrix->r_cnt && matrix->rows_st[itr].row == i)
            ++itr;
        /*tmp = 0;
        for (int j = 0; j < matrix->rows; ++j)
            tmp += matrix->umatr.vals[j + matrix->columns + i] * vector->umatr.vals[j];
        dst->umatr.vals[i] = tmp;*/
    }
    dst->el_cnt = 0;
    for (int it = 0; it < matrix->columns; ++it)
    {
        if (dst->vals[it])
        {
            if (it != dst->el_cnt)
                dst->vals[dst->el_cnt] = dst->vals[it];
            dst->cols[dst->el_cnt] = it;
            dst->el_cnt++;
        }
    }
    if (dst->el_cnt)
    {
        dst->r_cnt = 1;
        dst->rows_st[0].row = 0;
        dst->rows_st[0].start = 0;
    }
}