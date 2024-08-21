#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_struct.h"
#include "my_errors.h"

/**
 * Добавляет цифру в мантиссу.
 * На вход получает указатель на число и цифру.
 * Предполагается, что указатель корректный, а число неотрицательное.
 * Не включена в публичный интерфейс.
 */
void __add(struct bignum*, int32_t);
/**
 * Обрезает нули в конце, высчитывает количество цифр.
 * На вход получает указатель на структуру.
 * Предполагает, что указатель корректный.
 * Предполагает, что структура корректна, а мантисса не равна 0.
 * Не включена в публичный интерфейс.
 */
void __normalize(struct bignum*, bool);
/**
 * Проверяет на наличие нулевого элемента
 * На вход принимает указатели на 2 структуры.
 * Возвращает логическое значение, есть ли ноль среди этих чисел.
 * Не проверяет корректность значений структур.
 * Предполагает, что указатели корректные, разные и нет перекрытия памяти.
 * Если указатели совпадают, то функция корректно отработает.
 * Не включена в публичный интерфейс.
 */
bool __is_null(const struct bignum*, const struct bignum*);

int convert(struct bignum *this, const char *buf)
{
    this->exp = 0;
    for (size_t i = 0; i < __ARR_SIZE; ++i)
        *(this->mnt + i) = 0;
    this->msign = false;
    this->size = 0;

    int rc = OK;
    bool not_null = false;
    size_t cnt = 0;

    for (; isspace(*buf); ++buf);
    
    if (*buf == '-' || *buf == '+')
    {
        if (*buf == '-')
            this->msign = true;
        ++buf;
    }

    for (; isdigit(*buf); ++buf)
    {
        if (*buf - '0' || not_null) {
            __add(this, (int32_t)(*buf - '0'));
            ++cnt;
            not_null = true;
        }
    } 

    if (not_null)
        this->exp = cnt;

    if (*buf == '.')
    {
        ++buf;
        size_t acnt = 0;
        for (; isdigit(*buf); ++buf)
        {
            if (*buf - '0' || not_null) {
                __add(this, (int32_t)(*buf - '0'));
                if (!not_null)
                    this->exp = acnt;
                not_null = true;
            }
            --acnt;
        } 
    }

    if (not_null)
    {
        __normalize(this, false);
        if (isspace(*buf))
            ++buf;
        if (*buf == 'E' || *buf == 'e')
        {
            ++buf;
            if (isspace(*buf))
                ++buf;

            int32_t exp = 0;
            int32_t mult = 1;
            if (*buf == '-' || *buf == '+')
            {
                if (*buf == '-')
                    mult = -1;
                ++buf;
            }
            for (; isdigit(*buf); ++buf)
            {
                exp *= 10;
                exp += (int32_t)(*buf - '0');
            } 
            this->exp += exp * mult;
        }
    }
    else
        this->size = 1;

    return rc;
}

void __add(struct bignum *this, int32_t digit)
{
    int32_t tmp = digit;
    for (size_t i = 0; i < __ARR_SIZE; ++i)
    {
        *(this->mnt + i) *= 10;
        *(this->mnt + i) += tmp;
        tmp = *(this->mnt + i) / __MAX_ELEM_SIZE;
        *(this->mnt + i) %= __MAX_ELEM_SIZE;
    }
}

void __normalize(struct bignum *this, bool have_size)
{
    int32_t tmp;
    size_t tsize = this->size;
    while (*(this->mnt) % 10 == 0)
    {
        tmp = 0;
        for (size_t i = __ARR_SIZE; i; --i)
        {
            *(this->mnt + i - 1) += tmp * __MAX_ELEM_SIZE;
            tmp = *(this->mnt + i - 1) % 10;
            *(this->mnt + i - 1) /= 10;
            tsize--;
        }
    }
    
    for (size_t i = 0; i < __ARR_SIZE; ++i)
    {
        if (*(this->mnt + i))
        {
            this->size = i * __MAX_ELEM_DIGIT_SIZE;
            int32_t decr = *(this->mnt + i);
            for (; decr; decr /= 10, ++(this->size));
        }
    }

    if (have_size && this->size != tsize)
    {
        this->exp -= tsize - this->size;
    }
}

int multiple(struct bignum *dst, const struct bignum *first, const struct bignum *second)
{
    dst->exp = 0;
    for (size_t i = 0; i < __ARR_SIZE; ++i)
    {
        *(dst->mnt + i) = 0;
    }
    dst->msign = false;
    dst->size = 0;
    int rc = OK;

    if (__is_null(first, second)) 
        dst->size = 1;
    else
    {
        dst->msign = (first->msign && !second->msign) || (!first->msign && second->msign);
        dst->exp = first->exp + second->exp;
        dst->size = first->size + second->size;
        if (abs(dst->exp) > __MAX_EXP_ABS)
        {
            rc = EXP_OVERFLOW;
            printf("Ошибка: переполнение экспоненты при вычислении произведения.\n");
        }
        else
        {
            size_t arrlen = 5;
            int64_t res;
            for (size_t i = 0; i < arrlen; ++i)
            {
                for (size_t j = 0; j < arrlen; ++j)
                {  
                    res = *(dst->mnt + i + j) + (int64_t)*(first->mnt + i) * *(second->mnt + j);
                    *(dst->mnt + i + j) = (int32_t)(res % __MAX_ELEM_SIZE);
                    *(dst->mnt + i + j + 1) += (int32_t)(res / __MAX_ELEM_SIZE);
                }
            }

            __normalize(dst, true);

            if (abs(dst->exp) > __MAX_EXP_ABS)
            {
                rc = EXP_OVERFLOW;
                printf("Ошибка: переполнение экспоненты при вычислении произведения.\n");
            }

            if (dst->size > __MAX_DIGIT_SIZE)
            {
                int32_t tmp = 0;
                for (; dst->size > __MAX_DIGIT_SIZE; --(dst->size))
                {
                    tmp = 0;
                    for (size_t i = __ARR_SIZE; i; --i)
                    {
                        *(dst->mnt + i - 1) += tmp * __MAX_ELEM_SIZE;
                        tmp = *(dst->mnt + i - 1) % 10;
                        *(dst->mnt + i - 1) /= 10;
                    }
                }

                if (tmp >= 5) 
                {
                    *(dst->mnt) += tmp >= 5;
                    tmp = 0;
                    for (size_t j = 0; j < __ARR_SIZE >> 1; ++j)
                    {
                        *(dst->mnt + j) += tmp;
                        tmp = *(dst->mnt + j) / __MAX_ELEM_SIZE;
                        *(dst->mnt + j) %= __MAX_ELEM_SIZE;
                    }
                }

                if (*(dst->mnt + 5))
                {
                    ++(dst->exp);
                    if (abs(dst->exp) > __MAX_EXP_ABS)
                    {
                        rc = EXP_OVERFLOW;
                        printf("Ошибка: переполнение экспоненты при вычислении произведения.\n");
                    }
                }

                __normalize(dst, false);
            }
        }
    }

    return rc;
}

bool __is_null(const struct bignum *first, const struct bignum *second)
{
    return (first->size == 1 && *(first->mnt) == 0) || (second->size == 1 && *(second->mnt) == 0);
}