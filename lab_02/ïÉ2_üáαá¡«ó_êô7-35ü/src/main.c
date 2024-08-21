#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "../inc/my_record.h"
#include "../inc/my_resize.h"
#include "../inc/my_errors.h"
#include "../inc/my_create.h"
#include "../inc/my_input.h"
#include "../inc/my_time.h"
#include "../inc/my_out.h"

int main(int argc, char **argv)
{
    int rc = ERR_OK;
#ifdef DEBUG
/**
 * Замер времени при размерах до 2000 на случайно сгенерированных данных.
 * Запуск возможен только при использовании флага -DDEBUG при компиляции.
 */
    if (argc == 2 && strcmp(argv[1], "--random") == 0)
    {
        srand(time(0));
        printf("Программа генерирует случайные данные и сортирует их.\n");
        printf("Размер таблицы в данном случае превышает макcимальный и достигает 2000.\n");
        iofiles io;
        io.input = stdin;
        io.output = stdout;
        student arr[2000];
        printf("Введите количество записей.\n");
        printf("Допустимо только целое число в диапазоне от 1 до 2000 включительно.\n");
        int16_t val;
        printf("Другие символы, кроме цифр, недопустимы.\n");
        do
            rc = read_num(&io, &val, ROOM);
        while (rc != ERR_OK);
        key keys[2000];
        rc = test_time(arr, keys, val, 99);
        return rc;
    }
#endif
    printf("Программа читает таблицу из файла (если необходимо) и выполняет выбранные действия.\n");
    printf("Для сортировки обычно используется сортировка слиянием, сравнивается с сортировкой пузырьком.\n");
    printf("Сортировка идёт по фамилии.\n");
    iofiles io;
    io.input = stdin;
    io.output = stdout;
    student arr[MAX_ARR_SIZE];
    student arr2[MAX_ARR_SIZE];
    size_t size = 0;
    key keys[MAX_ARR_SIZE];
    int16_t tmp;
    char filename[MAX_STR_SIZE + 1];
    size_t tmps;
    while (true)
    {
        printf("Действия:\n");
        printf("1 - Считать таблицу из файла.\n");
        printf("2 - Считать таблицу из клавиатуры.\n");
        printf("3 - Сохранить таблицу в файл.\n");
        printf("4 - Добавить запись в таблицу.\n");
        printf("5 - Удалить записи из таблицы по фамилии.\n");
        printf("6 - Вывести отсортированную таблицу студентов.\n");
        printf("7 - Вывести отсортированную таблицу ключей.\n");
        printf("8 - Отсортировать таблицу ключей и вывести по ней таблицу студентов.\n");
        printf("9 - Вывести отсортированную таблицу после использования каждого из вариантов сортировки.\n");
        printf("10 - Вывести таблицу студентов, поступивших в определенном году и проживающих в общежитии.\n");
        printf("11 - Вывести сравнение эффективности работы сортировок двумя способами таблиц ключей и значений.\n");
        printf("12 - Выход.\n");
        int16_t val = -1;
        printf("Введите номер действия.\n");
        do
        {
            rc = read_num(&io, &val, HOSTEL);
            if (rc == ERR_OK)
                if (val > 12)
                {
                    rc = ERR_INCORRECT_VALUE;
                    fprintf(io.output, "Ошибка. Число не входит в допустимый диапазон значений.\n");
                }
        } while (rc != ERR_OK);

        switch (val)
        {
        case 1:
            printf("Введите имя файла.\n");
            printf("В имени файла не рекомендуется использовать буквы, которые не входят в английский алфавит.\n");
            printf("Использовать можно любые символы, если файл с таким именем откроется.\n");
            printf("Прописывать лучше относительный путь, так как полный путь может не влезть в буфер.\n");
            do
            {
                rc = read_buf(&io, filename, &tmps, MAX_STR_SIZE);
            } while (rc != ERR_OK);
            
            io.input = fopen(filename, "r");
            if (io.input)
            {
                io.output = NULL;
                size = rewrite(arr, MAX_ARR_SIZE, &io, input_one);
                printf("Успешно считано записей: %d.\n", (int)size);
                create_keys(arr, keys, size);
                fclose(io.input);
            }
            else
            {
                rc = ERR_FILE_OPEN;
                printf("Ошибка при открытии файла на запись.\n");
            }
            break;

        case 2:
            printf("Введите количество записей.\n");
            printf("Допустимо только целое число в диапазоне от 1 до 50 включительно.\n");
            printf("Другие символы, кроме цифр, недопустимы.\n");
            do
            {
                rc = read_num(&io, &tmp, CNT_STUD);
            } while (rc != ERR_OK);
            size = rewrite(arr, tmp, &io, input_one);
            create_keys(arr, keys, size);
            break;

        case 3:
            printf("Введите имя файла.\n");
            printf("В имени файла не рекомендуется использовать буквы, которые не входят в английский алфавит.\n");
            printf("Использовать можно любые символы, если файл с таким именем откроется.\n");
            printf("Прописывать лучше относительный путь, так как полный путь может не влезть в буфер.\n");
            do
            {
                rc = read_buf(&io, filename, &tmps, MAX_STR_SIZE);
            } while (rc != ERR_OK);

            io.output = fopen(filename, "w");
            if (io.output)
            {
                save_table(arr, size, io.output);
                fclose(io.output);
            }
            else
            {
                rc = ERR_FILE_OPEN;
                printf("Ошибка при открытии файла на запись.\n");
            }
            break;

        case 4:
            add(arr, &size, &io);
            create_keys(arr, keys, size);
            break;

        case 5:
            del(arr, &size, &io);
            create_keys(arr, keys, size);
            break;

        case 6:
            if (size)
                print_table(arr, size, false, fast_sort);
            else
                printf("Таблица пустая.\n");
            break;

        case 7:
            if (size)
                print_keys(keys, size);
            else
                printf("Таблица пустая.\n");
            break;

        case 8:
            if (size)
                print_by_keys(arr, keys, size);
            else
                printf("Таблица пустая.\n");
            break;

        case 9:
            for (size_t i = 0; i < size; ++i)
                arr2[i] = arr[i];
            printf("Таблица после сортировки пузырьком.\n");
            rc = print_table(arr, size, false, easy_sort);
            if (rc == ERR_OK)
            {
                printf("Таблица после сортировки слиянием.\n");
                print_table(arr2, size, false, fast_sort);
            }
            break;

        case 10:
            if (size)
                print_table(arr, size, true, fast_sort);
            else
                printf("Таблица пустая.\n");
            break;

        case 11:
            if (size)
                test_time(arr, keys, size, 0);
            else
                printf("Таблица пустая.\n");
            break;

        case 12:
            return 0;
        }
        io.input = stdin;
        io.output = stdout;
    }
    return 0;
}
