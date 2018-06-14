#pragma GCC diagnostic ignored "-Walloc-size-larger-than="
#pragma GCC diagnostic ignored "-Wint-conversion"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef struct NODE {
    int index;
    int *from;
    int from_num;
    int *to;
    int to_num;
    int pass;
} node;

typedef struct EDGE {
    int index;
    node *start;
    node *end;
    int del;
} edge;

typedef struct COMMAND {
    char *cmd;
    long first;
    long second;
} command;

int success = 0;

void search_node(edge *edges, node *nodes, command *cmd, int count2, edge current, int *array, int depth);

int main() {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *ptr;

    int count = 0, count2 = 0, count3 = 0;
    int i, j;
    char *temp1 = NULL;
    char *temp2[3] = {NULL};
    char **text;
    text = malloc(sizeof(char *) * 3);
    const char space[2] = " ";

    ////////////////////////////////////////////////////////////////////////////

    while ((read = getline(&line, &len, stdin) != -1)) {
//        printf("Retrieved line of length %zu :", read);
//        printf("%s", line);
//        printf("sizeof line = %zu\n", len);
        temp1 = strdup(line);

        if (line[0] != '0') {
            temp2[0] = strtok(temp1, space);
            temp2[1] = strtok(NULL, space);
            temp2[2] = strtok(NULL, space);

            text = (char **) realloc(text, sizeof(char *) * (3 * (count + 1)));
            text[0 + count * 3] = temp2[0];
            text[1 + count * 3] = temp2[1];
            text[2 + count * 3] = temp2[2];

            if (text[2 + count * 3] == NULL) {
                count2++;
                text[1 + count * 3] = strtok(text[1 + count * 3], "\n");
//                printf("%s\n", text[1 + count * 3]);
            } else {
                count3++;
                text[2 + count * 3] = strtok(text[2 + count * 3], "\n");
//                printf("%s\n", text[2 + count * 3]);
            }
//            printf("text[%d] = %s text[%d] = %s text[%d] = %s\n", 0 + count * 3, text[0 + count * 3], 1 + count * 3,
//                   text[1 + count * 3], 2 + count * 3, text[2 + count * 3]);
        }

        count++;
    }
    ////////////////////////////////////////////////////////////////////////////

//    printf("count = %d\n", count);
//    printf("count2 = %d\n", count2);
//    printf("count3 = %d\n", count3);

    command *cmd = (command *) malloc(sizeof(command) * count3);

    edge *edges = (edge *) malloc(sizeof(edge) * count2);

    long max = LONG_MIN;

    for (i = 0; i < count2; i++) {
        if ((max <= strtol(text[i * 3], &ptr, 10)) && (max <= strtol(text[i * 3 + 1], &ptr, 10))) {
            if (strtol(text[i * 3], &ptr, 10) >= strtol(text[i * 3 + 1], &ptr, 10)) {
                max = strtol(text[i * 3], &ptr, 10);
            } else {
                max = strtol(text[i * 3 + 1], &ptr, 10);
            }
        }
    }
//    printf("max = %d\n", max);

    node *nodes = (node *) malloc(sizeof(node) * max);

    for (i = 0; i < max; i++) {
        nodes[i].index = i + 1;
//        printf("nodes[%d].index = %d\n", i, nodes[i].index);

        nodes[i].from_num = 0;
        nodes[i].to_num = 0;
        nodes[i].pass = 0;

        nodes[i].to = (int *) malloc(sizeof(int *));
        nodes[i].from = (int *) malloc(sizeof(int *));
    }

    for (i = 0; i < count2; i++) {
        long a = strtol(text[i * 3], &ptr, 10) - 1;
        long b = strtol(text[i * 3 + 1], &ptr, 10) - 1;

        nodes[a].to_num++;
        nodes[a].to = (int *) realloc(nodes[a].to, sizeof(int *) * nodes[a].to_num);
        nodes[a].to[nodes[a].to_num - 1] = nodes[b].index;

        nodes[b].from_num++;
        nodes[b].from = (int *) realloc(nodes[b].from, sizeof(int *) * nodes[b].from_num);
        nodes[b].from[nodes[b].from_num - 1] = nodes[a].index;

        edges[i].index = i;
        edges[i].start = &nodes[a];
        edges[i].end = &nodes[b];
        edges[i].del = 0;
    }

//    for (i = 0; i < count2; i++) {
//        printf("edges[%d].index = %d edges[%d].start.index = %d edges[%d].end.index = %d\n", i, edges[i].index, i,
//               edges[i].start.index, i, edges[i].end.index);
//    }

//    for (i = 0; i < max; i++) {
//        printf("nodes[%d].index = %d nodes[%d].from_num = %d nodes[%d].to_num = %d\n", i, nodes[i].index, i,
//               nodes[i].from_num, i, nodes[i].to_num);
//        printf("nodes[%d].from =", i);
//        for (j = 0; j < nodes[i].from_num; j++) {
//            printf(" %d", nodes[i].from[j]);
//        }
//        printf("\n");
//        printf("nodes[%d].to =", i);
//        for (j = 0; j < nodes[i].to_num; j++) {
//            printf(" %d", nodes[i].to[j]);
//        }
//        printf("\n");
//    }

    for (j = 0; j < count3; j++) {
        cmd[j].cmd = strdup(text[count2 * 3 + j * 3]);
        cmd[j].first = strtol(text[count2 * 3 + j * 3 + 1], &ptr, 10);
        cmd[j].second = strtol(text[count2 * 3 + j * 3 + 2], &ptr, 10);

//        printf("cmd[%d].cmd = %s cmd[%d].first = %d cmd[%d].second = %d\n", j, cmd[j].cmd, j, cmd[j].first, j,
//               cmd[j].second);
    }
    ////////////////////////////////////////////////////////////////////////////
    for (i = 0; i < max; i++) {
        int temp = 0;
        for (j = 0; j < nodes[i].from_num; j++) {
            for (int k = 0; k < nodes[i].from_num - 1; k++) {
                if (nodes[i].from[k] > nodes[i].from[k + 1]) {
                    temp = nodes[i].from[k];
                    nodes[i].from[k] = nodes[i].from[k + 1];
                    nodes[i].from[k + 1] = temp;
                }
            }
        }
        for (j = 0; j < nodes[i].to_num; j++) {
            for (int k = 0; k < nodes[i].to_num - 1; k++) {
                if (nodes[i].to[k] > nodes[i].to[k + 1]) {
                    temp = nodes[i].to[j];
                    nodes[i].to[k] = nodes[i].to[k + 1];
                    nodes[i].to[k + 1] = temp;
                }
            }
        }
    }

    for (i = 0; i < count2; i++) {
        for (j = 0; j < count2 - 1; j++) {
            if (edges[j].start->index > edges[j + 1].start->index) {
                edge temp;
                temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
    for (i = 0; i < count2; i++) {
        for (j = 0; j < count2 - 1; j++) {
            if (edges[j].start->index == edges[j + 1].start->index) {
                if (edges[j].end->index > edges[j + 1].end->index) {
                    edge temp;
                    temp = edges[j];
                    edges[j] = edges[j + 1];
                    edges[j + 1] = temp;
                }
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////
    for (i = 0; i < count3; i++) {
        if (strcmp(cmd[i].cmd, "sch") == 0) {
//            printf("cmd[%d].cmd = %s cmd[%d].first = %d cmd[%d].second = %d\n", i, cmd[i].cmd, i, cmd[i].first, i,
//                   cmd[i].second);

//            printf("first = %d second = %d\n\n", cmd[i].first, cmd[i].second);

            for (j = 0; j < count2; j++) {
                if ((edges[j].start->index == cmd[i].first) && (edges[j].del == 0) && (edges[j].start->pass == 0)) {

                    int depth = 0;
//                    char array[count2 * 2];
//                    for (int k = 0; k < count2 + 1; k++) {
//                        array[k] = 0;
//                    }
                    int array[count2];
//                    for (int k = 0; k < count2; k++) {
//                        array[k] = NULL;
//                    }

//                    array[depth] = (char) (edges[j].start->index + '0');
//                    array[depth + 1] = ' ';
                    array[depth] = edges[j].start->index;

//                    printf("init: %d -> %d - going to next node...\n", edges[j].start->index, edges[j].end->index);
                    edges[j].start->pass = 1;
                    edges[j].end->pass = 1;
                    search_node(edges, nodes, &cmd[i], count2, edges[j], array, depth);
                    edges[j].start->pass = 0;
                    edges[j].end->pass = 0;
                }
            }

            if (success == 0) {
                printf("\n");
            }

            success = 0;

        } else if (strcmp(cmd[i].cmd, "mod") == 0) {
            for (j = 0; j < max; j++) {
                if (nodes[j].index == cmd[i].first) {
                    int two = nodes[j].from_num + nodes[j].to_num;
                    int *array = malloc(sizeof(int) * two);

                    for (int k = 0; k < two; k++) {
                        for (int p = 0; p < nodes[j].from_num; p++) {
                            array[p] = nodes[j].from[p];
                        }

                        for (int p = 0; p < nodes[j].to_num; p++) {
                            array[p + nodes[j].from_num] = nodes[j].to[p];
                        }

                        for (int p = 0; p < two; p++) {
                            for (int q = 0; q < two - 1; q++) {
                                if (array[q] > array[q + 1]) {
                                    int temp = array[q];
                                    array[q] = array[q + 1];
                                    array[q + 1] = temp;
                                }
                            }
                        }
                    }

//                    printf("array =");
//                    for (int p = 0; p < two; p++) {
//                        printf(" %d", array[p]);
//                    }
//                    printf("\n");

                    int c2 = 1;
                    int tmp = 0;

                    for (int k = 0; k < two; k++) {
                        if (array[k] != array[k + 1]) {
                            c2++;
                        }
                        if (c2 == cmd[i].second) {
                            tmp = array[k + 1];
                            break;
                        }
                    }

//                    printf("%d %d ", cmd[i].first, cmd[i].second);
//                    printf("c2 = %d ", c2 + 1);
//                    printf("array[k+1] = %d\n", tmp);

                    for (int k = 0; k < nodes[j].from_num; k++) {
                        if (nodes[j].from[k] == tmp) {

                            for (int p = 0; p < count2; p++) {
                                if ((edges[p].start->index == tmp) && (edges[p].end->index == nodes[j].index)) {
                                    edges[p].del = 1;
                                }
                            }

                            for (int p = k; p < nodes[j].from_num - 1; p++) {
                                nodes[j].from[p] = nodes[j].from[p + 1];
                            }
                            nodes[j].from_num--;

                            for (int p = 0; p < nodes[tmp - 1].to_num; p++) {
                                if (nodes[j].index == nodes[tmp - 1].to[p]) {
                                    for (int q = p; q < nodes[tmp - 1].to_num - 1; q++) {
                                        nodes[tmp - 1].to[q] = nodes[tmp - 1].to[q + 1];
                                    }
                                    nodes[tmp - 1].to_num--;
                                }
                            }
                        }
                    }

                    for (int k = 0; k < nodes[j].to_num; k++) {
                        if (nodes[j].to[k] == tmp) {

                            for (int p = 0; p < count2; p++) {
                                if ((edges[p].start->index == nodes[j].index) && (edges[p].end->index == tmp)) {
                                    edges[p].del = 1;
                                }
                            }

                            for (int p = k; p < nodes[j].to_num - 1; p++) {
                                nodes[j].to[p] = nodes[j].to[p + 1];
                            }
                            nodes[j].to_num--;

                            for (int p = 0; p < nodes[tmp - 1].from_num; p++) {
                                if (nodes[j].index == nodes[tmp - 1].from[p]) {
                                    for (int q = p; q < nodes[tmp - 1].from_num - 1; q++) {
                                        nodes[tmp - 1].from[q] = nodes[tmp - 1].from[q + 1];
                                    }
                                    nodes[tmp - 1].from_num--;
                                }
                            }
                        }

                    }

                    free(array);
                }
            }
        }
    }

//    for (i = 0; i < count2; i++) {
//        printf("edges[%d].index = %d edges[%d].start.index = %d edges[%d].end.index = %d\n", i, edges[i].index, i,
//               edges[i].start.index, i, edges[i].end.index);
//    }

//    for (i = 0; i < max; i++) {
//        printf("nodes[%d].index = %d nodes[%d].from_num = %d nodes[%d].to_num = %d\n", i, nodes[i].index, i,
//               nodes[i].from_num, i, nodes[i].to_num);
//        printf("nodes[%d].from =", i);
//        for (j = 0; j < nodes[i].from_num; j++) {
//            printf(" %d", nodes[i].from[j]);
//        }
//        printf("\n");
//        printf("nodes[%d].to =", i);
//        for (j = 0; j < nodes[i].to_num; j++) {
//            printf(" %d", nodes[i].to[j]);
//        }
//        printf("\n");
//    }

    ////////////////////////////////////////////////////////////////////////////
    free(line);
    free(text);
    free(cmd);
    free(nodes);
    free(edges);

    return 0;
}

void search_node(edge *edges, node *nodes, command *cmd, int count2, edge current, int *array, int depth) {
//    printf("cmd[i].cmd = %s cmd[i].first = %d cmd[i].second = %d\n", cmd[0].cmd, cmd[0].first, cmd[0].second);

//    depth += 2;
    depth++;

    if (current.end->index == cmd[0].second) {
//        array[depth] = (char) (current.end->index + '0');
//        array[depth + 1] = '\0';
        array[depth] = current.end->index;
//        printf("%s\n", array);
//        printf("finished!!\n");
//        array[depth + 1] = 0;
        for (int i = 0; i < depth; i++) {
            printf("%d ", array[i]);
        }
        printf("%d\n", array[depth]);
        array[depth] = 0;
        success++;
    } else {
        for (int i = 0; i < count2; i++) {
            if ((edges[i].start->index == current.end->index) && (edges[i].del == 0) && (edges[i].end->pass == 0)) {
                edges[i].end->pass = 1;
//                array[depth] = (char) (edges[i].start->index + '0');
//                array[depth + 1] = ' ';
                array[depth] = edges[i].start->index;
//                printf("%d -> %d used - going to next node...\n", edges[i].start->index, edges[i].end->index);
                search_node(edges, nodes, cmd, count2, edges[i], array, depth);
                edges[i].end->pass = 0;
//                array[depth + 2] = 0;
                array[depth] = 0;
            }
        }
    }

//    printf("going back...\n");
}