#include <stdio.h>
#include <time.h>

#include "../inc/my_time.h"
#include "../inc/my_errors.h"
#include "../inc/my_sorts.h"
#include "../inc/my_create.h"

int test_time(student *stud, key *keys, size_t size, size_t rnd)
{
    student tmp[2000];
    key ktmp[2000];
    double dur1, dur2, dur3, dur4;
    dur1 = 0;
    dur2 = 0;
    dur3 = 0;
    dur4 = 0;
    int rc = ERR_OK;
    for (size_t i = 0; rc == ERR_OK && i < 1 + rnd; ++i)
    {
        if (rnd)
        {
            iofiles io;
            io.input = stdin;
            io.output = NULL;
            rewrite(stud, size, &io, random);
            create_keys(stud, keys, size);
        }
        for (size_t i = 0; i < size; ++i)
        {
            tmp[i] = stud[i];
            ktmp[i] = keys[i];
        }
        clock_t start, stop;
        start = clock();
        easy_sort(ktmp, size, sizeof(key), comp_keys);
        stop = clock();
        dur1 += (double)(stop - start) / CLOCKS_PER_SEC;
        start = clock();
        easy_sort(tmp, size, sizeof(student), comp_stud);
        stop = clock();
        dur2 += (double)(stop - start) / CLOCKS_PER_SEC;
        for (size_t i = 0; i < size; ++i)
        {
            tmp[i] = stud[i];
            ktmp[i] = keys[i];
        }
        if (rc == ERR_OK)
        {
            start = clock();
            rc = fast_sort(ktmp, size, sizeof(key), comp_keys);
            stop = clock();
        }
        dur3 += (double)(stop - start) / CLOCKS_PER_SEC;
        if (rc == ERR_OK)
        {
            start = clock();
            rc = fast_sort(tmp, size, sizeof(student), comp_stud);
            stop = clock();
        }
        dur4 += (double)(stop - start) / CLOCKS_PER_SEC;
    }
    dur1 /= (1 + rnd);
    dur2 /= (1 + rnd);
    dur3 /= (1 + rnd);
    dur4 /= (1 + rnd);

    if (rc == ERR_OK)
    {
        printf("Сортировка пузырьком таблицы студентов.\nВремя: %.6lf секунд.\n", dur2);
        printf("Память, затраченная на хранение данных: %d байт.\n", (int)(size * sizeof(student)));
        printf("Сортировка пузырьком таблицы ключей.\nВремя: %.6lf секунд.\n", dur1);
        printf("Память, затраченная на хранение данных: %d байт.\n", (int)(size * (sizeof(student) + sizeof(key))));
        printf("Сортировка слиянием таблицы студентов.\nВремя: %.6lf секунд.\n", dur4);
        printf("Память, затраченная на хранение данных: %d байт.\n", (int)(2 * size * sizeof(student)));
        printf("Сортировка слиянием таблицы ключей.\nВремя: %.6lf секунд.\n", dur3);
        printf("Память, затраченная на хранение данных: %d байт.\n", (int)(size * (sizeof(student) + 2 * sizeof(key))));
    }
    else
    {
        printf("Возникли ошибки при выделении памяти.\n");
    }
    return rc;
}