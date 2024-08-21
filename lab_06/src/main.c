#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "my_errors.h"
#include "my_helps.h"
#include "my_read.h"
#include "my_time.h"
#include "my_tree.h"

int main(void)
{
    int rc = ERR_OK, num;
    tree *wood = init_tree(); //, *tmp_wood = NULL;
    char *tmp_buf, *it;
    size_t buf_size;
    ssize_t unused;
    date day;
    srand(time(0));
    if (!(wood))
    {
        printf("Ошибка. Не удалось создать дерево. Запуск программы отменён.\n");
        rc = ERR_MEM_ALLOC;
    }
    int act = -1;
    while (act && rc == ERR_OK)
    {
        printf("Программа принимает на вход имена файлов и дату их последнего изменения и добавляет их в дерево.\n");
        printf("Доступны операции добавления, удаления по имени или по дате (не позднее определённой даты).\n");
        printf("Также доступен просмотр элементов и вывод графа в файл для графического представления.\n");
        printf("0 - Выход.\n");
        printf("1 - Добавление файла в дерево.\n");
        // printf("2 - Считывание дерева из файла.\n");
        printf("2 - Удаление файла из дерева по имени.\n");
        printf("3 - Удаление файлов из дерева по дате.\n");
        printf("4 - Поиск файла в дереве.\n");
        printf("5 - Сохранение дерева в файл.\n");
        printf("6 - Вывод состояния дерева.\n");
        printf("7 - Замер эффективности для данного дерева.\n");
        do
        {
            rc = read_num(&act, 0, 7);
        } while (rc);
        switch (act)
        {
        case 0:
            printf("До свидания!\n");
            break;
        case 1:
            if (size_tree(wood) < MAX_TREE_SIZE)
            {
                buf_size = 0;
                tmp_buf = NULL;
                printf("Введите имя файла: ");
                unused = getline(&tmp_buf, &buf_size, stdin);
                if (tmp_buf && unused > 1)
                {
                    it = tmp_buf;
                    for (; *it; ++it)
                        if (*it == '\n')
                            *it = '\0';
                    printf("Ввод года.\n");
                    rc = read_num(&(day.year), 1961, 2022);
                    printf("Ввод номера месяца.\n");
                    rc = read_num(&(day.month), 1, 12);
                    printf("Ввод числа.\n");
                    rc = read_num(&(day.day), 1, max_day[day.month - 1] + (day.month == 2 && ((day.year % 4 == 0 && day.year % 100 != 0) || day.year % 400 == 0)));
                    rc = add_to_tree(wood, tmp_buf, &day);
                    if (rc == ERR_MEM_ALLOC)
                        printf("Ошибка. Не удалось выделить память под элемент дерева.");
                }
                else
                {
                    printf("Ошибка. Не удалось считать строку.\n");
                    free(tmp_buf);
                }
            }
            else
                printf("Ошибка. Дерево заполнено.\n");
            free(tmp_buf);
            tmp_buf = NULL;
            buf_size = 0;
            break;
        case 2:
            if (!(is_empty_tree(wood)))
            {
                buf_size = 0;
                tmp_buf = NULL;
                printf("Введите имя файла: ");
                unused = getline(&tmp_buf, &buf_size, stdin);
                if (tmp_buf && unused > 0)
                {
                    it = tmp_buf;
                    for (; *it; ++it)
                        if (*it == '\n')
                            *it = '\0';
                    rc = del_by_name_tree(wood, tmp_buf);
                    if (rc == ERR_NOT_FOUND)
                        printf("Не найден файл с таким именем.\n");
                }
                else
                    printf("Ошибка. Не удалось считать строку.\n");
            }
            else
                printf("Ошибка. Дерево пустое.\n");
            free(tmp_buf);
            tmp_buf = NULL;
            buf_size = 0;
            break;
        case 3:
            if (!(is_empty_tree(wood)))
            {
                printf("Ввод года.\n");
                rc = read_num(&(day.year), 1961, 2022);
                printf("Ввод номера месяца.\n");
                rc = read_num(&(day.month), 1, 12);
                printf("Ввод числа.\n");
                rc = read_num(&(day.day), 1, max_day[day.month - 1] + (day.month == 2 && ((day.year % 4 == 0 && day.year % 100 != 0) || day.year % 400 == 0)));
                num = del_earlier_than_tree(wood, &day);
                printf("Удалено файлов: %d.\n", num);
            }
            else
                printf("Ошибка. Дерево пустое.\n");
            break;
        case 4:
            if (!(is_empty_tree(wood)))
            {
                buf_size = 0;
                tmp_buf = NULL;
                printf("Введите имя файла: ");
                unused = getline(&tmp_buf, &buf_size, stdin);
                if (tmp_buf && unused > 0)
                {
                    it = tmp_buf;
                    for (; *it; ++it)
                        if (*it == '\n')
                            *it = '\0';
                    day = search_tree(wood, tmp_buf);
                    if (day.day == -1)
                        printf("Не найден файл с таким именем.\n");
                    else
                        printf("Дата последнего изменения: %02d.%02d.%04d\n", day.day, day.month, day.year);
                }
                else
                    printf("Ошибка. Не удалось считать строку.\n");
            }
            else
                printf("Ошибка. Дерево пустое.\n");
            free(tmp_buf);
            tmp_buf = NULL;
            buf_size = 0;
            break;
        case 5:
            if (!(is_empty_tree(wood)))
            {
                buf_size = 0;
                tmp_buf = NULL;
                printf("Введите имя выходного файла: ");
                unused = getline(&tmp_buf, &buf_size, stdin);
                if (tmp_buf && unused > 1)
                {
                    it = tmp_buf;
                    for (; *it; ++it)
                        if (*it == '\n')
                            *it = '\0';
                    rc = export_tree(wood, tmp_buf);
                    if (rc == ERR_OK)
                        printf("Дерево сохранено в файл  \"%s\".\n", tmp_buf);
                    else
                        printf("Ошибка. Не удалось открыть файл для записи.\n");
                    free(tmp_buf);
                }
                else
                {
                    printf("Ошибка. Не удалось считать строку.\n");
                    free(tmp_buf);
                }
            }
            else
                printf("Ошибка. Дерево пустое.\n");
            break;
        case 6:
            if (!(is_empty_tree(wood)))
                apply_tree(wood, print_leaf_tree, NULL, DO_MIDDLE);
            else
                printf("Ошибка. Дерево пустое.\n");
            break;
        default:
            rc = time_check(wood);
            break;
        }
        rc = ERR_OK;
    }
    destroy_tree(wood);
    return rc;
}