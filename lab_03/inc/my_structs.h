#ifndef __MY_STRUCTS_H__
#define __MY_STRUCTS_H__

typedef struct special sp_m;
typedef struct usual us_m;

/**
 * Хранит разреженную матрицу.
 */
struct special
{
    int rows, columns; // Количество строк и столбцов.
    int el_cnt, r_cnt; // Количество ненулевых элементов и строк, содержащих ненулевые элементы.
    int *vals; // Значения.
    int *cols; // Столбцы.
    struct
    {
        int row;
        int start;
    } *rows_st; // Строки и индексы их начал в двух предыдущих массивах.
};

struct usual
{
    int rows, columns; // Количество строк и столбцов.
    int *vals; // Элементы матрицы.
}; 

/**
 * Инициализация разреженной матрицы.
 * Возвращает пустую разреженную матрицу.
 */
sp_m spm_init(void);

/**
 * Инициализация матрицы.
 * Возвращает пустую матрицу.
 */
us_m usm_init(void);

/**
 * Заполнение обычной матрицы на основе разреженной.
 * Принимает на вход указатели на обычную и разреженную матрицы.
 * Возвращает код ошибки.
 * Корректность указателей не проверяется.
 */
int create_usual(us_m *us, const sp_m *sp);

#endif // __MY_STRUCTS_H__