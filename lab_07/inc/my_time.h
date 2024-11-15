/**
 * Данный модуль содержит функции замера времени и памяти при вставке и поиске.
 */
#ifndef __MY_TIME_H__
#define __MY_TIME_H__

#include "my_avl_tree.h"
#include "my_tree.h"
#include "my_hasho.h"
#include "my_hashc.h"

/**
 * Данная структура хранит указатели на все 4 структуры, для которых идёт замер эффективности.
 * Создана только для сокращения передаваемых в функцию параметров.
 */
typedef struct
{
    tree *bt;
    avl_tree *bta;
    hasho *hto;
    hashc *htc;
} structs_t;

/**
 * Данная структура содержит времена вставки (t*), количество сравнений (a*), а таже число реструктуризаций (r*).
 * adds - число вставок.
 * Окончания переменных:
 * bt - двоичное дерево поиска;
 * bta - АВЛ дерево;
 * hto - хеш-таблица с открытым хешированием;
 * htc - хеш-таблица с закрытым хешированием.
 */
typedef struct
{
    double tbt, tbta, thto, thtc;
    double abt, abta, ahto, ahtc;
    int rhto, rhtc;
    int adds;
} times_t;


/**
 * Вывод эффективности вставки.
 * На вход принимает структуру из указателей на структуры и времена работы.
 * Возвращает код ошибки.
 * Корректность указателей не проверяется.
 */
int check_add(structs_t, times_t);

/**
 * Замер эффективности поиска.
 * На вход принимает структуру из указателей на структуры.
 * Возвращает код ошибки.
 * Корректность указателей не проверяется.
 */
int check_time(structs_t);

#endif // __MY_TIME_H__