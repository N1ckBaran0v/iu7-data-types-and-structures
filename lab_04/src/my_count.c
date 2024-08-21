#include "my_astack.h"
#include "my_lstack.h"
#include "my_count.h"
#include "my_read.h"

TYPE_STACK plus(TYPE_STACK first, TYPE_STACK second);
TYPE_STACK minus(TYPE_STACK first, TYPE_STACK second);
TYPE_STACK multiple(TYPE_STACK first, TYPE_STACK second);

int count_astack(astack *stack, TYPE_STACK *ans)
{
    int rc = ERR_OK;
    astack *left = init_astack();
    astack *right = copy_astack(stack);
    if (left && right)
    {
        reverse_astack(right);
        TYPE_STACK lsign, rsign, rval, lval;
        TYPE_STACK(*operation[3])
        (TYPE_STACK, TYPE_STACK) = {plus, minus, multiple};
        while (rc == ERR_OK && size_astack(left) + size_astack(right) > 1)
        {
            lsign = rsign = rval = lval = -1;
            rc = pop_astack(right, &rval);
            if (!(is_empty_astack(left)))
                rc = pop_astack(left, &lsign);
            if (!(is_empty_astack(right)))
                rc = pop_astack(right, &rsign);
            if (lsign == -1)
            {
                rc = push_astack(left, rval);
                if (rc == ERR_OK)
                    rc = push_astack(left, rsign);
            }
            else if (rsign == -1)
            {
                rc = pop_astack(left, &lval);
                rval = operation[lsign](lval, rval);
                rc = push_astack(right, rval);
            }
            else
            {
                if ((lsign < 2 && rsign < 2) || lsign >= rsign)
                {
                    rc = pop_astack(left, &lval);
                    rval = operation[lsign](lval, rval);
                    rc = push_astack(right, rsign);
                    if (rc == ERR_OK)
                        rc = push_astack(right, rval);
                }
                else
                {
                    rc = push_astack(left, lsign);
                    if (rc == ERR_OK)
                        rc = push_astack(left, rval);
                    if (rc == ERR_OK)
                        rc = push_astack(left, rsign);
                }
            }
        }
        if (rc == ERR_OK)
            rc = pop_astack(right, ans);
        destroy_astack(right);
        destroy_astack(left);
    }
    else
        rc = ERR_MEM_ALLOC;
    return rc;
}

int count_lstack(lstack *stack, TYPE_STACK *ans)
{
    int rc = ERR_OK;
    lstack *left = init_lstack();
    lstack *right = copy_lstack(stack);
    if (left && right)
    {
        reverse_lstack(right);
        TYPE_STACK lsign, rsign, rval, lval;
        TYPE_STACK(*operation[3])
        (TYPE_STACK, TYPE_STACK) = {plus, minus, multiple};
        while (rc == ERR_OK && size_lstack(left) + size_lstack(right) > 1)
        {
            lsign = rsign = rval = lval = -1;
            rc = pop_lstack(right, &rval);
            if (!(is_empty_lstack(left)))
                rc = pop_lstack(left, &lsign);
            if (!(is_empty_lstack(right)))
                rc = pop_lstack(right, &rsign);
            if (lsign == -1)
            {
                rc = push_lstack(left, rval);
                if (rc == ERR_OK)
                    rc = push_lstack(left, rsign);
            }
            else if (rsign == -1)
            {
                rc = pop_lstack(left, &lval);
                rval = operation[lsign](lval, rval);
                rc = push_lstack(right, rval);
            }
            else
            {
                if ((lsign < 2 && rsign < 2) || lsign >= rsign)
                {
                    rc = pop_lstack(left, &lval);
                    rval = operation[lsign](lval, rval);
                    rc = push_lstack(right, rsign);
                    if (rc == ERR_OK)
                        rc = push_lstack(right, rval);
                }
                else
                {
                    rc = push_lstack(left, lsign);
                    if (rc == ERR_OK)
                        rc = push_lstack(left, rval);
                    if (rc == ERR_OK)
                        rc = push_lstack(left, rsign);
                }
            }
        }
        if (rc == ERR_OK)
            rc = pop_lstack(right, ans);
        destroy_lstack(right);
        destroy_lstack(left);
    }
    else
        rc = ERR_MEM_ALLOC;
    return rc;
}

TYPE_STACK plus(TYPE_STACK first, TYPE_STACK second)
{
    return (first + second) % MAX_VAL_SIZE;
}

TYPE_STACK minus(TYPE_STACK first, TYPE_STACK second)
{
    return (first - second + MAX_VAL_SIZE) % MAX_VAL_SIZE;
}

TYPE_STACK multiple(TYPE_STACK first, TYPE_STACK second)
{
    return (first * second) % MAX_VAL_SIZE;
}
