#include "../inc/my_out.h"
#include "../inc/my_errors.h"
#include "../inc/my_sorts.h"
#include "../inc/my_input.h"

void save_table(student *stud, size_t size, FILE *fout)
{
    for (size_t i = 0; i < size; ++i)
    {
        fprintf(fout, "%s\n", stud[i].surname);
        fprintf(fout, "%s\n", stud[i].name);
        fprintf(fout, "%d\n", (int)stud[i].isMale);
        fprintf(fout, "%s\n", stud[i].group);
        fprintf(fout, "%d\n", (int)stud[i].age);
        fprintf(fout, "%.2f\n", stud[i].mark);
        fprintf(fout, "%d\n", (int)stud[i].year);
        fprintf(fout, "%d\n", (int)stud[i].typeofplace);
        if (stud[i].typeofplace == HOME)
        {
            fprintf(fout, "%s\n", stud[i].placeinfo.home.street);
            fprintf(fout, "%d\n", (int)stud[i].placeinfo.home.house);
            fprintf(fout, "%d\n", (int)stud[i].placeinfo.home.flat);
        }
        else
        {
            fprintf(fout, "%d\n", (int)stud[i].placeinfo.hostel.num);
            fprintf(fout, "%d\n", (int)stud[i].placeinfo.hostel.room);
        }
    }
}

int print_table(student *stud, size_t size, bool need_filter, t_sort sort_data)
{
    int rc = sort_data(stud, size, sizeof(student), comp_stud);
    if (rc == ERR_OK)
    {
        if (need_filter)
            rc = filter(stud, &size);
    }
    else 
    {
        printf("Ошибка. Невозможно выделить память для сортировки массива.\n");
    }

    if (rc == ERR_OK)
    {
        printf(" №|      Фамилия       |        Имя         |       Группа       |Возр|Балл| ГП | Адрес или комната в общежитии  \n");
        for (size_t i = 0; i < size; ++i)
        {
            for (size_t i = 0; i < 115; ++i)
                printf("-");
            printf("\n%2d|%20s|", (int)(i + 1), stud[i].surname);
            printf("%20s|", stud[i].name);
            if (stud[i].isMale)
                printf("М|");
            else
                printf("Ж|");
            printf("%20s|", stud[i].group);
            printf("%4d|", (int)stud[i].age);
            printf("%4.2f|", stud[i].mark);
            printf("%4d|", (int)stud[i].year);
            if (stud[i].typeofplace == HOME)
            {
                printf("%20s, ", stud[i].placeinfo.home.street);
                printf("%4d, ", (int)stud[i].placeinfo.home.house);
                printf("%4d\n", (int)stud[i].placeinfo.home.flat);
            }
            else
            {
                printf("             Общежитие №%2d, ", (int)stud[i].placeinfo.hostel.num);
                printf("%4d\n", (int)stud[i].placeinfo.hostel.room);
            }
        }
    }
    return rc;
}

int filter(student *stud, size_t *size)
{
    int rc = ERR_OK;
    iofiles curr;
    curr.input = stdin;
    curr.output = stdout;
    printf("Введите год поступления студента, по которому осуществлять поиск.\n");
    printf("Допустимо только целое число в диапазоне от 1961 до 2022 включительно.\n");
    int16_t val;
    printf("Другие символы, кроме цифр, недопустимы.\n");
    rc = read_num(&curr, &val, ENROLLED);
    if (rc == ERR_OK)
    {
        size_t it = 0;
        for (size_t i = 0; i < *size && rc == ERR_OK; ++i)
        {
            if (stud[i].year != val || stud[i].typeofplace == HOME)
                it++;
            else
                stud[i - it] = stud[i];
        }
        *size -= it;
        if (*size == 0)
        {
            rc = ERR_EMPTY_ARRAY;
            printf("Ошибка. Не найдены студенты, поступившие в указанном году и проживающих в общежитии.\n");
        }
    }
    return rc;
}

int print_keys(key *keys, size_t size)
{
    int rc = fast_sort(keys, size, sizeof(key), comp_keys);
    if (rc == ERR_OK)
    {
        printf(" №|      Фамилия       |№ в таблице\n");
        for (size_t i = 0; i < size; ++i)
        {
            for (size_t i = 0; i < 35; ++i)
                printf("-");
            
            printf("\n%2d|%20s|%11d\n", (int)(i + 1), keys[i].surname, (int)(keys[i].pos + 1));
        }
    }
    else 
    {
        printf("Ошибка. Невозможно выделить память для сортировки массива.\n");
    }
    return rc;
}

int print_by_keys(student *stud, key *keys, size_t size)
{
    int rc = fast_sort(keys, size, sizeof(key), comp_keys);
    if (rc == ERR_OK)
    {
        printf(" №|      Фамилия       |        Имя         |       Группа       |Возр|Балл| ГП | Адрес или комната в общежитии  \n");
        for (size_t i = 0; i < size; ++i)
        {
            for (size_t i = 0; i < 115; ++i)
                printf("-");
            printf("\n%2d|%20s|", (int)(i + 1), stud[keys[i].pos].surname);
            printf("%20s|", stud[keys[i].pos].name);
            if (stud[keys[i].pos].isMale)
                printf("М|");
            else
                printf("Ж|");
            printf("%20s|", stud[keys[i].pos].group);
            printf("%4d|", (int)stud[keys[i].pos].age);
            printf("%4.2f|", stud[keys[i].pos].mark);
            printf("%4d|", (int)stud[keys[i].pos].year);
            if (stud[keys[i].pos].typeofplace == HOME)
            {
                printf("%20s, ", stud[keys[i].pos].placeinfo.home.street);
                printf("%4d, ", (int)stud[keys[i].pos].placeinfo.home.house);
                printf("%4d\n", (int)stud[keys[i].pos].placeinfo.home.flat);
            }
            else
            {
                printf("             Общежитие №%2d, ", (int)stud[keys[i].pos].placeinfo.hostel.num);
                printf("%4d\n", (int)stud[keys[i].pos].placeinfo.hostel.room);
            }
        }
    }
    else 
    {
        printf("Ошибка. Невозможно выделить память для сортировки массива.\n");
    }
    return rc;
}