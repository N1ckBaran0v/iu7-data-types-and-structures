#include <stdlib.h>
#include <string.h>

#include "../inc/my_sorts.h"
#include "../inc/my_record.h"
#include "../inc/my_errors.h"

/**
 * Обмен значений.
 * Принимает на вход два указателя и размер.
 * Корректность указателей не проверяется.
 * Корректность размера не проверяется.
 * Не включена в публичный интерфейс.
 */
void swap(void *first, void *second, const size_t size);

/**
 * Копирование значения.
 * Принимает на вход два указателя и размер.
 * Корректность указателей не проверяется.
 * Корректность размера не проверяется.
 * Не включена в публичный интерфейс.
 */
void mycpy(const void *first, void *second, const size_t size);

/**
 * Сортировка слиянием.
 * Принимает на вход указатели на 2 буфера, левую и правую границы, размер элемента и компаратор.
 * Корректность указателей не проверяется.
 * Корректность размера и границ не проверяется.
 * Не включена в публичный интерфейс из-за огромного количества параметров.
 * Функция fast_sort фактически является оберткой для этой функции, которая упрощает её вызов.
 */
void sort_step(void *buf, void *tmpbuf, size_t left, size_t right, size_t size, f_comp comparator);
//int my_strcmp(const char *first, const char *second);

int easy_sort(void *buf, size_t nmemb, size_t size, f_comp comparator)
{
    for (size_t i = nmemb; i; --i)
    {
        for (size_t j = 1; j < i; ++j)
        {
            if (comparator((char*)buf + (j - 1) * size, (char*)buf + j * size) > 0)
            {
                swap((char*)buf + (j - 1) * size, (char*)buf + j * size, size);
            }
        }
    }
    return ERR_OK;
}

int fast_sort(void *buf, size_t nmemb, size_t size, f_comp comparator)
{
    int rc = ERR_OK;
    void *tmpbuf = calloc(nmemb, size);
    if (tmpbuf)
        sort_step(buf, tmpbuf, 0, nmemb, size, comparator);
    else
        rc = ERR_ALLOC_MEMORY;
    free(tmpbuf);
    return rc;
}

void sort_step(void *buf, void *tmpbuf, size_t left, size_t right, size_t size, f_comp comparator)
{
    if (left + 1 < right)
    {
        size_t mid = ((size_t)(right + left) >> 1);
        sort_step(buf, tmpbuf, left, mid, size, comparator);
        sort_step(buf, tmpbuf, mid, right, size, comparator);
        size_t itl = left, itr = mid;
        for (size_t it = left; it < right; ++it)
        {
            if (itr < right && (itl == mid || comparator((char*)buf + itl * size, (char*)buf + itr * size) > 0))
            {
                mycpy((char*)buf + itr * size, (char*)tmpbuf + it * size, size);
                ++itr;
            }
            else
            {
                mycpy((char*)buf + itl * size, (char*)tmpbuf + it * size, size);
                ++itl;
            }
        }
        for (size_t it = left; it < right; ++it)
            mycpy((char*)tmpbuf + it * size, (char*)buf + it * size, size);
    }
}

int comp_stud(const void *first, const void *second)
{
    const student *fs = first;
    const student *ss = second;
    return strcmp(fs->surname, ss->surname);
}

int comp_keys(const void *first, const void *second)
{
    const key *fk = first;
    const key *sk = second;
    return strcmp(fk->surname, sk->surname);
}

void swap(void *first, void *second, const size_t size)
{
    char *cfirst = first;
    char *csecond = second;
    char tmp;
    for (size_t i = 0; i < size; ++i)
    {
        tmp = *(cfirst + i);
        *(cfirst + i) = *(csecond + i);
        *(csecond + i) = tmp;
    }
}

void mycpy(const void *first, void *second, const size_t size)
{
    const char *cfirst = first;
    char *csecond = second;
    for (size_t i = 0; i < size; ++i)
        *(csecond + i) = *(cfirst + i);
}

/*int my_strcmp(const char *first, const char *second)
{
    int ret = 0;
    for (size_t i = 0; i < MAX_STR_SIZE && ret == 0; ++i)
    {
        if (first[i] > second[i])
            ret = 1;
        if (first[i] < second[i])
            ret = -1;   
    }
    return ret;
}*/