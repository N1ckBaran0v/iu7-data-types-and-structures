#define _GNU_SOURCE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#include "my_avl_tree.h"

typedef struct node_tree node_tree;

struct avl_tree
{
    node_tree *start;
    size_t size;
#ifndef DNDEBUG
    int lastcmp;
#endif
};

struct node_tree
{
    char *key;
    char *value;
    node_tree *left, *right;
    int hleft, hright;
};

typedef void *(*my_action)(avl_tree *, node_tree *, node_tree *, const char *, const void *);

static void *goto_leaf(avl_tree *, node_tree *, node_tree *, const char *, const void *, my_action);
static void *apply_tree(avl_tree *, node_tree *, node_tree *, const void *, int, my_action);
static void *add_leaf(avl_tree *, node_tree *, node_tree *, const char *, const void *);
static void *remove_leaf(avl_tree *, node_tree *, node_tree *, const char *, const void *);
static void *find_leaf(avl_tree *, node_tree *, node_tree *, const char *, const void *);
static void *print_leaf(avl_tree *, node_tree *, node_tree *, const char *, const void *);
static void *update_leaf(avl_tree *, node_tree *, node_tree *, const char *, const void *);
static void *balance_leaf(avl_tree *, node_tree *, node_tree *, const char *, const void *);

avl_tree *init_avl_tree(void)
{
    avl_tree *ret = calloc(1, sizeof(avl_tree));
    return ret;
}

int add_avl_tree(avl_tree *wood, const char *key, const char *value)
{
    int ret = -1;
    if (wood && key && value)
    {
        ret = 0;
        void *added = goto_leaf(wood, NULL, wood->start, key, value, add_leaf);
        if (!(added))
            ret = -1;
    }
    return ret;
}

int del_avl_tree(avl_tree *wood, const char *key)
{
    int ret = -1;
    if (wood && key)
    {
        ret = 0;
        (void)goto_leaf(wood, NULL, wood->start, key, NULL, remove_leaf);
    }
    return ret;
}

int clear_avl_tree(avl_tree *wood)
{
    int ret = -1;
    if (wood)
    {
        ret = 0;
        (void)apply_tree(wood, NULL, wood->start, NULL, 2, remove_leaf);
    }
    return ret;
}

char *search_avl_tree(const avl_tree *wood, const char *key)
{
    void *found = NULL;
    if (wood && key)
        found = goto_leaf((avl_tree *)wood, NULL, wood->start, key, NULL, find_leaf);
    return found;
}

size_t size_avl_tree(const avl_tree *wood)
{
    return wood ? wood->size : 0;
}

size_t mem_size_avl_tree(const avl_tree *wood)
{
    if (wood)
        return sizeof(avl_tree) + wood->size * sizeof(node_tree);
    return 0;
}

bool is_empty_avl_tree(const avl_tree *wood)
{
    return wood ? wood->size == 0 : true;
}

void destroy_avl_tree(avl_tree *wood)
{
    (void)clear_avl_tree(wood);
    free(wood);
}

int export_avl_tree(avl_tree *wood, const char *filename)
{
    int rc = -1;
    FILE *fout = fopen(filename, "w");
    if (fout)
    {
        rc = 0;
        fprintf(fout, "digraph tree {\n");
        apply_tree(wood, NULL, wood->start, fout, 0, print_leaf);
        fprintf(fout, "}\n");
        fclose(fout);
    }
    return rc;
}

static void *goto_leaf(avl_tree *wood, node_tree *prev, node_tree *curr, const char *key, const void *val, my_action foo)
{
    void *ret = NULL;
#ifndef DNDEBUG
    if (prev == NULL)
        wood->lastcmp = 0;
    ++(wood->lastcmp);
#endif
    if (curr)
    {
        int diff = strcmp(curr->key, key);
        if (diff > 0)
            ret = goto_leaf(wood, curr, curr->left, key, val, foo);
        else if (diff < 0)
            ret = goto_leaf(wood, curr, curr->right, key, val, foo);
        else
        {
            int flag = (foo == remove_leaf && curr->hleft + curr->hright == 0);
            ret = foo(wood, prev, curr, key, val);
            if (flag)
                curr = NULL;
        }
        curr = update_leaf(wood, prev, curr, key, val);
        curr = balance_leaf(wood, prev, curr, key, val);
    }
    else if (val)
        ret = foo(wood, prev, curr, key, val);
    return ret;
}

static void *apply_tree(avl_tree *wood, node_tree *prev, node_tree *curr, const void *params, int mode, my_action act)
{
    void *ret = NULL;
    if (curr)
    {
        if (mode == 0)
        {
            ret = act(wood, prev, curr, NULL, params);
            ret = apply_tree(wood, curr, curr->left, params, mode, act);
            ret = apply_tree(wood, curr, curr->right, params, mode, act);
        }
        else if (mode == 1)
        {
            ret = apply_tree(wood, curr, curr->left, params, mode, act);
            ret = act(wood, prev, curr, NULL, params);
            ret = apply_tree(wood, curr, curr->right, params, mode, act);
        }
        else
        {
            ret = apply_tree(wood, curr, curr->left, params, mode, act);
            ret = apply_tree(wood, curr, curr->right, params, mode, act);
            ret = act(wood, prev, curr, NULL, params);
        }
    }
    return ret;
}

static void *add_leaf(avl_tree *wood, node_tree *prev, node_tree *curr, const char *key, const void *value)
{
    void *ret = NULL;
    if (curr)
    {
        char *tmp_value = calloc(strlen(value) + 1, sizeof(char));
        if (tmp_value)
        {
            free(curr->value);
            memmove(tmp_value, value, strlen(value));
            curr->value = tmp_value;
            ret = curr;
        }
    }
    else
    {
        node_tree *node = calloc(1, sizeof(node_tree));
        if (node)
        {
            node->key = calloc(strlen(key) + 1, sizeof(char));
            node->value = calloc(strlen(value) + 1, sizeof(char));
            if (node->key && node->value)
            {
                memmove(node->key, key, strlen(key));
                memmove(node->value, value, strlen(value));
                ret = node;
                (wood->size)++;
                if (prev)
                {
                    if (strcmp(prev->key, key) > 0)
                        prev->left = ret;
                    else
                        prev->right = ret;
                }
                else
                    wood->start = ret;
            }
            else
            {
                free(node->key);
                free(node->value);
                free(node);
            }
        }
    }
    return ret;
}

static void *remove_leaf(avl_tree *wood, node_tree *prev, node_tree *curr, const char *key, const void *unused)
{
    free(curr->key);
    free(curr->value);
    node_tree *destr = NULL;
    if (curr->left == NULL && curr->right == NULL)
    {
        destr = curr;
        if (prev)
        {
            if (prev->left == curr)
                prev->left = NULL;
            else
                prev->right = NULL;
        }
        else
            wood->start = NULL;
    }
    else if (curr->left == NULL || curr->right == NULL)
    {
        if (curr->left == NULL)
            destr = curr->right;
        else
            destr = curr->left;
        curr->right = destr->right;
        curr->left = destr->left;
        curr->key = destr->key;
        curr->value = destr->value;
    }
    else
    {
        node_tree *predstr = curr;
        destr = curr->right;
        while (destr->left)
        {
            predstr = destr;
            destr = destr->left;
        }
        curr->key = destr->key;
        curr->value = destr->value;
        if (curr->right != destr)
        {
            predstr->left = destr->right;
            free(goto_leaf(wood, prev, curr, predstr->key, NULL, find_leaf));
        }
        else
            predstr->right = destr->right;
    }
    free(destr);
    (wood->size)--;
    return NULL;
}

static void *find_leaf(avl_tree *wood, node_tree *prev, node_tree *curr, const char *key, const void *unused)
{
    return strdup(curr->value);
}

static void *print_leaf(avl_tree *wood, node_tree *prev, node_tree *curr, const char *key, const void *file)
{
    if (curr->left)
        fprintf((FILE *)file, "    \"%s\" -> \"%s\";\n", curr->key, curr->left->key);
    if (curr->right)
        fprintf((FILE *)file, "    \"%s\" -> \"%s\";\n", curr->key, curr->right->key);
    return NULL;
}

static void *update_leaf(avl_tree *wood, node_tree *prev, node_tree *curr, const char *key, const void *unused)
{
    if (curr)
    {
        if (curr->left)
            curr->hleft = MAX(curr->left->hleft, curr->left->hright) + 1;
        else
            curr->hleft = 0;
        if (curr->right)
            curr->hright = MAX(curr->right->hleft, curr->right->hright) + 1;
        else
            curr->hright = 0;
    }
    return curr;
}

static void *balance_leaf(avl_tree *wood, node_tree *prev, node_tree *curr, const char *key, const void *unused)
{
    if (curr)
    {
        if (curr->hleft - curr->hright > 1)
        {
            node_tree *tmp1 = curr, *tmp2 = curr->left->right, *tmp3 = curr->left->left;
            if (tmp2 && tmp3)
            {
                curr = curr->left;
                tmp1->left = tmp2;
                curr->right = tmp1;
                tmp1->left = update_leaf(wood, prev, tmp1->left, key, unused);
                curr->right = update_leaf(wood, prev, curr->right, key, unused);
                curr = update_leaf(wood, prev, curr, key, unused);
            }
            else
            {
                if (tmp3)
                {
                    curr = curr->left;
                    curr->right = tmp1;
                    curr->right->left = NULL;
                    curr->right = update_leaf(wood, prev, curr->right, key, unused);
                    curr = update_leaf(wood, prev, curr, key, unused);
                }
                else
                {
                    curr = tmp2;
                    curr->left = tmp1->left;
                    curr->right = tmp1;
                    curr->left->right = NULL;
                    curr->right->left = NULL;
                    curr->right = update_leaf(wood, prev, curr->right, key, unused);
                    curr->left = update_leaf(wood, prev, curr->left, key, unused);
                    curr = update_leaf(wood, prev, curr, key, unused);
                }
            }
            if (prev)
            {
                if (prev->left == tmp1)
                    prev->left = curr;
                else
                    prev->right = curr;
            }
            else
                wood->start = curr;
        }
        else if (curr->hleft - curr->hright < -1)
        {
            node_tree *tmp1 = curr, *tmp2 = curr->right->left, *tmp3 = curr->right->right;
            if (tmp2 && tmp3)
            {
                curr = curr->right;
                tmp1->right = tmp2;
                curr->left = tmp1;
                tmp1->right = update_leaf(wood, prev, tmp1->right, key, unused);
                curr->left = update_leaf(wood, prev, curr->left, key, unused);
                curr = update_leaf(wood, prev, curr, key, unused);
            }
            else
            {
                if (tmp3)
                {
                    curr = curr->right;
                    curr->left = tmp1;
                    curr->left->right = NULL;
                    curr->left = update_leaf(wood, prev, curr->left, key, unused);
                    curr = update_leaf(wood, prev, curr, key, unused);
                }
                else
                {
                    curr = tmp2;
                    curr->right = tmp1->right;
                    curr->left = tmp1;
                    curr->left->right = NULL;
                    curr->right->left = NULL;
                    curr->right = update_leaf(wood, prev, curr->right, key, unused);
                    curr->left = update_leaf(wood, prev, curr->left, key, unused);
                    curr = update_leaf(wood, prev, curr, key, unused);
                }
            }
            if (prev)
            {
                if (prev->left == tmp1)
                    prev->left = curr;
                else
                    prev->right = curr;
            }
            else
                wood->start = curr;
        }
    }
    return curr;
}


#ifndef DNDEBUG
int lastcmp_avl_tree(const avl_tree *wood)
{
    return wood->lastcmp;
}
#endif