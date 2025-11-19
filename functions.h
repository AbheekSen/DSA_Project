#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_PRODUCTS 100
#define MAX_NODES 10
#define MAX_SUPPLIERS 5
#define ID_LEN 16     
#define CAPACITY 1000 


typedef struct {
    char id[ID_LEN];
    int quantity;
} Product;

typedef struct StackNode {
    Product data;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} Stack;

void init_stack(Stack* s);
int is_stack_empty(Stack* s);
void push(Stack* s, Product p);
Product pop(Stack* s);

typedef struct AdjNode {
    int dest;
    int weight;
    struct AdjNode* next;
} AdjNode;

typedef struct {
    int n;
    AdjNode* heads[MAX_NODES];
    char names[MAX_NODES][32];
    int suppliers[MAX_SUPPLIERS];
    int supplier_count;
} Graph;

void init_sample_graph(Graph *g);
void add_edge(Graph *g, int u, int v, int w);
void display_routes(const Graph *g);
void display_suppliers_and_connections(const Graph *g);
int count_suppliers(const Graph *g);

typedef struct PQNode {
    int vertex;
    int distance;
    struct PQNode* next;
} PQNode;

typedef struct {
    PQNode* front;
} PriorityQueue;

void init_pq(PriorityQueue* pq);
int is_pq_empty(PriorityQueue* pq);
void push_pq(PriorityQueue* pq, int vertex, int distance);
PQNode pop_pq(PriorityQueue* pq);

void init_products(Product products[], int *pcount);
int add_product(Product products[], int *pcount, const char *id, int qty, Stack* stack);
int remove_product(Product products[], int *pcount, const char *id, Stack* stack);
void display_inventory(Product products[], int pcount);
void display_remaining_space(Product products[], int pcount);
Product* find_product(Product products[], int pcount, const char *id);
int count_products(Product products[], int pcount);

int dijkstra(const Graph *g, int src, int dest, int dist_out[MAX_NODES], int parent[MAX_NODES]);
int find_nearest_supplier(const Graph *g, int src_node);
void auto_refill(Product products[], int pcount, Graph *g, int threshold, int reorder_qty);
void free_graph(Graph *g);

#endif 
