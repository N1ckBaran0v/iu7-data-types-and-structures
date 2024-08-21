#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "my_tree.h"

struct tree
{
    node_tree *start;
    size_t size;
};

struct node_tree
{
    char *name;
    date day;
    node_tree *left, *right;
};

tree *init_tree(void)
{
    tree *ret = malloc(sizeof(tree));
    if (ret)
    {
        ret->size = 0;
        ret->start = NULL;
    }
    return ret;
}

int add_to_tree(tree *wood, const char *name, const date *mod)
{
    int rc = ERR_OK;
    node_tree *node = malloc(sizeof(node_tree));
    if (wood->size >= MAX_TREE_SIZE)
    {
        rc = ERR_TREE_OVERFLOW;
        free(node);
    }
    else if (node)
    {
        node->day = *mod;
        node->left = NULL;
        node->right = NULL;
        node->name = strdup(name);
        if (node->name)
        {
            if (wood->size == 0)
            {
                wood->start = node;
                (wood->size)++;
            }
            else
            {
                node_tree *prev = NULL, *curr = wood->start;
                bool right = true;
                bool cont = true;
                int cmp = 0;
                while (cont)
                {
                    if (curr)
                    {
                        cmp = strcmp(node->name, curr->name);
                        if (cmp < 0)
                        {
                            prev = curr;
                            curr = prev->left;
                            right = false;
                        }
                        else if (cmp > 0)
                        {
                            prev = curr;
                            curr = prev->right;
                            right = true;
                        }
                        else
                        {
                            curr->day = node->day;
                            free(node->name);
                            free(node);
                            cont = false;
                        }
                    }
                    else
                    {
                        if (right)
                            prev->right = node;
                        else
                            prev->left = node;
                        (wood->size)++;
                        cont = false;
                    }
                }
            }
        }
        else
        {
            rc = ERR_MEM_ALLOC;
            free(node);
            node = NULL;
        }
    }
    else
        rc = ERR_MEM_ALLOC;
    return rc;
}

int del_by_name_tree(tree *wood, const char *name)
{
    node_tree *prev = NULL, *curr = wood->start;
    int cmp = 0;
    int rc = ERR_NOT_FOUND;
    if (wood->size == 0)
        rc = ERR_EMPTY_TREE;
    while (curr)
    {
        cmp = strcmp(name, curr->name);
        if (cmp < 0)
        {
            prev = curr;
            curr = curr->left;
        }
        else if (cmp > 0)
        {
            prev = curr;
            curr = curr->right;
        }
        else
        {
            rc = ERR_OK;
            free(curr->name);
            if (curr->left == NULL && curr->right == NULL)
            {
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
            else if (curr->left == NULL)
            {
                if (prev)
                {
                    if (prev->left == curr)
                        prev->left = curr->right;
                    else
                        prev->right = curr->right;
                }
                else
                    wood->start = curr->right;
            }
            else if (curr->right == NULL)
            {
                if (prev)
                {
                    if (prev->left == curr)
                        prev->left = curr->left;
                    else
                        prev->right = curr->left;
                }
                else
                    wood->start = curr->left;
            }
            else
            {
                node_tree *del = curr;
                curr = curr->right;
                prev = del;
                while (curr->left)
                {
                    prev = curr;
                    curr = curr->left;
                }
                del->name = curr->name;
                if (prev != del)
                    prev->left = curr->right;
                else
                    prev->right = curr->right;
            }
            free(curr);
            (wood->size)--;
            curr = NULL;
        }
    }
    return rc;
}

int del_earlier_than_tree(tree *wood, const date *deadline)
{
    int deleted = 0;
    if (wood)
    {
        node_tree *ptr_st[MAX_TREE_SIZE], *curr = NULL, *prev = NULL;
        int phase[MAX_TREE_SIZE];
        size_t size = 0;
        if (wood->start)
        {
            ptr_st[0] = wood->start;
            phase[0] = 0;
            ++size;
        }
        while (size)
        {
            if (phase[size - 1] == 0)
            {
                if (ptr_st[size - 1]->left)
                {
                    ptr_st[size] = ptr_st[size - 1]->left;
                    phase[size] = 0;
                    ++size;
                }
                else
                    phase[size - 1]++;
            }
            else if (phase[size - 1] == 1)
            {
                if (ptr_st[size - 1]->right)
                {
                    ptr_st[size] = ptr_st[size - 1]->right;
                    phase[size] = 0;
                    ++size;
                }
                else
                    phase[size - 1]++;
            }
            else
            {
                curr = ptr_st[size - 1];
                prev = NULL;
                if (size > 1)
                    prev = ptr_st[size - 2];
                if (curr->day.year < deadline->year || (curr->day.year == deadline->year && curr->day.month < deadline->month) || (curr->day.year == deadline->year && curr->day.month == deadline->month && curr->day.day <= deadline->day))
                {
                    free(curr->name);
                    if (curr->left == NULL && curr->right == NULL)
                    {
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
                    else if (curr->left == NULL)
                    {
                        if (prev)
                        {
                            if (prev->left == curr)
                                prev->left = curr->right;
                            else
                                prev->right = curr->right;
                        }
                        else
                            wood->start = curr->right;
                    }
                    else if (curr->right == NULL)
                    {
                        if (prev)
                        {
                            if (prev->left == curr)
                                prev->left = curr->left;
                            else
                                prev->right = curr->left;
                        }
                        else
                            wood->start = curr->left;
                    }
                    else
                    {
                        node_tree *del = curr;
                        curr = curr->right;
                        prev = del;
                        while (curr->left)
                        {
                            prev = curr;
                            curr = curr->left;
                        }
                        del->name = curr->name;
                        if (prev != del)
                            prev->left = curr->right;
                        else
                            prev->right = curr->right;
                    }
                    free(curr);
                    (wood->size)--;
                    deleted++;
                }
                curr = NULL;
                size--;
                if (size)
                    phase[size - 1]++;
            }
        }
    }
    return deleted;
}

date search_tree(const tree *wood, const char *name)
{
    node_tree *curr = wood->start;
    int cmp = 0;
    date ret = {-1, -1, -1};
    while (curr)
    {
        cmp = strcmp(name, curr->name);
        if (cmp < 0)
            curr = curr->left;
        else if (cmp > 0)
            curr = curr->right;
        else
        {
            ret = curr->day;
            curr = NULL;
        }
    }
    return ret;
}

void apply_tree(const tree *wood, ptr_func_tree func, void *params, const int mode)
{
    if (wood)
    {
        node_tree *ptr_st[MAX_TREE_SIZE];
        int phase[MAX_TREE_SIZE];
        size_t size = 0;
        if (wood->start)
        {
            ptr_st[0] = wood->start;
            phase[0] = 0;
            ++size;
        }
        while (size)
        {
            if ((phase[size - 1] == 0 && mode != DO_BEFORE) || (phase[size - 1] == 1 && mode == DO_BEFORE))
            {
                if (ptr_st[size - 1]->left)
                {
                    ptr_st[size] = ptr_st[size - 1]->left;
                    phase[size] = 0;
                    ++size;
                }
                else
                    phase[size - 1]++;
            }
            else if ((phase[size - 1] == 1 && mode == DO_AFTER) || (phase[size - 1] == 2 && mode != DO_AFTER))
            {
                if (ptr_st[size - 1]->right)
                {
                    ptr_st[size] = ptr_st[size - 1]->right;
                    phase[size] = 0;
                    ++size;
                }
                else
                    phase[size - 1]++;
            }
            else if (phase[size - 1] == mode)
            {
                func(ptr_st[size - 1], params);
                phase[size - 1]++;
            }
            if (phase[size - 1] > 2)
            {
                size--;
                if (size)
                    phase[size - 1]++;
            }
        }
    }
}

void print_leaf_tree(node_tree *node, void *ignored)
{
    printf("%02d.%02d.%04d %s\n", node->day.day, node->day.month, node->day.year, node->name);
}

void get_random_value_tree(node_tree *node, void *ignored)
{
    char **name = ignored;
    if (!(node->left) && !(node->right))
    {
        if (*name == NULL || rand() % 2)
            *name = node->name;
    }
}

size_t size_tree(const tree *wood)
{
    return wood ? wood->size : 0;
}

size_t mem_size_tree(const tree *wood)
{
    if (wood)
        return sizeof(tree) + wood->size * sizeof(node_tree);
    return 0;
}

bool is_empty_tree(const tree *wood)
{
    return (wood) ? (wood->size == 0) : false;
}

void destroy_node(node_tree *node, void *unused)
{
    if (node)
        free(node->name);
    free(node);
}

void destroy_tree(tree *wood)
{
    if (wood)
        apply_tree(wood, destroy_node, NULL, DO_AFTER);
    free(wood);
}

void dot_node(node_tree *node, void *file)
{
    if (node->left)
        fprintf(file, "    \"%s\\n%02d.%02d.%04d\" -> \"%s\\n%02d.%02d.%04d\";\n",
                node->name, node->day.day, node->day.month, node->day.year,
                node->left->name, node->left->day.day, node->left->day.month, node->left->day.year);
    if (node->right)
        fprintf(file, "    \"%s\\n%02d.%02d.%04d\" -> \"%s\\n%02d.%02d.%04d\";\n",
                node->name, node->day.day, node->day.month, node->day.year,
                node->right->name, node->right->day.day, node->right->day.month, node->right->day.year);
}

int export_tree(const tree *wood, const char *filename)
{
    FILE *fout = fopen(filename, "w");
    int rc = ERR_OK;
    if (fout)
    {
        fprintf(fout, "digraph tree {\n");
        apply_tree(wood, dot_node, fout, DO_BEFORE);
        fprintf(fout, "}\n");
        fclose(fout);
    }
    else
        rc = ERR_FOPEN_ERROR;
    return rc;
}