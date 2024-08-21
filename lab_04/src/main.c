#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "my_count.h"
#include "my_errors.h"
#include "my_lstack.h"
#include "my_astack.h"
#include "my_read.h"
#include "my_time.h"

int main(int argc, char **argv)
{

    TYPE_STACK val1 = -1;
    TYPE_STACK val2 = -1;
    TYPE_STACK act = -1;
    TYPE_STACK tmp = 0;
    astack *as = init_astack();
    lstack *ls = init_lstack();
    int rc = init_log();
    srand(time(0));
    if (rc)
        printf("Ошибка при создании журнала.\n");
    while (act && as && ls && rc == ERR_OK)
    {
        printf("Программа записывает в стек выражение и вычисляет его значение.\n");
        printf("Все вычисления проводятся по модулю %lld.\n", (long long)MAX_VAL_SIZE);
        printf("Допустимые операции: сложение '+', вычитание '-', умножениие '*'.\n");
        printf("Вычисления производятся слева направо.\n");
        printf("Числа и знаки операций записываются в стек слева направо.\n");
        printf("0 - Выход.\n");
        printf("1 - Добавление элемента в стеки.\n");
        printf("2 - Удаление элементов из стеков.\n");
        printf("3 - Очищение стеков.\n");
        printf("4 - Вычисление значения с использованием стека на массиве.\n");
        printf("5 - Вычисление значения с использованием стека на списке.\n");
        printf("6 - Вывод состояния стека и истории освобождения адресов.\n");
        printf("7 - Замер эффективности для данных стеков.\n");
        printf("8 - Замер эффективности для случайно сгенерированных данных.\n");
        do
        {
            rc = read_num(&act, 0, 8);
        } while (rc);
        switch (act)
        {
        case 0:
            printf("До свидания!\n");
            break;
        case 1:
            if (size_astack(as) < MAX_STACK_SIZE)
            {
                if (size_astack(as) & 1)
                    rc = read_sign(&tmp);
                else
                    rc = read_num(&tmp, 0, MAX_VAL_SIZE - 1);
                if (rc == ERR_OK)
                    rc = push_astack(as, tmp);
                if (rc == ERR_OK)
                    rc = push_lstack(ls, tmp);
                if (rc == ERR_MEM_ALLOC)
                    printf("Ошибка. Не удалось выделить память под очередной элемент стека.\n");
                if (rc == ERR_OK)
                {
                    val1 = -1;
                    val2 = -1;
                }
            }
            else
                printf("Ошибка. Стек заполнен.\n");
            break;
        case 2:
            rc = pop_astack(as, &tmp);
            if (rc == ERR_OK)
                rc = pop_lstack(ls, &tmp);
            if (rc == ERR_OK)
            {
                printf("Удалённое значение: ");
                if (size_astack(as) & 1)
                {
                    switch (tmp)
                    {
                    case 0:
                        printf("+\n");
                        break;
                    case 1:
                        printf("-\n");
                        break;
                    default:
                        printf("*\n");
                        break;
                    }
                }
                else
                    printf("%lld\n", (long long)tmp);
                val1 = val2 = -1;
            }
            else
                printf("Ошибка. Стеки пустые.\n");
            break;
        case 3:
            while (!(is_empty_astack(as)))
            {
                rc = pop_astack(as, &tmp);
                rc = pop_lstack(ls, &tmp);
            }
            val1 = val2 = -1;
            break;
        case 4:
            if (is_empty_astack(as))
                printf("Интересно, что вы тут считать собрались? Стек пустой.\n");
            else
            {
                if (val1 == -1)
                {
                    if (size_astack(as) & 1)
                    {
                        rc = count_astack(as, &val1);
                        if (rc == ERR_MEM_ALLOC)
                        {
                            printf("Ошибка. Не удалось выделить память под очередной элемент стека.\n");
                            printf("Вычисление не было завершено.\n");
                            val1 = -1;
                        }
                    }
                    else
                        printf("Ошибка. При вычислении выражение должно начинаться с числа.\n");
                }
                if (val1 != -1)
                    printf("Вычисленное значение: %lld\n", (long long)val1);
            }
            break;
        case 5:
            if (is_empty_lstack(ls))
                printf("Интересно, что вы тут считать собрались? Стек пустой.\n");
            else
            {
                if (val2 == -1)
                {
                    if (size_lstack(ls) & 1)
                    {
                        rc = count_lstack(ls, &val2);
                        if (rc == ERR_MEM_ALLOC)
                        {
                            printf("Ошибка. Не удалось выделить память под очередной элемент стека.\n");
                            printf("Вычисление не было завершено.\n");
                            val2 = -1;
                        }
                    }
                    else
                        printf("Ошибка. При вычислении выражение должно начинаться с числа.\n");
                }
                if (val2 != -1)
                    printf("Вычисленное значение: %lld\n", (long long)val2);
            }
            break;
        case 6:
            rc = print_state(ls);
            if (rc == ERR_MEM_ALLOC)
                printf("Ошибка при выделении памяти.\n");
            break;
        case 7:
            rc = check_time(as, ls);
            break;
        default:
            rc = check_time(NULL, NULL);
            break;
        }
        rc = ERR_OK;
    }
    if (!(as) || !(ls))
    {
        printf("Ошибка при создании стеков. Не удалось запустить программу.\n");
        rc = ERR_MEM_ALLOC;
    }
    else
        rc = ERR_OK;
    destroy_astack(as);
    destroy_lstack(ls);
    free_log();
    return ERR_OK;
}
