#include <stdlib.h>
#include <memory.h>

#include "my_lstack.h"

typedef struct my_leaf my_leaf;

struct log_s
{
    void *ptr;
    bool is_add;
};

struct log_s *alog = NULL;
static size_t lsize = 0;
static size_t blsize = 0;
static bool need_log = true;

struct my_leaf
{
    TYPE_STACK value;
    my_leaf *next;
};

struct lstack
{
    size_t size;
    my_leaf *head;
};

my_leaf *create_leaf(const TYPE_STACK value, my_leaf *next);
my_leaf *destroy_leaf(my_leaf *leaf);
int add_log(void *ptr, bool is_add);

lstack *init_lstack(void)
{
    lstack *ret = malloc(sizeof(lstack));
    if (ret)
    {
        ret->size = 0;
        ret->head = NULL;
    }
    return ret;
}

lstack *copy_lstack(lstack *stack)
{
    int rc = ERR_OK;
    lstack *ret = NULL;
    if (stack)
    {
        ret = init_lstack();
        if (ret)
        {
            ret->size = stack->size;
            ret->head = NULL;
            my_leaf *ptr = stack->head, *ptr2 = NULL;
            while (ptr)
            {
                if (ptr2)
                {
                    ptr2->next = create_leaf(ptr->value, NULL);
                    if (ptr2->next)
                    {
                        ptr2 = ptr2->next;
                        ptr = ptr->next;
                        rc = add_log(ptr2, true);
                        if (rc == ERR_MEM_ALLOC)
                        {
                            destroy_lstack(ret);
                            ret = NULL;
                            ptr = NULL;
                        }
                    }
                    else
                    {
                        destroy_lstack(ret);
                        ret = NULL;
                        ptr = NULL;
                    }
                }
                else
                {
                    ptr2 = create_leaf(ptr->value, NULL);
                    if (ptr2)
                    {
                        ptr = ptr->next;
                        ret->head = ptr2;
                        rc = add_log(ptr2, true);
                        if (rc == ERR_MEM_ALLOC)
                        {
                            destroy_lstack(ret);
                            ret = NULL;
                            ptr = NULL;
                        }
                    }
                    else
                    {
                        destroy_lstack(ret);
                        ret = NULL;
                        ptr = NULL;
                    }
                }
            }
        }
    }
    return ret;
}

int push_lstack(lstack *stack, const TYPE_STACK value)
{
    int ret = ERR_OK;
    if (!(stack))
        ret = ERR_NULLPTR;
    else if (stack->size >= MAX_STACK_SIZE)
        ret = ERR_STACK_OVERFLOW;
    else
    {
        my_leaf *tmp = create_leaf(value, stack->head);
        if (tmp)
        {
            ret = add_log(tmp, true);
            if (ret)
                free(tmp);
            else
            {
                // printf("push: %p\n", (void*)tmp);
                stack->head = tmp;
                ++(stack->size);
            }
        }
        else
            ret = ERR_MEM_ALLOC;
    }
    return ret;
}

int pop_lstack(lstack *stack, TYPE_STACK *value)
{
    int ret = ERR_OK;
    if (!(stack) || !(value))
        ret = ERR_NULLPTR;
    else if (stack->size == 0)
        ret = ERR_EMPTY_STACK;
    else
    {
        *value = stack->head->value;
        ret = add_log(stack->head, false);
        if (ret == ERR_OK)
        {
            // printf("pop: %p\n", (void*)stack->head);
            stack->head = destroy_leaf(stack->head);
            --(stack->size);
        }
    }
    return ret;
}

size_t memsize_lstack(lstack *stack)
{
    size_t ans = 0;
    if (stack)
    {
        ans = sizeof(*stack);
        if (stack->head)
            ans += stack->size * sizeof(*(stack->head));
    }
    return ans;
}

size_t size_lstack(lstack *stack)
{
    return stack ? stack->size : 0;
}

bool is_empty_lstack(lstack *stack)
{
    return stack ? stack->size == 0 : true;
}

void reverse_lstack(lstack *stack)
{
    my_leaf *prev = NULL, *curr = stack->head, *next = NULL;
    while (curr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    stack->head = prev;
}

void destroy_lstack(lstack *stack)
{
    if (stack)
        while (stack->head)
            stack->head = destroy_leaf(stack->head);
    free(stack);
}

int init_log(void)
{
    int rc = ERR_OK;
    free(alog);
    alog = malloc(sizeof(struct log_s));
    lsize = 0;
    blsize = 0;
    if (alog)
        blsize = 1;
    else
        rc = ERR_MEM_ALLOC;
    return rc;
}

int add_log(void *ptr, bool is_add)
{
    int rc = ERR_OK;
    if (need_log)
    {
        if (lsize >= blsize)
        {
            struct log_s *tmp = realloc(alog, blsize * 2 * sizeof(struct log_s));
            if (tmp)
            {
                alog = tmp;
                blsize <<= 1;
            }
            else
                rc = ERR_MEM_ALLOC;
        }
        if (rc == ERR_OK)
        {
            alog[lsize].ptr = ptr;
            alog[lsize].is_add = is_add;
            lsize++;
        }
    }
    // printf("log: %p\nsize: %lld\n", (void*)alog, (long long)(blsize * sizeof(struct log_s)));
    return rc;
}

int print_state(lstack *stack)
{
    int rc = ERR_OK;
    if (stack)
    {
        if (stack->head)
        {
            my_leaf *curr = stack->head;
            printf("-------------------------------\n");
            printf("|Номер|Значение|         Адрес|\n");
            printf("-------------------------------\n");
            for (int i = 0; curr; ++i, curr = curr->next)
            {
                if ((i & 1) == (stack->size & 1))
                    printf("|%5d|%8c|%14p|\n", i, (curr->value == 0) ? '+' : ((curr->value == 1) ? '-' : '*'), (void *)curr);
                else
                    printf("|%5d|%8lld|%14p|\n", i, (long long)curr->value, (void *)curr);
                printf("-------------------------------\n");
            }
        }
        else
            printf("Стек пустой.\n");
        printf("История выделения и освобождения памяти не ведётся во время измерения эффективности работы программы.\n");
        if (lsize)
        {
            printf("История выделения и освобождения памяти.\n");
            printf("-----------------------------\n");
            printf("|         Адрес|    Операция|\n");
            printf("-----------------------------\n");
            for (size_t i = 0; i < lsize; ++i)
            {
                if (alog[i].is_add)
                    printf("|%14p|   Выделение|\n", alog[i].ptr);
                else
                    printf("|%14p|Освобождение|\n", alog[i].ptr);
                printf("-----------------------------\n");
            }
            free_log();
            rc = init_log();
        }
        else
            printf("Память не выделялась и не освобождалась с момента последнего просмотра.\n");
    }
    else
        rc = ERR_NULLPTR;
    return rc;
}

void free_log(void)
{
    free(alog);
    alog = NULL;
    lsize = 0;
    blsize = 0;
}

my_leaf *create_leaf(const TYPE_STACK value, my_leaf *next)
{
    my_leaf *ret = malloc(sizeof(my_leaf));
    if (ret)
    {
        ret->value = value;
        ret->next = next;
    }
    return ret;
}

my_leaf *destroy_leaf(my_leaf *leaf)
{
    my_leaf *ret = NULL;
    if (leaf)
    {
        ret = leaf->next;
        free(leaf);
    }
    return ret;
}

void set_log(bool val)
{
    need_log = val;
}