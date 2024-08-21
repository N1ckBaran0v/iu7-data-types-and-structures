/**
 * Данный модуль содержит структуру даты и данные о месяцах.
 * Изначально должен был содержать любую информацию, которая нужна была в разных местах программы,
 * из-за чего файл назван my_helps.h, а не my_dates.h.
 */
#ifndef __MY_HELPS_H__
#define __MY_HELPS_H__

/**
 * Имена месяцев как строки.
 */
static char *const month_name[] = 
{
    "January", "February", "March", "April",
    "May", "June", "July", "August", "September", 
    "October", "November", "December"
};

/**
 * Количество дней в месяце (если год не високосный).
 */
static const int max_day[] = 
{
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/**
 * Структура, хранящая дату в виде 3 чисел: дня, месяца и года.
 */
typedef struct
{
    int day, month, year;
} date;

#endif // __MY_HELPS_H__