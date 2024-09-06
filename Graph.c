//
// Created by Rahul on 8/30/2024.
//
#include <math.h>
#include <stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
typedef struct Node {
    int distance;
    int vertex;
    struct Node *next;
} Node;

typedef struct Graph {
    int numVertices;
    Node **adjList;
} Graph;


typedef struct MinHeap {
    int top;
    int capacity;
    Node **queue;
} MinHeap;

MinHeap *createMinHeap(int n) {
    MinHeap *min_heap = malloc(sizeof(MinHeap));
    min_heap->capacity = n;
    min_heap->top = -1;
    min_heap->queue = malloc(sizeof(Node *) * n);
    return min_heap;
}

Graph *createGraph(int V) {
    Graph *graph = malloc(sizeof(Graph));
    graph->numVertices = V;
    graph->adjList = malloc(sizeof(Node *) * V);
    for (int i = 0; i < V; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

Node *createNode(int dest, int w) {
    Node *node = malloc(sizeof(Node));
    node->vertex = dest;
    node->distance = w;
    node->next = NULL;
    return node;
}


void addEdge(Graph *graph, int src, int dest, int w) {
    Node *node1 = createNode(dest, w);
    node1->next = graph->adjList[src];
    graph->adjList[src] = node1;

    Node *node2 = createNode(src, w);


    node2->next = graph->adjList[dest];
    graph->adjList[dest] = node2;
}

void dfs(Graph *graph, bool *visited, int vertex) {
    printf("%d  ", vertex);
    Node *node1 = graph->adjList[vertex];
    while (node1 != NULL) {
        printf("%d (%d)  ", node1->vertex, node1->distance);
        node1 = node1->next;
    }
    printf("\n");
    vertex[visited] = true;
    Node *node = graph->adjList[vertex];
    while (node != NULL) {
        if (!visited[node->vertex]) {
            dfs(graph, visited, node->vertex);
        }
        node = node->next;
    }
}

void displayGraph(Graph *graph) {
    bool visited[graph->numVertices];
    dfs(graph, visited, 0);
}

void displayAdjacentLists(Graph *graph) {
    for (int i = 0; i < graph->numVertices; i++) {
        Node *node = graph->adjList[i];

        while (node != NULL) {
            printf("%d (%d)  ", node->vertex, node->distance);
            node = node->next;
        }
        printf("\n");
    }
}

int getParent(int i) {
    return (i - 1) / 2;
}

int getLeftChild(int i) {
    return 2 * i + 1;
}

int getRightChild(int i) {
    return 2 * i + 2;
}

void shiftUp(MinHeap *min_heap, int i) {
    while (i > 0 && min_heap->queue[i]->distance < min_heap->queue[getParent(i)]->distance) {
        Node *temp = min_heap->queue[i];
        min_heap->queue[i] = min_heap->queue[getParent(i)];
        min_heap->queue[getParent(i)] = temp;
        i = getParent(i);
    }
}

void insertMinHeap(MinHeap *min_heap, Node *node) {
    min_heap->queue[++min_heap->top] = node;
    shiftUp(min_heap, min_heap->top);
}

bool isEmpty(MinHeap *pq) {
    if (pq->top == -1) {
        return true;
    }
    return false;
}

void displayMinHeap(MinHeap *min_heap) {
    for (int i = 0; i <= min_heap->top; i++) {
        printf("%d (%d)", min_heap->queue[i]->vertex, min_heap->queue[i]->distance);
    }
    printf("\n");
}


void shiftDown(MinHeap *pq, int i) {
    int l = getLeftChild(i);
    int r = getRightChild(i);
    int m = i;
    if (l <= pq->top && pq->queue[l]->distance < pq->queue[m]->distance) {
        m = l;
    }
    if (r <= pq->top && pq->queue[r]->distance < pq->queue[m]->distance) {
        m = r;
    }
    if (i != m) {
        Node *temp = pq->queue[i];
        pq->queue[i] = pq->queue[m];
        pq->queue[m] = temp;

        shiftDown(pq, m);
    }
}

Node *extractMin(MinHeap *pq) {
    Node *result = pq->queue[0];
    pq->queue[0] = pq->queue[pq->top--];
    shiftDown(pq, 0);
    return result;
}

int getSize(MinHeap *min_heap) {
    return min_heap->top + 1;
}

void dijkstra(Graph *graph, int src) {
    int distances[graph->numVertices];
    for (int i = 0; i < graph->numVertices; i++) {
        distances[i] = INFINITY;
    }

    distances[src] = 0;


    MinHeap *pq = createMinHeap(graph->numVertices);
    Node *srcNode = createNode(src, 0);
    insertMinHeap(pq, srcNode);
    int count = 0;
    while (!isEmpty(pq)) {
        Node *node = extractMin(pq);

        int u = node->vertex;
        Node *adjNode = graph->adjList[u];
        while (adjNode) {
            count++;
            int v = adjNode->vertex;
            int cost_u_v = adjNode->distance;
            if (distances[u] != INFINITY && distances[u] + cost_u_v < distances[v]) {
                insertMinHeap(pq, adjNode);
                distances[v] = distances[u] + cost_u_v;
                // distances[v] = distances[u] + cost_u_v;
                // Node *temp = createNode(v, distances[v]);
                // insertMinHeap(pq, temp);
            }
            adjNode = adjNode->next;
        }
    }


    printf("Updated distances are:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%d  ", distances[i]);
    }
}

int main() {
    Graph *graph = createGraph(7);


    addEdge(graph, 3, 0, 4); // D - A, weight 4
    addEdge(graph, 3, 4, 2); // D - E, weight 2
    addEdge(graph, 0, 2, 3); // A - C, weight 3
    addEdge(graph, 0, 4, 4); // A - E, weight 4
    addEdge(graph, 4, 2, 4); // E - C, weight 4
    addEdge(graph, 4, 6, 5); // E - G, weight 5
    addEdge(graph, 2, 5, 5); // C - F, weight 5
    addEdge(graph, 2, 1, 2); // C - B, weight 2
    addEdge(graph, 1, 5, 2); // B - F, weight 2
    addEdge(graph, 6, 5, 5); // G - F, weight 5

    for (int i = 0; i < graph->numVertices; i++) {
        dijkstra(graph, i);
        printf("\n");
    }

    return 0;
}
