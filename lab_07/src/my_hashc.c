#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "my_hashc.h"

#define START_TABLE_SIZE 7
#define MAX_CMP 4
#define MULT_K 1.6

typedef struct record record;

struct hashc
{
    size_t size, bufsize;
    record *table;
    int lastcmp;
};

struct record
{
    char *key;
    char *value;
};

static int hash(const char *, int);
static int restruct(hashc **, const char *, const char *);

hashc *init_hashc(void)
{
    hashc *ret = calloc(1, sizeof(hashc));
    if (ret)
    {
        ret->table = calloc(START_TABLE_SIZE, sizeof(record));
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

int add_hashc(hashc **table, const char *key, const char *value)
{
    int ret = -1;
    if (*table && key && value)
    {
        ret = 0;
        size_t hash_code = hash(key, (*table)->bufsize);
        size_t place = hash_code;
        (*table)->lastcmp = 1;
        for (; (!((*table)->table[place].key) || strcmp((*table)->table[place].key, key)) && (*table)->lastcmp < MAX_CMP;
             place = (place + 1) % (*table)->bufsize) 
            ++((*table)->lastcmp);
        if (((*table)->lastcmp) < MAX_CMP)
        {
            char *tmp = strdup(value);
            if (tmp)
            {
                free((*table)->table[place].value);
                (*table)->table[place].value = tmp;
            }
            else
                ret = -2;
        }
        else
        {
            //(*table)->lastcmp = 0;
            place = hash_code;
            for (; (*table)->table[place].key && ((*table)->lastcmp) < MAX_CMP * 2 + 1;
                 place = (place + 1) % (*table)->bufsize)
                ++((*table)->lastcmp);
            if (((*table)->lastcmp) < MAX_CMP * 2 + 1)
            {
                (*table)->table[place].key = strdup(key);
                if ((*table)->table[place].key)
                {
                    (*table)->table[place].value = strdup(value);
                    if ((*table)->table[place].value == NULL)
                    {
                        free((*table)->table[place].key);
                        (*table)->table[place].key = NULL;
                        ret = -2;
                    }
                }
                else
                    ret = -2;
                if (ret == 0)
                    ++((*table)->size);
            }
            else
            {
                //--((*table)->lastcmp);
                ret = restruct(table, key, value);
                if (ret == 0)
                    ++((*table)->size);
            }
        }
    }
    return ret;
}

int del_hashc(hashc *table, const char *key)
{
    int ret = -1;
    if (table && key)
    {
        ret = 0;
        size_t place = hash(key, table->bufsize);
        for (table->lastcmp = 0; (!(table->table[place].key) || strcmp(table->table[place].key, key)) && (table->lastcmp) < MAX_CMP;
             place = (place + 1) % table->bufsize, ++(table->lastcmp))
            ;
        if (table->table[place].key && table->lastcmp < MAX_CMP)
        {
            free(table->table[place].key);
            table->table[place].key = NULL;
            free(table->table[place].value);
            table->table[place].value = NULL;
            if (ret == 0)
                --(table->size);
        }
    }
    return ret;
}

void clear_hashc(hashc *table)
{
    if (table)
    {
        for (size_t i = 0; i < table->bufsize; ++i)
        {
            free(table->table[i].key);
            table->table[i].key = NULL;
            free(table->table[i].value);
            table->table[i].value = NULL;
        }
    }
}

char *search_hashc(hashc *table, const char *key)
{
    char *ret = NULL;
    if (table && key)
    {
        size_t place = hash(key, table->bufsize);
        for (table->lastcmp = 1; (!(table->table[place].key) || strcmp(table->table[place].key, key)) && (table->lastcmp) < MAX_CMP;
             place = (place + 1) % table->bufsize)
            ++(table->lastcmp);
        if (table->table[place].key && table->lastcmp <= MAX_CMP)
            ret = strdup(table->table[place].value);
        if (table->lastcmp > MAX_CMP)
            --(table->lastcmp);
    }
    return ret;
}

size_t size_hashc(const hashc *table)
{
    return table ? table->size : 0;
}

size_t mem_size_hashc(const hashc *table)
{
    return (table) ? (sizeof(hashc) + sizeof(record) * table->bufsize) : 0;
}

bool is_empty_hashc(const hashc *table)
{
    return table ? table->size == 0 : true;
}

void destroy_hashc(hashc *table)
{
    clear_hashc(table);
    free(table->table);
    free(table);
}

int export_hashc(const hashc *table, const char *filename)
{
    int rc = -1;
    FILE *fout = fopen(filename, "w");
    if (fout)
    {
        rc = 0;
        for (size_t i = 0; i < table->bufsize; ++i)
        {
            fprintf(fout, "%ld", i);
            if (table->table[i].key)
                fprintf(fout, ";%d;%s", hash(table->table[i].key, table->bufsize), table->table[i].key);
            fprintf(fout, "\n");
        }
        fclose(fout);
    }
    return rc;
}

int lastcmp_hashc(const hashc *table)
{
    return (table) ? table->lastcmp : 0;
}

static int hash(const char *key, int size)
{
    int sum = 0;
    for (; *key; ++key)
        sum += *key;
    return sum % size;
}

static int restruct(hashc **table, const char *key, const char *value)
{
    int rc = -1;
    hashc *new_table = calloc(1, sizeof(hashc));
    char *nkey = strdup(key), *nvalue = strdup(value);
    if (new_table && nkey && nvalue)
    {
        rc = 0;
        int bufsize = (*table)->bufsize, code, cnt;
        while (rc == 0 && new_table->table == NULL)
        {
            bufsize = (int)(bufsize * MULT_K);
            new_table->table = calloc(bufsize, sizeof(record));
            new_table->lastcmp = (*table)->lastcmp;
            if (new_table->table)
            {
                new_table->bufsize = bufsize;
                new_table->size = (*table)->size;
                for (size_t i = 0; i < (*table)->bufsize && new_table->table; ++i)
                {
                    if ((*table)->table[i].key)
                    {
                        code = hash((*table)->table[i].key, bufsize);
                        for (cnt = 0; new_table->table[code].key && strcmp(new_table->table[code].key, (*table)->table[i].key) && cnt < MAX_CMP; code = (code + 1) % bufsize, ++cnt)
                            ;
                        if (cnt >= MAX_CMP)
                        {
                            free(new_table->table);
                            new_table->table = NULL;
                        }
                        else
                        {
                            new_table->table[code].key = (*table)->table[i].key;
                            new_table->table[code].value = (*table)->table[i].value;
                        }
                    }
                }
                if (new_table->table)
                {
                    code = hash(nkey, bufsize);
                    for (cnt = 0; new_table->table[code].key && strcmp(new_table->table[code].key, key) && cnt < MAX_CMP; code = (code + 1) % bufsize, ++cnt)
                        ;
                    if (cnt >= MAX_CMP)
                    {
                        free(new_table->table);
                        new_table->table = NULL;
                    }
                    else
                    {
                        new_table->table[code].key = nkey;
                        new_table->table[code].value = nvalue;
                    }
                }
            }
            else
                rc = -2;
        }
        if (rc)
        {
            free(new_table->table);
            free(new_table);
        }
        else
        {
            free((*table)->table);
            free(*table);
            *table = new_table;
        }
    }
    else
    {
        free(nkey);
        free(nvalue);
        free(new_table);
    }
    return rc;
}