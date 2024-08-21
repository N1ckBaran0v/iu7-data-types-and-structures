#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "my_model.h"

#define EPS 5e-7

static size_t max(size_t first, size_t second);
static double min(double first, double second);
static double gen_time(double minval, double maxval);
static double expected(const params_t *times);

int simulate(const params_t *times, int mode, ret_t *mem)
{
    int rc = QUEUE_OK;
    my_queue *q1 = init(mode);
    my_queue *q2 = init(mode);
    if (q1 && q2)
    {
        size_t sum1 = 0, sum2 = 0;
        int app_state = 0;
        double last1 = 0, last2 = 0, tmp;
        double wait = 0, work = 0, last = 0;
        size_t first_out = 0;
        size_t len_sum1 = 0, changed1 = 0, len_sum2 = 0, changed2 = 0;
        QUEUE_TYPE unused;
        bool need_add1 = false, need_add2 = false;
        while (rc == QUEUE_OK && first_out < 1000)
        {
            if (rc == QUEUE_OK && last1 < EPS)
            {
                last1 = gen_time(times->t11, times->t12);
                if (need_add1)
                {
                    len_sum1 += q1->size(q1->queue);
                    if (q1->size(q1->queue) < QUEUE_MAX_SIZE)
                    {
                        rc = q1->push(q1->queue, NULL);
                        ++sum1;
                    }
                }
                else
                    need_add1 = true;
            }
            if (rc == QUEUE_OK && last2 < EPS)
            {
                last2 = gen_time(times->t21, times->t22);
                if (need_add2)
                {
                    len_sum2 += q2->size(q2->queue);
                    if (q2->size(q2->queue) < QUEUE_MAX_SIZE)
                    {
                        rc = q2->push(q2->queue, NULL);
                        ++sum2;
                    }
                }
                else
                    need_add2 = true;
            }
            if (rc == QUEUE_OK && last < EPS)
            {
                wait -= last;
                last = 0;
                if (app_state == 1)
                {
                    if (q1->is_empty(q1->queue))
                        app_state = 0;
                    else
                    {
                        len_sum1 += q1->size(q1->queue);
                        ++changed1;
                        ++first_out;
                        last = gen_time(times->t31, times->t32);
                        q1->pop(q1->queue, &unused);
                        work += last;
                        if (first_out % 100 == 0 && !(mem))
                        {
                            printf("Обработано заявок 1 типа: %ld\n", first_out);
                            printf("Текущая длина 1 очереди: %ld\n", q1->size(q1->queue));
                            printf("Средняя длина 1 очереди: %ld\n", len_sum1 / (changed1 + sum1));
                            printf("Текущая длина 2 очереди: %ld\n", q2->size(q2->queue));
                            printf("Средняя длина 2 очереди: %ld\n", len_sum2 / (changed2 + sum2));
                        }
                    }
                }
                if (app_state == 2)
                {
                    if (q2->is_empty(q2->queue))
                        app_state = 0;
                    else
                    {
                        len_sum2 += q2->size(q2->queue);
                        ++changed2;
                        last = gen_time(times->t41, times->t42);
                        q2->pop(q2->queue, &unused);
                        work += last;
                    }
                }
                if (app_state == 0)
                {
                    if (!(q2->is_empty(q2->queue)))
                    {
                        len_sum2 += q2->size(q2->queue);
                        ++changed2;
                        app_state = 2;
                        last = gen_time(times->t41, times->t42);
                        q2->pop(q2->queue, &unused);
                        work += last;
                    }
                    else if (!(q1->is_empty(q1->queue)))
                    {
                        len_sum1 += q1->size(q1->queue);
                        ++changed1;
                        ++first_out;
                        app_state = 1;
                        last = gen_time(times->t31, times->t32);
                        q1->pop(q1->queue, &unused);
                        work += last;
                        if (first_out % 100 == 0 && !(mem))
                        {
                            printf("Обработано заявок 1 типа: %ld\n", first_out);
                            printf("Текущая длина 1 очереди: %ld\n", q1->size(q1->queue));
                            printf("Средняя длина 1 очереди: %ld\n", len_sum1 / (changed1 + sum1));
                            printf("Текущая длина 2 очереди: %ld\n", q2->size(q2->queue));
                            printf("Средняя длина 2 очереди: %ld\n", len_sum2 / (changed2 + sum2));
                        }
                    }
                }
            }
            if (rc == QUEUE_OK)
            {
                if (mem)
                    mem->size = max(mem->size, q1->memsize(q1->queue) + q2->memsize(q2->queue));
                if (last > EPS)
                    tmp = min(last, min(last1, last2));
                else
                    tmp = min(last1, last2);
                last -= tmp;
                last1 -= tmp;
                last2 -= tmp;
            }
        }
        if (rc == QUEUE_OK && !(mem))
        {
            printf("Полученные результаты.\n");
            printf("Вошло заявок 1 типа: %ld\n", sum1);
            printf("Обработано заявок 1 типа: %ld\n", changed1);
            printf("Вошло заявок 2 типа: %ld\n", sum2);
            printf("Обработано заявок 2 типа: %ld\n", changed2);
            printf("Время работы: %.6lf\n", work);
            printf("Время простоя: %.6lf\n", wait);
            printf("Суммарное время: %.6lf\n", work + wait);
            tmp = expected(times);
            printf("Ожидаемое время: %.6lf\n", tmp);
            printf("Погрешность: %.2lf%%\n", 100 * fabs(work + wait - tmp) / (work + wait));
        }
        else if (rc)
        {
            if (rc == QUEUE_ERR_OVERFLOW)
                printf("Ошибка. Переполнение очереди.\n");
        }
    }
    else
        rc = QUEUE_ERR_MEM_ALLOC;
    if (rc == QUEUE_ERR_MEM_ALLOC)
        printf("Ошибка. Не удалось выделить память.\n");
    if (q1)
        q1->destroy(q1);
    if (q2)
        q2->destroy(q2);
    return rc;
}

static size_t max(size_t first, size_t second)
{
    return first > second ? first : second;
}

static double min(double first, double second)
{
    return first < second ? first : second;
}

static double gen_time(double minval, double maxval)
{
    return minval + (((double)rand()) / RAND_MAX) * (maxval - minval);
}

static double expected(const params_t *times)
{
    double t1 = times->t12 + times->t11;
    double t3 = times->t32 + times->t31;
    double t2 = times->t22 + times->t21;
    double t4 = times->t42 + times->t41;
    double ret = 0;
    if (t3 > t1)
        ret = 500.0 * t3;
    else if (t1 > t3 + (1 + t4 / t2))
        ret = 500.0 * t1;
    else
        ret = 500.0 * t3 * (1 + t4 / t2);
    return ret;
}