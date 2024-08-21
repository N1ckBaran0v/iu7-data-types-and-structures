#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#include "my_time.h"

#define MAX_CMP 4

void read_num(int *val, int minval, int maxval);
void read_str(char **str, const char *msg);
int init_all(structs_t*, times_t*);

int main(void)
{
    int rc = 0;
    bool need_iterate = true;
    structs_t strs;
    times_t times = {0};
    clock_t t11, t12, t21, t22, t31, t32, t41, t42;
    rc = init_all(&strs, &times);
    int act, rnd;
    char *key = NULL, *value = NULL;
    if (rc == 0)
    {
        printf("Программа строит хеш-таблицы и деревья дл зарезервированных ключевых слов языка C++.\n");
        printf("Дерева 2 - АВЛ-дерево и обычное дерево двоичного поиска.\n");
        printf("Хэш-таблиц 2 - с открытым и закрытым хешированием.\n");
    }
    while (rc == 0 && need_iterate)
    {
        printf("0 - Выход.\n");
        printf("1 - Добавить ключевое слово и его описание.\n");
        printf("2 - Удалить ключевое слово.\n");
        printf("3 - Найти описание ключевого слова.\n");
        printf("4 - Сохранение в файл.\n");
        printf("5 - Сравнение эффективности добавления в дерево и хэш-таблицы.\n");
        printf("6 - Сравнение эффективности работы деревьев и хэш-таблиц.\n");
        read_num(&act, 0, 6);
        if (act == 0)
        {
            printf("До свидания!\n");
            need_iterate = false;
        }
        else if (act == 1)
        {
            read_str(&key, "Введите ключевое слово: ");
            read_str(&value, "Введите описание: ");
            t11 = clock();
            rc = add_tree(strs.bt, key, value);
            t12 = clock();
            if (rc == 0)
            {
                t21 = clock();
                rc = add_avl_tree(strs.bta, key, value);
                t22 = clock();
                if (rc == 0)
                {
                    t31 = clock();
                    rc = add_hasho(&(strs.hto), key, value);
                    t32 = clock();
                    if (rc == 0)
                    {
                        t41 = clock();
                        rc = add_hashc(&(strs.htc), key, value);
                        t42 = clock();
                        if (rc == 0)
                        {
                            times.tbt += (double)(t12 - t11) / CLOCKS_PER_SEC;
                            times.tbta += (double)(t22 - t21) / CLOCKS_PER_SEC;
                            times.thto += (double)(t32 - t31) / CLOCKS_PER_SEC;
                            times.thtc += (double)(t42 - t41) / CLOCKS_PER_SEC;
                            times.abt += lastcmp_tree(strs.bt);
                            times.abta += lastcmp_avl_tree(strs.bta);
                            times.ahto += lastcmp_hasho(strs.hto);
                            times.ahtc += lastcmp_hashc(strs.htc);
                            times.rhto += lastcmp_hasho(strs.hto) > MAX_CMP;
                            times.rhtc += lastcmp_hashc(strs.htc) > MAX_CMP;
                            (times.adds)++;
                        }
                        else
                        {
                            del_tree(strs.bt, key);
                            del_avl_tree(strs.bta, key);
                            del_hasho(strs.hto, key);
                        }
                    }
                    else
                    {
                        del_tree(strs.bt, key);
                        del_avl_tree(strs.bta, key);
                    }
                }
                else
                    del_tree(strs.bt, key);
            }
            if (rc)
            {
                printf("Ошибка. Не удалось добавить элемент в деревья и таблицы.\n");
                rc = 0;
            }
            free(value);
            value = NULL;
        }
        else if (act == 2)
        {
            read_str(&key, "Введите ключевое слово: ");
            del_tree(strs.bt, key);
            del_avl_tree(strs.bta, key);
            del_hasho(strs.hto, key);
            del_hashc(strs.htc, key);
        }
        else if (act == 3)
        {
            read_str(&key, "Введите ключевое слово: ");
            printf("0 - Поиск в двоичном дереве поиска.\n");
            printf("1 - Поиск в АВЛ дереве.\n");
            printf("2 - Поиск в хеш-таблице с открытым хешированием.\n");
            printf("3 - Поиск в хеш-таблице с закрытым хешированием.\n");
            printf("Выберите где будее искать.\n");
            read_num(&rnd, 0, 3);
            //printf("%d\n", rnd);
            if (rnd == 0)
                value = search_tree(strs.bt, key);
            else if (rnd == 1)
                value = search_avl_tree(strs.bta, key);
            else if (rnd == 2)
                value = search_hasho(strs.hto, key);
            else
                value = search_hashc(strs.htc, key);
            if (value)
                printf("%s\n", value);
            else
                printf("Слово не найдено.\n");
            free(value);
            value = NULL;
        }
        else if (act == 4)
        {
            printf("Что вы хотите сохранить в файл?\n");
            printf("1 - Двоичное дерево поиска.\n");
            printf("2 - АВЛ-дерево.\n");
            printf("3 - Хеш-таблицу с открытым хешированием.\n");
            printf("4 - Хеш-таблицу с закрытым хешированием.\n");
            read_num(&rnd, 1, 4);
            printf("Предпочтительный формат файла для сохранения деревьев: *.gv\n");
            printf("Предпочтительный формат файла для сохранения хеш-таблиц: *.csv\n");
            read_str(&key, "Введите имя файла (с расширением): ");
            if (rnd == 1)
                export_tree(strs.bt, key);
            if (rnd == 2)
                export_avl_tree(strs.bta, key);
            if (rnd == 3)
                export_hasho(strs.hto, key);
            if (rnd == 4)
                export_hashc(strs.htc, key);
        }
        else if (act == 5)
        {
            rc = check_add(strs, times);
        }
        else
            rc = check_time(strs);
    }
    return rc;
}

void read_num(int *val, int minval, int maxval)
{
    char buf[2];
    char tmp;
    bool cont = true;
    size_t buf_size;
    *val = 0;
    while (cont)
    {
        printf("Введите целое число от %1d до %1d: ", minval, maxval);
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
        if (buf_size > 1)
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
                    *val += (int)(buf[i] - '0');
                }
                else
                {
                    printf("-------------------------------------");
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

void read_str(char **str, const char *msg)
{

    size_t size;
    ssize_t rsize;
    bool cont = true;
    while (cont)
    {
        free(*str);
        printf("%s", msg);
        size = 0;
        rsize = getline(str, &size, stdin);
        if (rsize < 2)
            printf("Ошибка. Строка пустая.\n");
        else
        {
            cont = false;
            for (char *it = *str; *it; ++it)
                if (*it == '\n')
                    *it = '\0';
        }
    }
}

int init_all(structs_t *strs, times_t *times)
{
    int rc = 0;
    strs->bt = init_tree();
    strs->bta = init_avl_tree();
    strs->hto = init_hasho();
    strs->htc = init_hashc();
    if (strs->bt && strs->bta && strs->hto && strs->htc)
    {
        clock_t t11, t12, t21, t22, t31, t32, t41, t42;
        char *words[] = {"auto", "bool", "break", "case", "catch",
                         "char", "class", "const", "const_cast", "continue",
                         "decltype", "delete", "do", "double", "dynamic_cast",
                         "else", "explicit", "export", "extern", "false"//};
                         ,"aauto", "bbool", "cbreak", "dcase", "ecatch",
                         "fchar", "gclass", "hconst", "iconst_cast", "jcontinue",
                         "kdecltype", "ldelete", "mdo", "ndouble", "odynamic_cast",
                         "pelse", "qexplicit", "rexport", "sextern", "tfalse"};
        char **vals = words;
        /*char *vals[] = {"Deduces the type of a declared variable from its initialization expression.", 
                         "This keyword is a built-in type. A variable of this type can have values true and false.", 
                         "The break statement ends execution of the nearest enclosing loop or conditional statement in which it appears. Control passes to the statement that follows the end of the statement, if any.", 
                         "Allows selection among multiple sections of code, depending on the value of an integral expression.", 
                         "To implement exception handling in C++, you use try, throw, and catch expressions.",
                         "The char type is a character representation type that efficiently encodes members of the basic execution character set.", 
                         "The class keyword declares a class type or defines an object of a class type.", 
                         "When it modifies a data declaration, the const keyword specifies that the object or variable isn't modifiable.", 
                         "Removes the const, volatile, and __unaligned attribute(s) from a class.", 
                         "Forces transfer of control to the controlling expression of the smallest enclosing do, for, or while loop.",
                         "The decltype type specifier yields the type of a specified expression.", 
                         "Deallocates a block of memory.", 
                         "Executes a statement repeatedly until the specified termination condition (the expression) evaluates to zero.", 
                         "Type double is a floating point type that is larger than or equal to type float, but shorter than or equal to the size of type long double.", 
                         "Converts the operand expression to an object of type type-id.",
                         "The else clause of an if...else statement is associated with the closest previous if statement in the same scope that doesn't have a corresponding else statement.", 
                         "The explicit keyword tells the compiler that the specified conversion can't be used to perform implicit conversions.", 
                         "Used to mark a template definition exported, which allows the same template to be declared, but not defined, in other translation units.", 
                         "The extern keyword may be applied to a global variable, function, or template declaration.", 
                         "The keyword is one of the two values for a variable of type bool or a conditional expression (a conditional expression is now a true Boolean expression)."};*/
        char *key, *value;
        for (size_t i = 0; i < 40; ++i)
        {
            key = words[i];
            value = vals[i];
            t11 = clock();
            rc = add_tree(strs->bt, key, value);
            t12 = clock();
            if (rc == 0)
            {
                t21 = clock();
                rc = add_avl_tree(strs->bta, key, value);
                t22 = clock();
                if (rc == 0)
                {
                    t31 = clock();
                    rc = add_hasho(&(strs->hto), key, value);
                    t32 = clock();
                    if (rc == 0)
                    {
                        t41 = clock();
                        rc = add_hashc(&(strs->htc), key, value);
                        t42 = clock();
                        if (rc == 0)
                        {
                            times->tbt += (double)(t12 - t11) / CLOCKS_PER_SEC;
                            times->tbta += (double)(t22 - t21) / CLOCKS_PER_SEC;
                            times->thto += (double)(t32 - t31) / CLOCKS_PER_SEC;
                            times->thtc += (double)(t42 - t41) / CLOCKS_PER_SEC;
                            times->abt += lastcmp_tree(strs->bt);
                            times->abta += lastcmp_avl_tree(strs->bta);
                            times->ahto += lastcmp_hasho(strs->hto);
                            times->ahtc += lastcmp_hashc(strs->htc);
                            times->rhto += lastcmp_hasho(strs->hto) > MAX_CMP;
                            times->rhtc += lastcmp_hashc(strs->htc) > 2 * MAX_CMP;
                            (times->adds)++;
                        }
                        else
                        {
                            del_tree(strs->bt, key);
                            del_avl_tree(strs->bta, key);
                            del_hasho(strs->hto, key);
                        }
                    }
                    else
                    {
                        del_tree(strs->bt, key);
                        del_avl_tree(strs->bta, key);
                    }
                }
                else
                    del_tree(strs->bt, key);
            }
        }
    }
    if (rc)
    {
        destroy_tree(strs->bt);
        destroy_avl_tree(strs->bta);
        destroy_hasho(strs->hto);
        destroy_hashc(strs->htc);
    }
    return rc;
}
