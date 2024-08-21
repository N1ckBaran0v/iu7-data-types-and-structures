#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "my_graph.h"

struct my_graph
{
    size_t roads_cnt;
    uint32_t **buf;
};

my_graph *my_graph_init(void)
{
    my_graph *ret = calloc(1, sizeof(my_graph));
    if (ret)
    {
        ret->buf = calloc(1, MAX_NUM * sizeof(uint32_t *) + MAX_NUM * MAX_NUM * sizeof(uint32_t));
        if (ret->buf)
            for (size_t it = 0; it < MAX_NUM; ++it)
            {
                if (it)
                    ret->buf[it] = ret->buf[it - 1] + MAX_NUM;
                else
                    ret->buf[it] = (uint32_t *)(ret->buf + MAX_NUM);
            }
        else
        {
            free(ret);
            ret = NULL;
        }
    }
    return ret;
}

int my_graph_push(my_graph *graph, uint32_t first, uint32_t second, uint32_t lenght)
{
    int rc = first < 1 || first > MAX_NUM || second < 1 || second > MAX_NUM || lenght < 1 || lenght > MAX_LENGHT || first == second;
    if (rc == ERR_OK)
    {
        if (graph && graph->buf)
        {
            graph->roads_cnt += graph->buf[first - 1][second - 1] == 0;
            graph->buf[first - 1][second - 1] = lenght;
        }
        else
            rc = ERR_INCORRECT;
    }
    else
        rc = ERR_INCORRECT;
    return rc;
}

uint32_t my_graph_pop(my_graph *graph, uint32_t first, uint32_t second)
{
    uint32_t ret = 0;
    if (graph)
    {
        if (!(first < 1 || first > MAX_NUM || second < 1 || second > MAX_NUM))
        {
            ret = graph->buf[first - 1][second - 1];
            if (ret)
                --(graph->roads_cnt);
            graph->buf[first - 1][second - 1] = 0;
        }
    }
    return ret;
}

size_t my_graph_find(my_graph *graph, uint32_t start, uint32_t maxlen, uint32_pair **result)
{
    const int inf = MAX_LENGHT * MAX_NUM + 1;
    size_t size = 0;
    if (graph && graph->buf && graph->roads_cnt && start && start < MAX_NUM + 1 && *result == NULL)
    {
        uint32_t *len = malloc(MAX_NUM * sizeof(uint32_t));
        // uint32_t *prev  = malloc(MAX_NUM * sizeof(uint32_t));
        uint8_t *set = calloc(MAX_NUM, sizeof(uint8_t));
        if (len && /*prev && */ set)
        {
            size_t qsize = 0;
            for (size_t i = 0; i < MAX_NUM; ++i)
            {
                len[i] = inf;
                // prev[i] = MAX_NUM;
            }
            len[start - 1] = 0;
            set[start - 1] = 1;
            ++qsize;
            while (qsize)
            {
                uint32_t u = MAX_NUM, minlen = inf;
                //printf("u: %ld\n", (long)u);
                for (size_t i = 0; i < MAX_NUM; ++i)
                {
                    if (set[i] == 1 && len[i] <= minlen)
                    {
                        minlen = len[i];
                        u = i;
                    }
                }
                //printf("u: %ld\n", (long)u);
                set[u] = 2;
                qsize--;
                //printf("u: %ld\n", (long)u);
                for (size_t i = 0; i < MAX_NUM; ++i)
                {
                    //printf("u, i: %ld, %ld\n", (long)u, (long)i);
                    if (set[i] < 2 && graph->buf[u][i])
                    {
                        if (len[i] > len[u] + graph->buf[u][i])
                            len[i] = len[u] + graph->buf[u][i];
                        // prev[i] = u;
                        if (set[i] == 0)
                        {
                            set[i] = 1;
                            qsize++;
                        }
                    }
                }
            }
            for (size_t i = 0; i < MAX_NUM; ++i)
                if (i != start - 1 && len[i] <= maxlen)
                    size++;
            if (size)
            {
                *result = calloc(size, sizeof(uint32_pair));
                if (*result)
                {
                    uint32_t it = 0;
                    for (uint32_t i = 0; i < MAX_NUM; ++i)
                    {
                        if (i != start - 1 && len[i] <= maxlen)
                        {
                            (*result)[it].number = i + 1;
                            (*result)[it++].value = len[i];
                        }
                    }
                }
            }
        }
        free(len);
        // free(prev);
        free(set);
    }
    return size;
}

int my_graph_save(my_graph *graph, const char *filename)
{
    int ret = graph && graph->buf && filename;
    if (ret != ERR_OK)
    {
        if (graph->roads_cnt == 0)
            ret = ERR_NOT_FOUND;
        else
            ret = ERR_OK;
    }
    else
        ret = ERR_INCORRECT;
    if (ret == ERR_OK)
    {
        FILE *fout = fopen(filename, "w");
        if (fout)
        {
            fprintf(fout, "digraph my_graph {\n");
            for (size_t i = 0; i < MAX_NUM; ++i)
                for (size_t j = 0; j < MAX_NUM; ++j)
                    if (graph->buf[i][j])
                        fprintf(fout, "\t%ld -> %ld [label=\"%ld\"];\n", (long)i + 1, (long)j + 1, (long)(graph->buf[i][j]));
            fprintf(fout, "}\n");
            fclose(fout);
        }
        else
            ret = ERR_FILE;
    }
    return ret;
}

void my_graph_destroy(my_graph *graph)
{
    if (graph)
        free(graph->buf);
    free(graph);
}