#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_read.h"
#include "my_tree.h"
#include "my_time.h"

void do_nothing(node_tree *node, void *unused) {}

int time_check(const tree *analyse)
{
    printf("Ввод числа повторений при замере эффективности.\n");
    printf("На экран будет выведено суммарное время работы.\n");
    printf("Память, используемая под стек на массиве, имеет постоянный размер.\n");
    printf("Память, используемая под стек на списке, зависит от количества элементов в стеке.\n");
    double srt = 0, src = 0;
    clock_t start, stop;
    int n;
    int rc = read_num(&n, 1, 1000000);
    date unused;
    char *tmp = NULL;
    if (rc == ERR_OK)
    {
        if (!(analyse))
            rc = ERR_EMPTY_TREE;
        for (int i = 1; rc == ERR_OK && i <= n; ++i)
        {
            printf("Замер времени: [%d/%d]\r", (int)(i + 1), (int)n);
            fflush(stdout);
            start = clock();
            apply_tree(analyse, do_nothing, NULL, DO_MIDDLE);
            stop = clock();
            srt += (double)(stop - start) / CLOCKS_PER_SEC;
            apply_tree(analyse, get_random_value_tree, &tmp, DO_MIDDLE);
            start = clock();
            unused = search_tree(analyse, tmp);
            stop = clock();
            tmp = NULL;
            src += (double)(stop - start) / CLOCKS_PER_SEC;
        }
        if (rc == ERR_OK)
        {
            printf("Измерение эффективности успешно завершено.\n");
            printf("Полученные результаты: \n");
            printf("Используемая память: %llu байт.\n", (unsigned long long)mem_size_tree(analyse));
            printf("Обход в отсортированном порядке:\n");
            printf("Время работы:        %lf сек.\n", srt);
            printf("Поиск:\n");
            printf("Время работы:        %lf сек.\n", src);
        }
        else
            printf("Измерение эффективности было прервано.\n");
        n -= unused.day;
    }
    return rc;
}
