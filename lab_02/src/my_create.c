#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../inc/my_create.h"
#include "../inc/my_errors.h"
#include "../inc/my_input.h"

int input_one(student *stud, iofiles *io)
{
    int rc = (io->input == NULL);
    if (rc == ERR_OK)
    {
        if (io->output)
        {
            fprintf(io->output, "Введите фамилию студента.\n");
            fprintf(io->output, "Максимальная длина строки - %d символов.\n", (int)MAX_STR_SIZE);
            fprintf(io->output, "Допустимы английские буквы и тире.\n");
        }
        do
            rc = read_str(io, stud->surname, NAME_ALPHA);
        while (rc != ERR_OK && io->output);
    }
    if (rc == ERR_OK)
    {
        if (io->output)
            fprintf(io->output, "Введите имя студента. Требования к вводу те же.\n");
        do
            rc = read_str(io, stud->name, NAME_ALPHA);
        while (rc != ERR_OK && io->output);
    }
    if (rc == ERR_OK)
    {
        if (io->output)
        {
            fprintf(io->output, "0 - Женский.\n");
            fprintf(io->output, "1 - Мужской.\n");
        }
        do
        {
            if (io->output)
                fprintf(io->output, "Выберите пол студента (введите только одну цифру): ");
            char c = fgetc(io->input);
            if (c == '0' || c == '1')
            {
                rc = ERR_OK;
                int val = (int)(c - '0');
                c = fgetc(io->input);
                if (c == '\n' || c == '\0' || c == EOF)
                    stud->isMale = val;
                else
                {
                    while (c != '\n' && c != EOF && c)
                        c = fgetc(io->input);
                    rc = ERR_ARR_OVERFLOW;
                    if (io->output)
                        fprintf(io->output, "Ошибка. Введено слишком много символов.\n");
                }
            }
            else
            {
                while (c != '\n' && c != EOF && c)
                    c = fgetc(io->input);
                rc = ERR_INCORRECT_VALUE;
                if (io->output)
                {
                    fprintf(io->output, "---------------------------------------------------------^\n");
                    fprintf(io->output, "Ошибка. Встречен некорректный символ.\n");
                }
            }
        } while (rc != ERR_OK && io->output);
    }
    if (rc == ERR_OK)
    {
        if (io->output)
            fprintf(io->output, "Введите группу студента. Можно также использовать цифры.\n");
        do
            rc = read_str(io, stud->group, GROUP_ALPHA);
        while (rc != ERR_OK && io->output);
    }
    if (rc == ERR_OK)
    {
        if (io->output)
        {
            fprintf(io->output, "Введите возраст студента.\n");
            fprintf(io->output, "Допустимо только целое число в диапазоне от 16 до 27 включительно.\n");
            fprintf(io->output, "Другие символы, кроме цифр, недопустимы.\n");
        }
        do
            rc = read_num(io, &(stud->age), YEAR);
        while (rc != ERR_OK && io->output);
    }
    if (rc == ERR_OK)
    {
        if (io->output)
        {
            fprintf(io->output, "Введите средний балл студента.\n");
            fprintf(io->output, "Допустимо только дробное число в диапазоне от 2 до 5 включительно.\n");
            fprintf(io->output, "Допустимо наличие 2 цифр после разделителя.\n");
            fprintf(io->output, "Разделителем считается точка.\n");
        }
        do
            rc = read_float(io, &(stud->mark));
        while (rc != ERR_OK && io->output);
    }
    if (rc == ERR_OK)
    {
        if (io->output)
        {
            fprintf(io->output, "Введите год поступления студента.\n");
            fprintf(io->output, "Допустимо только целое число в диапазоне от 1961 до 2022 включительно.\n");
            fprintf(io->output, "Другие символы, кроме цифр, недопустимы.\n");
        }
        do
            rc = read_num(io, &(stud->year), ENROLLED);
        while (rc != ERR_OK && io->output);
    }
    if (rc == ERR_OK)
    {
        if (io->output)
        {
            fprintf(io->output, "0 - В доме.\n");
            fprintf(io->output, "1 - В общежитии.\n");
        }
        do
        {
            if (io->output)
                fprintf(io->output, "Выберите, где живёт студент (введите только одну цифру): ");
            char c = fgetc(io->input);
            if (c == '0' || c == '1')
            {
                rc = ERR_OK;
                int val = (int)(c - '0');
                c = fgetc(io->input);
                if (c == '\n' || c == '\0' || c == EOF)
                    stud->typeofplace = val;
                else
                {
                    while (c != '\n' && c != EOF && c)
                        c = fgetc(io->input);
                    rc = ERR_ARR_OVERFLOW;
                    if (io->output)
                        fprintf(io->output, "Ошибка. Введено слишком много символов.\n");
                }
            }
            else
            {
                while (c != '\n' && c != EOF && c)
                    c = fgetc(io->input);
                rc = ERR_INCORRECT_VALUE;
                if (io->output)
                {
                    fprintf(io->output, "---------------------------------------------------------^\n");
                    fprintf(io->output, "Ошибка. Встречен некорректный символ.\n");
                }
            }
        } while (rc != ERR_OK && io->output);
    }
    if (rc == ERR_OK)
    {
        switch (stud->typeofplace)
        {
        case HOME:
            if (io->output)
            {
                fprintf(io->output, "Введите название улицы.\n");
                fprintf(io->output, "Максимальная длина строки - %d символов.\n", (int)MAX_STR_SIZE);
                fprintf(io->output, "Допустимы английские буквы, цифры и тире.\n");
            }
            do
                rc = read_str(io, stud->placeinfo.home.street, GROUP_ALPHA);
            while (rc != ERR_OK && io->output);
            if (rc == ERR_OK)
            {
                if (io->output)
                {
                    fprintf(io->output, "Введите номер дома.\n");
                    fprintf(io->output, "Допустимо только целое число в диапазоне от 1 до 999 включительно.\n");
                    fprintf(io->output, "Другие символы, кроме цифр, недопустимы.\n");
                }
                do
                    rc = read_num(io, &(stud->placeinfo.home.house), HOUSE);
                while (rc != ERR_OK && io->output);
            }
            if (rc == ERR_OK)
            {
                if (io->output)
                {
                    fprintf(io->output, "Введите номер квартиры.\n");
                    fprintf(io->output, "Допустимо только целое число в диапазоне от 1 до 2000 включительно.\n");
                    fprintf(io->output, "Другие символы, кроме цифр, недопустимы.\n");
                }
                do
                    rc = read_num(io, &(stud->placeinfo.home.flat), ROOM);
                while (rc != ERR_OK && io->output);
            }
            break;
        case HOSTELL:
            if (io->output)
            {
                fprintf(io->output, "Введите номер общежития.\n");
                fprintf(io->output, "Допустимо только целое число в диапазоне от 1 до 20 включительно.\n");
                fprintf(io->output, "Другие символы, кроме цифр, недопустимы.\n");
            }
            do
                rc = read_num(io, &(stud->placeinfo.hostel.num), HOSTEL);
            while (rc != ERR_OK && io->output);
            if (rc == ERR_OK)
            {
                if (io->output)
                {
                    fprintf(io->output, "Введите номер комнаты.\n");
                    fprintf(io->output, "Допустимо только целое число в диапазоне от 1 до 2000 включительно.\n");
                    fprintf(io->output, "Другие символы, кроме цифр, недопустимы.\n");
                }
                do
                    rc = read_num(io, &(stud->placeinfo.hostel.room), ROOM);
                while (rc != ERR_OK && io->output);
            }
            break;
        }
    }
    return rc;
}

int random(student *stud, iofiles *io)
{
    size_t siz = rand() % 20 + 1;
    for (size_t i = 0; i < siz; ++i)
        stud->surname[i] = 'A' + (rand() % 20);
    for (size_t i = siz; i < 21; ++i)
        stud->surname[i] = 0;
    siz = rand() % 20 + 1;
    for (size_t i = 0; i < siz; ++i)
        stud->name[i] = 'A' + (rand() % 20);
    stud->name[siz] = '\0';
    stud->isMale = rand() % 2;
    siz = rand() % 20 + 1;
    for (size_t i = 0; i < siz; ++i)
        stud->group[i] = 'A' + (rand() % 20);
    stud->group[siz] = '\0';
    stud->age = 16 + (rand() % 11);
    stud->mark = 2 + (rand() % 4);
    stud->year = 1961 + (rand() % 62);
    stud->typeofplace = rand() % 2;
    switch (stud->typeofplace)
    {
    case HOSTELL:
        stud->placeinfo.hostel.num = 1 + (rand() % 19);
        stud->placeinfo.hostel.room = 1 + (rand() % 1999);
        break;
    case HOME:
        siz = rand() % 20 + 1;
        for (size_t i = 0; i < siz; ++i)
            stud->placeinfo.home.street[i] = 'A' + (rand() % 3);
        stud->placeinfo.home.street[siz] = '\0';
        stud->placeinfo.home.house = 1 + (rand() % 999);
        stud->placeinfo.home.flat = 1 + (rand() % 1999);
        break;
    }

    return ERR_OK;
}

size_t rewrite(student *arr, size_t size, iofiles *io, foo_in func)
{
    size_t real = 0;
    int rc = ERR_OK;
    for (; real < size && rc == ERR_OK;) // ++real)
    {
        if (io->output)
            fprintf(io->output, "Ввод %d-й строки таблицы.\n", (int)(real + 1));
        rc = func(arr + real, io);
        real += (rc == ERR_OK);
    }
    if (io->output)
        fprintf(io->output, "Ввод завершён. Всего получено строк: %d.\n", (int)real);
    return real;
}

int create_keys(student *stud, key *keys, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        keys[i].pos = i;
        strcpy(keys[i].surname, stud[i].surname);
    }
    return ERR_OK;
}