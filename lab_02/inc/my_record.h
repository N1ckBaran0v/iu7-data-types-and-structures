/**
 * Этот модуль содержит структуры. 
 */
#ifndef __MY_RECORD_H__

#define __MY_RECORD_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_STR_SIZE 20
#define MAX_ARR_SIZE 50

/**
 * Структура с вариантной частью, хранящая данные о студенте.
 */
typedef struct
{
    char surname[MAX_STR_SIZE + 1]; // Фамилия
    char name[MAX_STR_SIZE + 1]; // Имя
    bool isMale; // Пол
    char group[MAX_STR_SIZE + 1]; // Группа
    int16_t age; // Возраст
    float mark; // Средний балл
    int16_t year; // Год поступления
    enum
    {
        HOME, 
        HOSTELL
    } typeofplace; // Подсказка как обрабатывать объединение
    union // Вариантная часть
    {
        struct // Если студент живет дома
        {
            char street[MAX_STR_SIZE + 1]; // Улица
            int16_t house; // Дом
            int16_t flat; // Квартира
        } home;
        struct // Если живет в общежитии
        {
            int16_t num; // Общежитие
            int16_t room; // Комната
        } hostel;
    } placeinfo;
} student;

/**
 * Хранит только фамилию (ключ) и положение в исходной таблице. 
 */
typedef struct 
{
    size_t pos; // Положение в исходной таблице
    char surname[MAX_STR_SIZE + 1]; // Фамилия
} key;

/**
 * Хранит потоки ввода и вывода.
 * Нужна для сокращения числа аргументов у функций чтения.
 */
typedef struct 
{
    FILE *input; // Поток ввода
    FILE *output; // Поток вывода
} iofiles;

#endif //__MY_RECORD_H__