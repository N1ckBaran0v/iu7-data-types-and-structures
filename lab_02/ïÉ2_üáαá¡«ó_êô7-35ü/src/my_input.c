#include <ctype.h>

#include "../inc/my_input.h"
#include "../inc/my_errors.h"

int read_str(iofiles *io, char *buf, const int alpha)
{
    size_t size = 0;
    int rc = read_buf(io, buf, &size, MAX_STR_SIZE);
    for (size_t i = 0; i < size && rc == ERR_OK; ++i)
    {
        if (((alpha == NAME_ALPHA && !(isalpha(buf[i]))) || (alpha == GROUP_ALPHA && !(isalnum(buf[i])))) \
              && buf[i] != '-')
        { 
            rc = ERR_INCORRECT_VALUE;
            if (io->output)
            {
                for (size_t j = 0; j < i; ++j)
                    fprintf(io->output, "-");
                fprintf(io->output, "^\n");
                fprintf(io->output, "Ошибка. Встречен некорректный символ.\n");
            }
        }
    }
    return rc;
}

int read_num(iofiles *io, int16_t *val, const int type)
{
    char buf[NUM_BUF_SIZE + 1];
    size_t size = 0;
    int rc = read_buf(io, buf, &size, NUM_BUF_SIZE);
    *val = 0;
    for (size_t i = 0; i < size && rc == ERR_OK; ++i)
    {
        if (!(isdigit(buf[i])))
        { 
            rc = ERR_INCORRECT_VALUE;
            if (io->output)
            {
                for (size_t j = 0; j < i; ++j)
                    fprintf(io->output, "-");
                fprintf(io->output, "^\n");
                fprintf(io->output, "Ошибка. Встречен некорректный символ.\n");
            }
        }
        else
            *val = (*val * 10) + (int16_t)(buf[i] - '0');
    }
    if (rc == ERR_OK)
    {
        INIT_MIN_ARR;
        INIT_MAX_ARR;
        if (*val < MIN_ARR[type] || *val > MAX_ARR[type])
        {
            rc = ERR_INCORRECT_VALUE;
            if (io->output)
                fprintf(io->output, "Ошибка. Число не входит в допустимый диапазон значений.\n");
        }
    }
    return rc;
}

int read_float(iofiles *io, float *val)
{
    char buf[NUM_BUF_SIZE + 1];
    size_t size = 0;
    int rc = read_buf(io, buf, &size, NUM_BUF_SIZE);
    for (size_t i = 0; i < size && rc == ERR_OK; ++i)
    {
        if (i != 1 && !(isdigit(buf[i])))
        { 
            rc = ERR_INCORRECT_VALUE;
            if (io->output)
            {
                for (size_t j = 0; j < i; ++j)
                    fprintf(io->output, "-");
                fprintf(io->output, "^\n");
                fprintf(io->output, "Ошибка. Встречен некорректный символ.\n");
            }
        }
        else if (i == 1 && buf[i] != '.')
        {
            rc = ERR_INCORRECT_VALUE;
            if (io->output)
            {
                fprintf(io->output, "-^\n");
                fprintf(io->output, "Ошибка. Встречен некорректный символ.\n");
            }
        }
        else if (i == 0)
            *val = (float)(buf[i] - '0');
        else if (i > 1)
            *val = *val + ((float)(buf[i] - '0') * (0.1 * (i == 2) + 0.01 * (i == 3))); 
    }
    if (rc == ERR_OK)
    {
        if (*val < MIN_MARK - EPS || *val > MAX_MARK + EPS)
        {
            rc = ERR_INCORRECT_VALUE;
            if (io->output)
                fprintf(io->output, "Ошибка. Число не входит в допустимый диапазон значений.\n");
        }
    }
    return rc;
}

int read_buf(iofiles *io, char *buf, size_t *size, size_t max_buf)
{
    int rc = (io->input == NULL);
    if (io->output)
        fprintf(io->output, "----|----|----|----|\n");
    char tmp;
    if (rc == ERR_OK)
    {
        *size = 0;
        tmp = fgetc(io->input);
    }
    while (tmp && tmp != '\n' && tmp != EOF && io->input != NULL)
    {
        if (rc == ERR_OK)
        {
            buf[*size] = tmp;
            ++(*size);
        }
        tmp = fgetc(io->input);
        if (*size > max_buf)
        {
            rc = ERR_ARR_OVERFLOW;
        }
    }
    if (rc == ERR_ARR_OVERFLOW)
    {
        if (io->output)
            fprintf(io->output, "Ошибка. Переполнение буфера при вводе.\n");
    }
    else if (*size == 0)
    {
        rc = ERR_EMPTY_ARRAY;
        if (io->output)
            fprintf(io->output, "Ошибка. Пустой ввод.\n");
    }
    else if (rc == ERR_OK)
        buf[*size] = '\0';
    return rc;
}