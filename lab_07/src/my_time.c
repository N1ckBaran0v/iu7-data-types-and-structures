#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>

#include "my_time.h"

static void gen_str(char *str, size_t size);

int check_add(structs_t strs, times_t tms)
{
    int rc = 0;
    if (tms.adds == 0)
        printf("Добавлений пока не было.\n");
    else
    {
        printf("Количество добавлений: %d\n", tms.adds);
        printf("Полученные результаты: \n");

        printf("Для двоичного дерева поиска:\n");
        printf("Суммарное время работы:       %.6lf сек.\n", tms.tbt);
        printf("Среднее количество сравнений: %.6lf.\n", tms.abt / tms.adds);

        printf("Для АВЛ дерева:\n");
        printf("Суммарное время работы:       %.6lf сек.\n", tms.tbta);
        printf("Среднее количество сравнений: %.6lf.\n", tms.abta / tms.adds);

        printf("Для хеш-таблицы с открытым хешированием:\n");
        printf("Суммарное время работы:       %.6lf сек.\n", tms.thto);
        printf("Среднее количество сравнений: %.6lf.\n", tms.ahto / tms.adds);
        printf("Количество реструктуризаций:  %d.\n", tms.rhto);

        printf("Для хеш-таблицы с закрытым хешированием:\n");
        printf("Суммарное время работы:       %.6lf сек.\n", tms.thtc);
        printf("Среднее количество сравнений: %.6lf.\n", tms.ahtc / tms.adds);
        printf("Количество реструктуризаций:  %d.\n", tms.rhtc);
    }
    return rc;
}

int check_time(structs_t strs)
{
    int rc = 0;
    double btt = 0, btat = 0, htot = 0, htct = 0;
    double btc = 0, btac = 0, htoc = 0, htcc = 0;
    clock_t start, stop;
    size_t its = 1000;
    char *ret, key[10];
    for (size_t i = 0; rc == 0 && i < its; ++i)
    {
        printf("Замер времени поиска: [%d/%d]\r", (int)(i + 1), (int)its);
        fflush(stdout);
        gen_str(key, 3 + rand() % 5);

        start = clock();
        ret = search_tree(strs.bt, key);
        stop = clock();
        free(ret);
        btt += (double)(stop - start) / CLOCKS_PER_SEC;
        btc += lastcmp_tree(strs.bt);

        start = clock();
        ret = search_avl_tree(strs.bta, key);
        stop = clock();
        free(ret);
        btat += (double)(stop - start) / CLOCKS_PER_SEC;
        btac += lastcmp_avl_tree(strs.bta);
        
        start = clock();
        ret = search_hasho(strs.hto, key);
        stop = clock();
        free(ret);
        htot += (double)(stop - start) / CLOCKS_PER_SEC;
        htoc += lastcmp_hasho(strs.hto);

        start = clock();
        ret = search_hashc(strs.htc, key);
        stop = clock();
        free(ret);
        htct += (double)(stop - start) / CLOCKS_PER_SEC;
        htcc += lastcmp_hashc(strs.htc);
    }
    if (rc == 0)
    {
        printf("Измерение эффективности успешно завершено.\n");
        printf("Полученные результаты: \n");

        printf("Для двоичного дерева поиска:\n");
        printf("Суммарное время работы:       %.6lf сек.\n", btt);
        printf("Среднее количество сравнений: %.6lf.\n", btc / its);
        printf("Используемая память:          %llu байт.\n", (unsigned long long)mem_size_tree(strs.bt));

        printf("Для АВЛ дерева:\n");
        printf("Суммарное время работы:       %.6lf сек.\n", btat);
        printf("Среднее количество сравнений: %.6lf.\n", btac / its);
        printf("Используемая память:          %llu байт.\n", (unsigned long long)mem_size_avl_tree(strs.bta));

        printf("Для хеш-таблицы с открытым хешированием:\n");
        printf("Суммарное время работы:       %.6lf сек.\n", htot);
        printf("Среднее количество сравнений: %.6lf.\n", htoc / its);
        printf("Используемая память:          %llu байт.\n", (unsigned long long)mem_size_hasho(strs.hto));

        printf("Для хеш-таблицы с закрытым хешированием:\n");
        printf("Суммарное время работы:       %.6lf сек.\n", htct);
        printf("Среднее количество сравнений: %.6lf.\n", htcc / its);
        printf("Используемая память:          %llu байт.\n", (unsigned long long)mem_size_hashc(strs.htc));
    }
    else
        printf("Измерение эффективности было прервано.\n");
    return rc;
}

static void gen_str(char *str, size_t size)
{
    for (size_t i = 0; i + 1 < size; ++i)
        str[i] = 'a' + rand() % 26;
    if (size)
        str[size - 1] = '\0';
}