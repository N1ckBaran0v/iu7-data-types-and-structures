#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "../inc/my_convert.h"
#include "../inc/my_errors.h"

int unum(const char *buf, int *dst)
{
    *dst = 0;
    bool sign = false;
    int size = 0;
    if (*buf == '-')
    {
        sign = true;
        ++buf;
    }
    for (; buf && size > 0 && size < 7; ++buf)
    {
        if (isdigit(*buf))
        {
            *dst = *dst * 10 + (*buf - '0'); 
            ++size;
        }
        else
            size = -1;
    }
    *dst *= -1 * sign;
    return size;
}

int uf(const char *buf, float *dst)
{
    int rc = ERR_ARR_OVERFLOW;
    if (strlen(buf) < 5)
    {
        rc = ERR_OK;
        *dst = 0;
        int decr = 1;
        bool was_dot = false;
        for (; buf && rc == ERR_OK; ++buf)
        {
            if (isdigit(*buf))
            {
                if (was_dot)
                    decr *= 10;
                *dst += (float)(*buf - '0') / decr;
            }
            else if (*buf == '.' && !was_dot)
                was_dot = true;
        }
        if (rc == ERR_OK && (*dst > 5 || *dst < 2))
            rc = ERR_INCORRECT_VALUE;
    }
    return rc;
}