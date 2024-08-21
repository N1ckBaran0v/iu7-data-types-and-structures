#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "my_time.h"
#include "my_astack.h"
#include "my_lstack.h"
#include "my_read.h"
#include "my_count.h"

int check_time(astack *as, lstack *ls)
{
    set_log(false);
    printf("Ввод числа повторений при замере эффективности.\n");
    printf("На экран будет выведено среднее время работы.\n");
    printf("Память, используемая под стек на массиве, имеет постоянный размер.\n");
    printf("Память, используемая под стек на списке, зависит от количества элементов в стеке.\n");
    double ast = 0, lst = 0;
    clock_t start, stop;
    TYPE_STACK n, size, tmp;
    int rc = read_num(&n, 1, 1000);
    size_t mema = 0, meml = 0;
    astack *asc;
    lstack *lsc;
    if (rc == ERR_OK)
    {
        if (!(as) || !(ls))
        {
            printf("Ввод количества чисел в стеке.\n");
            rc = read_num(&size, 1, 1000);
        }
        for (TYPE_STACK i = 1; rc == ERR_OK && i <= n; ++i)
        {
            printf("Замер времени: [%d/%d]\r", (int)(i + 1), (int)n);
            fflush(stdout);
            if (as && ls)
            {
                asc = copy_astack(as);
                lsc = copy_lstack(ls);
            }
            else
            {
                asc = init_astack();
                lsc = init_lstack();
                if (asc && lsc)
                {
                    TYPE_STACK tmp = rand() % MAX_VAL_SIZE;
                    rc = push_astack(asc, tmp);
                    rc = push_lstack(lsc, tmp);
                    for (TYPE_STACK j = 1; rc == ERR_OK && j < size; ++j)
                    {
                        tmp = rand() % 3;
                        rc = push_astack(asc, tmp);
                        rc = push_lstack(lsc, tmp);
                        if (rc == ERR_OK)
                        {
                            tmp = rand() % MAX_VAL_SIZE;
                            rc = push_astack(asc, tmp);
                            rc = push_lstack(lsc, tmp);
                        }
                    }
                }
                else 
                    rc = ERR_MEM_ALLOC;
            }
            if (asc && lsc && rc == ERR_OK)
            {
                if (mema == 0)
                    mema = memsize_astack(asc);
                start = clock();
                rc = count_astack(asc, &tmp);
                stop = clock();
                ast += (double)(stop - start) / CLOCKS_PER_SEC;
                if (rc == ERR_OK)
                {
                    if (meml == 0)
                        meml = memsize_lstack(lsc);
                    start = clock();
                    rc = count_lstack(lsc, &tmp);
                    stop = clock();
                    lst += (double)(stop - start) / CLOCKS_PER_SEC;
                }
            }
            else
                rc = ERR_MEM_ALLOC;
            destroy_astack(asc);
            destroy_lstack(lsc);
        }
        if (rc == ERR_OK)
        {
            printf("Измерение эффективности успешно завершено.\n");
            printf("Полученные результаты: \n");
            printf("Для стека на массиве:\n");
            printf("Время работы:        %lf сек.\n", ast / n);
            printf("Используемая память: %llu байт.\n", (unsigned long long)mema);
            printf("Для стека на списке:\n");
            printf("Время работы:        %lf сек.\n", lst / n);
            printf("Используемая память: %llu байт.\n", (unsigned long long)meml);
        }
        else
            printf("Измерение эффективности было прервано.\n");
    }
    set_log(true);
    return rc;
}

