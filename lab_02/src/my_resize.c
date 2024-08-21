#include <string.h>

#include "../inc/my_resize.h"
#include "../inc/my_input.h"
#include "../inc/my_errors.h"
#include "../inc/my_sorts.h"

int add(student *arr, size_t *size, iofiles *io)
{
    int rc = ERR_OK;
    if (*size == MAX_ARR_SIZE)
    {
        rc = ERR_ARR_OVERFLOW;
        if (io->output)
            fprintf(io->output, "Ошибка. Массив уже содержит максимальное количество элементов.\n");
    }
    else
        rc = input_one(arr + *size, io);
    if (rc == ERR_OK)
        ++(*size);
    return rc;
}

int del(student *arr, size_t *size, iofiles *io)
{
    char pattern[MAX_STR_SIZE + 1];
    if (io->output)
    {
        fprintf(io->output, "Введите фамилию, по которой будут удаляться записи из таблицы.\n");
        fprintf(io->output, "Можно вводить только английские буквы и тире.\n");
        fprintf(io->output, "Максимальная длина строки: %d символов.\n", (int)MAX_STR_SIZE);
    }
    int rc = ERR_OK;
    do
        rc = read_str(io, pattern, NAME_ALPHA);
    while (rc != ERR_OK && io->output);
    size_t it = 0;
    for (size_t i = 0; i < *size && rc == ERR_OK; ++i)
    {
        if (strcmp(pattern, arr[i].surname) == 0)
            it++;
        else
            arr[i - it] = arr[i];
    }
    if (rc == ERR_OK)
        *size -= it;
    return rc;
}