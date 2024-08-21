#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "my_hasho.h"

#define START_TABLE_SIZE 7
#define MAX_CMP 4
#define MULT_K 1.6

typedef struct leaf leaf;

struct hasho
{
    size_t size, bufsize;
    leaf **table;
    int lastcmp;
};

struct leaf
{
    leaf *next;
    char *key;
    char *value;
};

static int hash(const char *, int);
static int restruct(hasho **);

hasho *init_hasho(void)
{
    hasho *ret = calloc(1, sizeof(hasho));
    if (ret)
    {
        ret->table = calloc(START_TABLE_SIZE, sizeof(leaf *));
        if (ret->table)
            ret->bufsize = START_TABLE_SIZE;
        else
        {
            free(ret->table);
            free(ret);
            ret = NULL;
        }
    }
    return ret;
}

int add_hasho(hasho **table, const char *key, const char *value)
{
    int ret = -1;
    if (*table && key && value)
    {
        ret = 0;
        size_t place = hash(key, (*table)->bufsize);
        leaf *prev = NULL;
        leaf *ptr = (*table)->table[place];
        (*table)->lastcmp = 1;
        for (; ptr && strcmp(ptr->key, key);)
        {
            prev = ptr;
            ptr = ptr->next;
            ++((*table)->lastcmp);
        }
        if (ptr)
        {
            char *tmp = strdup(value);
            if (tmp)
            {
                free(ptr->value);
                ptr->value = tmp;
            }
            else
                ret = -2;
        }
        else
        {
            leaf *new_leaf = calloc(1, sizeof(leaf));
            if (new_leaf)
            {
                new_leaf->value = strdup(value);
                new_leaf->key = strdup(key);
                if (new_leaf->key && new_leaf->value)
                {
                    if (prev)
                        prev->next = new_leaf;
                    else
                        (*table)->table[place] = new_leaf;
                    ++((*table)->size);
                    if ((*table)->lastcmp > MAX_CMP)
                    {
                        //--((*table)->lastcmp);
                        ret = restruct(table);
                    }
                }
                else
                {
                    free(new_leaf->key);
                    free(new_leaf->value);
                    free(new_leaf);
                    ret = -2;
                }
            }
            else
                ret = -2;
        }
    }
    return ret;
}

int del_hasho(hasho *table, const char *key)
{
    int ret = -1;
    if (table && key)
    {
        ret = 0;
        size_t place = hash(key, table->bufsize);
        leaf *ptr = table->table[place], *prev = NULL;
        for (; ptr && strcmp(ptr->key, key); ptr = ptr->next)
            prev = ptr;
        if (ptr)
        {
            free(ptr->key);
            free(ptr->value);
            if (prev)
                prev->next = ptr->next;
            else
                table->table[place] = ptr->next;
            free(ptr);
            --(table->size);
        }
    }
    return ret;
}

void clear_hasho(hasho *table)
{
    if (table)
    {
        leaf *tmp;
        for (size_t i = 0; i < table->bufsize; ++i)
        {
            while (table->table[i])
            {
                tmp = table->table[i];
                table->table[i] = tmp->next;
                free(tmp->key);
                free(tmp->value);
                free(tmp);
            }
            table->size = 0;
        }
    }
}

char *search_hasho(hasho *table, const char *key)
{
    char *ret = NULL;
    if (table && key)
    {
        size_t place = hash(key, table->bufsize);
        leaf *ptr = table->table[place];
        for (table->lastcmp = 1; ptr && strcmp(ptr->key, key); ptr = ptr->next)
            table->lastcmp++;
        if (table->lastcmp > MAX_CMP)
            --(table->lastcmp);
        if (ptr)
            ret = strdup(ptr->value);
    }
    return ret;
}

size_t size_hasho(const hasho *table)
{
    return table ? table->size : 0;
}

size_t mem_size_hasho(const hasho *table)
{
    return (table) ? (sizeof(table) + sizeof(table->table) + table->size * sizeof(leaf)) : 0;
}

bool is_empty_hasho(const hasho *table)
{
    return table ? table->size == 0 : true;
}

void destroy_hasho(hasho *table)
{
    clear_hasho(table);
    free(table->table);
    free(table);
}

int export_hasho(const hasho *table, const char *filename)
{
    int rc = -1;
    FILE *fout = fopen(filename, "w");
    if (fout)
    {
        rc = 0;
        for (size_t i = 0; i < table->bufsize; ++i)
        {
            fprintf(fout, "%ld", i);
            for (leaf *tmp = table->table[i]; tmp; tmp = tmp->next)
                fprintf(fout, ";%s", tmp->key);
            fprintf(fout, "\n");
        }
        fclose(fout);
    }
    return rc;
}

int lastcmp_hasho(const hasho *table)
{
    return table ? table->lastcmp : 0;
}

static int hash(const char *key, int size)
{
    int sum = 0;
    for (; *key; ++key)
        sum += *key;
    return sum % size;
}

static int restruct(hasho **table)
{
    int rc = -1;
    hasho *new_table = calloc(1, sizeof(hasho));
    if (new_table)
    {
        rc = 0;
        int bufsize = (*table)->bufsize, code, cnt;
        while (rc == 0 && new_table->table == NULL)
        {
            bufsize = (int)(bufsize * MULT_K);
            new_table->table = calloc(bufsize, sizeof(leaf *));
            leaf *ptr, *ptr2, *new_leaf, *prev;
            if (new_table->table)
            {
                new_table->bufsize = bufsize;
                new_table->size = (*table)->size;
                new_table->lastcmp = (*table)->lastcmp;
                for (size_t i = 0; i < (*table)->bufsize && new_table->table; ++i)
                {
                    for (ptr = (*table)->table[i]; ptr; ptr = ptr->next)
                    {
                        code = hash(ptr->key, bufsize);
                        prev = NULL;
                        for (ptr2 = new_table->table[code], cnt = 0; ptr2 && ptr2->next; ptr2 = ptr2->next, ++cnt)
                            ;
                        prev = ptr2;
                        if (cnt + 1 >= MAX_CMP)
                        {
                            clear_hasho(new_table);
                            free(new_table->table);
                            new_table->table = NULL;
                        }
                        else
                        {
                            new_leaf = calloc(1, sizeof(leaf));
                            if (new_leaf)
                            {
                                new_leaf->key = strdup(ptr->key);
                                new_leaf->value = strdup(ptr->value);
                                if (new_leaf->key && new_leaf->value)
                                {
                                    if (prev)
                                        prev->next = new_leaf;
                                    else
                                        new_table->table[code] = new_leaf;
                                }
                                else
                                {
                                    free(new_leaf->key);
                                    free(new_leaf->value);
                                    free(new_leaf);
                                    rc = -2;
                                }
                            }
                            else
                                rc = -2;
                        }
                    }
                }
            }
            else
                rc = -2;
        }
        if (rc)
            destroy_hasho(new_table);
        else
        {
            destroy_hasho(*table);
            *table = new_table;
        }
    }
    return rc;
}