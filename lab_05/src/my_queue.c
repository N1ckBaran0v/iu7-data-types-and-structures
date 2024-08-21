#include <stdlib.h>

#include "my_queue.h"

typedef void *_init(void);

typedef struct _aqueue _aqueue;

struct _aqueue
{
    size_t size;
    size_t begin;
    size_t end;
    QUEUE_TYPE *arr;
};

_init _ainit;
push_f _apush;
pop_f _apop;
destroy_f _adestroy;
size_f _asize;
memsize_f _amemsize;
is_empty_f _ais_empty;
clear_f _aclear;

typedef struct _lqueue _lqueue;

typedef struct _leaf _leaf;

struct _leaf
{
    _leaf *next;
    QUEUE_TYPE value;
};

struct _lqueue
{
    size_t size;
    _leaf *head;
    _leaf *tail;
};

_init _linit;
push_f _lpush;
pop_f _lpop;
destroy_f _ldestroy;
size_f _lsize;
memsize_f _lmemsize;
is_empty_f _lis_empty;
clear_f _lclear;

my_queue *init(int mode)
{
    my_queue *ret = malloc(sizeof(my_queue));
    if (ret)
    {
        switch (mode)
        {
        case ARR_QUEUE:
            ret->queue = _ainit();
            if (ret->queue)
            {
                ((_aqueue *)ret->queue)->begin = 0;
                ((_aqueue *)ret->queue)->end = 0;
                ((_aqueue *)ret->queue)->size = 0;
                ret->push = _apush;
                ret->pop = _apop;
                ret->destroy = _adestroy;
                ret->size = _asize;
                ret->memsize = _amemsize;
                ret->is_empty = _ais_empty;
                ret->clear = _aclear;
            }
            else
            {
                free(ret);
                ret = NULL;
            }
            break;
        case LST_QUEUE:
            ret->queue = _linit();
            if (ret->queue)
            {
                ((_lqueue *)ret->queue)->head = NULL;
                ((_lqueue *)ret->queue)->tail = NULL;
                ((_lqueue *)ret->queue)->size = 0;
                ret->push = _lpush;
                ret->pop = _lpop;
                ret->destroy = _ldestroy;
                ret->size = _lsize;
                ret->memsize = _lmemsize;
                ret->is_empty = _lis_empty;
                ret->clear = _lclear;
            }
            else
            {
                free(ret);
                ret = NULL;
            }
            break;
        default:
            free(ret);
            ret = NULL;
            break;
        }
    }
    return ret;
}

void *_ainit(void)
{
    _aqueue *ret = malloc(sizeof(_aqueue));
    if (ret)
    {
        ret->arr = malloc(QUEUE_MAX_SIZE * sizeof(QUEUE_TYPE));
        if (ret->arr)
            ret->begin = ret->end = ret->size = 0;
        else
        {
            free(ret);
            ret = NULL;
        }
    }
    return ret;
}

int _apush(void *queue, QUEUE_TYPE value)
{
    int rc = QUEUE_OK;
    if (queue)
    {
        if (((_aqueue *)queue)->size < QUEUE_MAX_SIZE)
        {
            ((_aqueue *)queue)->arr[((_aqueue *)queue)->end] = value;
            ((_aqueue *)queue)->end = (((_aqueue *)queue)->end + 1) % QUEUE_MAX_SIZE;
            (((_aqueue *)queue)->size)++;
        }
        else
            rc = QUEUE_ERR_OVERFLOW;
    }
    else
        rc = QUEUE_ERR_NULLPTR;
    return rc;
}

int _apop(void *queue, QUEUE_TYPE *value)
{
    int rc = QUEUE_OK;
    if (queue && value)
    {
        if (((_aqueue *)queue)->size)
        {
            *value = ((_aqueue *)queue)->arr[((_aqueue *)queue)->begin];
            ((_aqueue *)queue)->begin = (((_aqueue *)queue)->begin + 1) % QUEUE_MAX_SIZE;
            (((_aqueue *)queue)->size)--;
        }
        else
            rc = QUEUE_ERR_EMPTY;
    }
    else
        rc = QUEUE_ERR_NULLPTR;
    return rc;
}

void _adestroy(my_queue *queue)
{
    if (queue)
    {
        if (queue->queue)
        {
            free(((_aqueue *)queue->queue)->arr);
            free(queue->queue);
        }
        free(queue);
    }
}

size_t _asize(void *queue)
{
    return (queue) ? ((_aqueue *)queue)->size : 0;
}

size_t _amemsize(void *queue)
{
    return (queue) ? QUEUE_MAX_SIZE * sizeof(QUEUE_TYPE) + sizeof(_aqueue) : 0;
}

bool _ais_empty(void *queue)
{
    return (queue) ? (((_aqueue *)queue)->size == 0) : true;
}

void _aclear(void *queue)
{
    if (queue)
        ((_aqueue *)queue)->size = ((_aqueue *)queue)->begin = ((_aqueue *)queue)->end = 0;
}

void *_linit(void)
{
    _lqueue *ret = malloc(sizeof(_lqueue));
    if (ret)
    {
        ret->head = NULL;
        ret->tail = NULL;
        ret->size = 0;
    }
    return ret;
}

int _lpush(void *queue, QUEUE_TYPE value)
{
    int rc = QUEUE_OK;
    if (queue)
    {
        if (((_lqueue *)queue)->size < QUEUE_MAX_SIZE)
        {
            _leaf *append = malloc(sizeof(_leaf));
            if (append)
            {
                append->value = value;
                append->next = NULL;
                if (((_lqueue *)queue)->tail)
                    ((_lqueue *)queue)->tail->next = append;
                ((_lqueue *)queue)->tail = append;
                if (((_lqueue *)queue)->size == 0)
                    ((_lqueue *)queue)->head = ((_lqueue *)queue)->tail;
                (((_lqueue *)queue)->size)++;
            }
            else
                rc = QUEUE_ERR_MEM_ALLOC;
        }
        else
            rc = QUEUE_ERR_OVERFLOW;
    }
    else
        rc = QUEUE_ERR_NULLPTR;
    return rc;
}

int _lpop(void *queue, QUEUE_TYPE *value)
{
    int rc = QUEUE_OK;
    if (queue && value)
    {
        if (((_lqueue *)queue)->size)
        {
            *value = ((_lqueue *)queue)->head->value;
            _leaf *destr = ((_lqueue *)queue)->head;
            ((_lqueue *)queue)->head = ((_lqueue *)queue)->head->next;
            free(destr);
            (((_lqueue *)queue)->size)--;
            if (((_lqueue *)queue)->size == 0)
                ((_lqueue *)queue)->tail = NULL;
        }
        else
            rc = QUEUE_ERR_EMPTY;
    }
    else
        rc = QUEUE_ERR_NULLPTR;
    return rc;
}

void _ldestroy(my_queue *queue)
{
    if (queue)
    {
        if (queue->queue)
        {
            _leaf *tmp;
            while (((_lqueue *)queue->queue)->head)
            {
                tmp = ((_lqueue *)queue->queue)->head;
                ((_lqueue *)queue->queue)->head = ((_lqueue *)queue->queue)->head->next;
                free(tmp);
            }
            free(queue->queue);
        }
        free(queue);
    }
}

size_t _lsize(void *queue)
{
    return (queue) ? ((_lqueue *)queue)->size : 0;
}

size_t _lmemsize(void *queue)
{
    return (queue) ? ((_lqueue *)queue)->size * sizeof(_leaf) + sizeof(_lqueue) : 0;
}

bool _lis_empty(void *queue)
{
    return (queue) ? (((_lqueue *)queue)->size == 0) : true;
}

void _lclear(void *queue)
{
    if (queue)
    {
        ((_lqueue *)queue)->size = 0;
        _leaf *tmp;
        while (((_lqueue *)queue)->head)
        {
            tmp = ((_lqueue *)queue)->head;
            ((_lqueue *)queue)->head = ((_lqueue *)queue)->head->next;
            free(tmp);
        }
    }
}