#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "my_input.h"
#include "my_model.h"
#include "my_time.h"

int main(void)
{
    int act = -1;
    srand(time(0));
    params_t times = {1, 5, 0, 3, 0, 4, 0, 1};
    int rc = QUEUE_OK;
    printf("Программа моделирует работу обслуживающегого аппарата с 2 очередьми заявок двух типов.\n");
    printf("Доступны операции добавления, удаления по имени или по дате (не позднее определённой даты).\n");
    printf("Также доступен просмотр элементов и вывод графа в файл для графического представления.\n");
    while (act)
    {
        printf("Интервалы времени:\n");
        printf("Время прихода заявки 1-го типа: %.6lf-%.6lf\n", times.t11, times.t12);
        printf("Время прихода заявки 2-го типа: %.6lf-%.6lf\n", times.t21, times.t22);
        printf("Время обработки заявки 1-го типа: %.6lf-%.6lf\n", times.t31, times.t32);
        printf("Время обработки заявки 2-го типа: %.6lf-%.6lf\n", times.t41, times.t42);
        printf("0 - Выход.\n");
        printf("1 - Задание параметров моделирования.\n");
        printf("2 - Моделирование для очередей на массиве.\n");
        printf("3 - Моделирование для очередей на списке.\n");
        printf("4 - Сравнение эффективности работы очередей.\n");
        read_num(&act, 0, 4);
        switch (act)
        {
        case 0:
            printf("До свидания!\n");
            break;
        case 1:
            read_params(&times);
            break;
        case 2:
            rc = simulate(&times, ARR_QUEUE, NULL);
            break;
        case 3:
            rc = simulate(&times, LST_QUEUE, NULL);
            break;
        default:
            rc = time_test(&times);
            break;
        }
    }
    if (rc)
        rc = 0;
    return rc;
}