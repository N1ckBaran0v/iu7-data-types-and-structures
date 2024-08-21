#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "my_time.h"
#include "my_model.h"

int time_test(const params_t *times)
{
    int rc = QUEUE_OK;
    double ast = 0, lst = 0;
    clock_t start, stop;
    size_t its = 1000;
    ret_t mem = {0};
    size_t mema = 0, meml = 0;
    if (rc == QUEUE_OK)
    {
        for (size_t i = 0; rc == QUEUE_OK && i < its; ++i)
        {
            printf("Замер времени: [%d/%d]\r", (int)(i + 1), (int)its);
            fflush(stdout);
            start = clock();
            rc = simulate(times, ARR_QUEUE, &mem);
            stop = clock();
            ast += (double)(stop - start) / CLOCKS_PER_SEC;
            mema = mem.size;
            mem.size = 0;
            if (rc == QUEUE_OK)
            {
                start = clock();
                rc = simulate(times, LST_QUEUE, &mem);
                stop = clock();
                lst += (double)(stop - start) / CLOCKS_PER_SEC;
                meml = mem.size;
                mem.size = 0;
            }
        }
        if (rc == QUEUE_OK)
        {
            printf("Измерение эффективности успешно завершено.\n");
            printf("Полученные результаты: \n");
            printf("Для очереди на массиве:\n");
            printf("Время работы:        %lf сек.\n", ast / its);
            printf("Используемая память: %llu байт.\n", (unsigned long long)mema);
            printf("Для очереди на списке:\n");
            printf("Время работы:        %lf сек.\n", lst / its);
            printf("Используемая память: %llu байт.\n", (unsigned long long)meml);
        }
        else
            printf("Измерение эффективности было прервано.\n");
    }
    return rc;
}