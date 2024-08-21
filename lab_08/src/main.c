#define _GNU_SOURCE

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>

#include "my_graph.h"

#define MAX_INPUT 999999

void read_num(uint32_t *val, uint32_t minval, uint32_t maxval);

int main(void)
{
    int rc = 0, trc;
    my_graph *graph = my_graph_init();
    bool need_iter = true;
    if (graph)
    {
        printf("Программа считывает граф с клавиатуры и находит для любой из его вершин все вершины,"
               "расстояние до которых меньше или равно заданному числу."
               "\nГраф может быть сохранён в формате .gv.\n");
    }
    else
    {
        printf("Ошибка. Не удалось создать граф.\n");
        rc = -1;
    }
    uint32_t act, start, end, tmp;
    size_t size;
    while (rc == 0 && need_iter)
    {
        printf("0 - Выход.\n"
               "1 - Добавление ребра в граф.\n"
               "2 - Удаление ребра из графа.\n"
               "3 - Поиск вершин.\n"
               "4 - Сохранение графа.\n"
               "Выберите номер действия.\n");
        read_num(&act, 0, 4);
        if (act == 0)
        {
            printf("До свидания!\n");
            need_iter = false;
        }
        if (act == 1)
        {
            printf("Ввод номера стартовой вершины.\n");
            read_num(&start, 1, MAX_NUM);
            printf("Ввод номера конечной вершины.\n");
            read_num(&end, 1, MAX_NUM);
            printf("Ввод длины пути.\n");
            read_num(&tmp, 1, MAX_LENGHT);
            trc = my_graph_push(graph, start, end, tmp);
            if (trc == ERR_INCORRECT)
                printf("Ошибка. Нельзя создать ребро в ту же вершину, из которой оно исходит.\n");
        }
        if (act == 2)
        {
            printf("Ввод номера стартовой вершины.\n");
            read_num(&start, 1, MAX_NUM);
            printf("Ввод номера конечной вершины.\n");
            read_num(&end, 1, MAX_NUM);
            tmp = my_graph_pop(graph, start, end);
            if (tmp == 0)
                printf("Ребро не найдено.\n");
        }
        if (act == 3)
        {
            printf("Ввод номера стартовой вершины.\n");
            read_num(&start, 1, MAX_NUM);
            printf("Ввод максимальной длины пути.\n");
            read_num(&tmp, 1, MAX_NUM * MAX_LENGHT);
            uint32_pair *ans = NULL;
            size = my_graph_find(graph, start, tmp, &ans);
            if (size)
            {
                printf("Достижимые вершины:\n");
                for (size_t i = 0; i < size; ++i)
                    printf("%ld: %ld\n", (long int)ans[i].number, (long int)ans[i].value);
            }
            else
                printf("Достижимых вершин за такое расстояние не найдено.\n");
            free(ans);
        }
        if (act == 4)
        {
            char *filename = NULL;
            size = 0;
            printf("Предпочтительный формат файла при сохранении: *.gv\n");
            printf("Введите имя файла (с расширением): ");
            ssize_t rsize = getline(&filename, &size, stdin);
            if (rsize > 1)
            {
                for (char *c = filename; *c; ++c)
                    if (*c == '\n')
                        *c = '\0';
                trc = my_graph_save(graph, filename);
                if (trc == ERR_FILE)
                    printf("Ошибка при открытии файла.\n");
                else if (trc == ERR_NOT_FOUND)
                    printf("Граф пустой.\n");
            }
            else
                printf("Ошибка. Строка пустая.\n");
        }
    }
    my_graph_destroy(graph);
    return rc;
}

void read_num(uint32_t *val, uint32_t minval, uint32_t maxval)
{
    char buf[8];
    char tmp;
    bool cont = true;
    size_t buf_size;
    *val = 0;
    while (cont)
    {
        printf("Введите целое число от %7ld до %7ld: ", (long int)minval, (long int)maxval);
        buf_size = 0;
        tmp = getchar();
        while (tmp != '\n')
        {
            buf[buf_size] = tmp;
            ++buf_size;
            tmp = getchar();
            if (buf_size > 1)
                while (tmp != '\n')
                    tmp = getchar();
        }
        if (buf_size > 7)
            printf("Ошибка. Переполнение буфера при вводе.\n");
        else if (buf_size == 0)
            printf("Ошибка. Пустой ввод.\n");
        else
        {
            buf[buf_size] = '\0';
            *val = 0;
            cont = false;
            for (size_t i = 0; i < buf_size && !(cont); ++i)
            {
                if (isdigit(buf[i]))
                {
                    *val *= 10;
                    *val += (uint32_t)(buf[i] - '0');
                }
                else
                {
                    printf("-----------------------------------------------");
                    for (int j = 0; j < i; ++j)
                        printf("-");
                    printf("^\n");
                    printf("Ошибка. Встречен неожиданный символ.\n");
                    cont = true;
                }
            }
            if (!(cont))
            {
                if (*val < minval || *val > maxval)
                {
                    printf("Ошибка. Число выходит за пределы допустимого диапазона.\n");
                    cont = true;
                }
            }
        }
    }
}