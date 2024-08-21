/**
 * Данный модуль содержит тип данных очередь и реализуемые над ним операции.
 */
#ifndef __MY_QUEUE_H__
#define __MY_QUEUE_H__

#include <stddef.h>
#include <stdbool.h>

#ifndef QUEUE_TYPE
#define QUEUE_TYPE void*
#endif

#ifndef QUEUE_MAX_SIZE
#define QUEUE_MAX_SIZE 10000
#endif

/**
 * Для выбора реализации очереди.
 */
#define ARR_QUEUE 0
#define LST_QUEUE 1

/**
 * Возможные ошибки.
 */
#define QUEUE_OK             0
#define QUEUE_ERR_MEM_ALLOC -1
#define QUEUE_ERR_OVERFLOW  -2
#define QUEUE_ERR_EMPTY     -3
#define QUEUE_ERR_NULLPTR   -4

/**
 * Обертка для различных реализаций.
 */
typedef struct my_queue my_queue;

/**
 * Указатели на функции, выполняющие основны действия.
 * push_f - добавление элемента в очередь.
 * pop_f - удаление элемента из очереди.
 * destroy_f - удаление очереди.
 * size_f - размер очереди.
 * memsize_f - используемая память.
 * is_empty_f - проверка на пустоту.
 * clear_f - очистка в очереди.
 */
typedef int push_f(void*, QUEUE_TYPE);
typedef int pop_f(void*, QUEUE_TYPE*);
typedef void destroy_f(my_queue*);
typedef size_t size_f(void*);
typedef size_t memsize_f(void*);
typedef bool is_empty_f(void*);
typedef void clear_f(void*);

/**
 * Реализация обертки - указатель на саму очередь и функции работы с ней.
 */
struct my_queue
{
    void *queue;
    push_f *push;
    pop_f *pop;
    destroy_f *destroy;
    size_f *size;
    memsize_f *memsize;
    is_empty_f *is_empty;
    clear_f *clear;
};

/**
 * Создание очереди.
 * На вход принимает число, обозначающее номер реализации (заданы выше макросами).
 * При некорректной работе возвращает нулевой указатель.
 */
my_queue *init(int mode);

#endif //__MY_QUEUE_H__