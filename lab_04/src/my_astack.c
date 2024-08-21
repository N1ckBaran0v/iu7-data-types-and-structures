#include <stdlib.h>
#include <memory.h>

#include "my_astack.h"

struct astack
{
    size_t size;
    TYPE_STACK *arr;
};

astack *init_astack(void)
{
    astack *ret = malloc(sizeof(astack));
    if (ret)
    {
        ret->size = 0;
        ret->arr = malloc(MAX_STACK_SIZE * sizeof(TYPE_STACK));
        if (!(ret->arr))
            free(ret);
    }
    return ret;
}

astack *copy_astack(astack *stack)
{
    astack *ret = NULL;
    if (stack)
    {
        ret = init_astack();
        if (ret)
        {
            ret->size = stack->size;
            memcpy(ret->arr, stack->arr, ret->size * sizeof(TYPE_STACK));
        }
    }
    return ret;
}

int push_astack(astack *stack, const TYPE_STACK value)
{
    int ret = ERR_OK;
    if (!(stack))
        ret = ERR_NULLPTR;
    else if (stack->size >= MAX_STACK_SIZE)
        ret = ERR_STACK_OVERFLOW;
    else
        stack->arr[(stack->size)++] = value;
    return ret;
}

int pop_astack(astack *stack, TYPE_STACK *value)
{
    int ret = ERR_OK;
    if (!(stack) || !(value))
        ret = ERR_NULLPTR;
    else if (stack->size == 0)
        ret = ERR_EMPTY_STACK;
    else
        *value = stack->arr[--(stack->size)];
    return ret;
}

size_t memsize_astack(astack *stack)
{
    size_t ans = 0;
    if (stack)
        ans += sizeof(*stack) + MAX_STACK_SIZE * sizeof(TYPE_STACK);
    return ans;
}

size_t size_astack(astack *stack)
{
    return stack ? stack->size : 0;
}

bool is_empty_astack(astack *stack)
{
    return stack ? stack->size == 0 : true;
}

void reverse_astack(astack *stack)
{
    TYPE_STACK tmp = 0;
    for (size_t i = 0; i < (stack->size >> 1); ++i)
    {
        tmp = stack->arr[i];
        stack->arr[i] = stack->arr[stack->size - i - 1];
        stack->arr[stack->size - i - 1] = tmp;
    }
}

void destroy_astack(astack *stack)
{
    if (stack)
        free(stack->arr);
    free(stack);
}
