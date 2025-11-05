#include "functions.h"

// ---------------- STACK IMPLEMENTATION ----------------
void init_stack(Stack* s) { s->top = NULL; }
int is_stack_empty(Stack* s) { return s->top == NULL; }

void push(Stack* s, Product p) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->data = p;
    newNode->next = s->top;
    s->top = newNode;
}

Product pop(Stack* s) {
    Product dummy = {"", 0};
    if (is_stack_empty(s)) return dummy;
    StackNode* temp = s->top;
    Product p = temp->data;
    s->top = temp->next;
    free(temp);
    return p;
}

// ---------------- PRODUCT MANAGEMENT ----------------
void init_products(Product products[], int *pcount) { *pcount = 0; }

int add_product(Product products[], int *pcount, const char *id, int qty, Stack* stack) {
    if (*pcount >= MAX_PRODUCTS) return 0;
    for (int i = 0; i < *pcount; ++i) {
        if (strcmp(products[i].id, id) == 0) {
            products[i].quantity = qty;
            push(stack, products[i]);
            return 1;
        }
    }
    strcpy(products[*pcount].id, id);
    products[*pcount].quantity = qty;
    push(stack, products[*pcount]);
    (*pcount)++;
    return 1;
}

int remove_product(Product products[], int *pcount, const char *id, Stack* stack) {
    for (int i = 0; i < *pcount; ++i) {
        if (strcmp(products[i].id, id) == 0) {
            push(stack, products[i]);
            for (int j = i; j < (*pcount) - 1; ++j)
                products[j] = products[j + 1];
            (*pcount)--;
            return 1;
        }
    }
    return 0;
}

void display_inventory(Product products[], int pcount) {
    printf("\nCurrent Inventory:\n-----------------------------\n");
    printf("%-10s | %-10s\n", "ID", "Quantity");
    printf("-----------------------------\n");
    for (int i = 0; i < pcount; ++i)
        printf("%-10s | %-10d\n", products[i].id, products[i].quantity);
    printf("-----------------------------\n");
}

Product* find_product(Product products[], int pcount, const char *id) {
    for (int i = 0; i < pcount; ++i)
        if (strcmp(products[i].id, id) == 0) return &products[i];
    return NULL;
}

int total_stock(Product products[], int pcount) {
    int total = 0;
    for (int i = 0; i < pcount; ++i) total += products[i].quantity;
    return total;
}

int count_products(Product products[], int pcount) { return pcount; }

// ---------------- GRAPH ----------------
void add_edge(Graph *g, int u, int v, int w) {
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    newNode->dest = v;
    newNode->weight = w;
    newNode->next = g->heads[u];
    g->heads[u] = newNode;
}

void init_sample_graph(Graph *g) {
    g->n = 4;
    for (int i = 0; i < g->n; ++i) g->heads[i] = NULL;

    strcpy(g->names[0], "Warehouse");
    strcpy(g->names[1], "Supplier_A");
    strcpy(g->names[2], "Supplier_B");
    strcpy(g->names[3], "Supplier_C");

    g->suppliers[0] = 1; g->suppliers[1] = 2; g->suppliers[2] = 3;
    g->supplier_count = 3;

    add_edge(g, 0, 1, 10); add_edge(g, 1, 0, 10);
    add_edge(g, 0, 2, 25); add_edge(g, 2, 0, 25);
    add_edge(g, 0, 3, 20); add_edge(g, 3, 0, 20);
    add_edge(g, 1, 2, 15); add_edge(g, 2, 1, 15);
    add_edge(g, 1, 3, 50); add_edge(g, 3, 1, 50);
    add_edge(g, 2, 3, 30); add_edge(g, 3, 2, 30);
}

void display_routes(const Graph *g) {
    printf("\nCurrent Supply Routes (Adjacency List):\n");
    for (int i = 0; i < g->n; ++i) {
        printf("%d (%s): ", i, g->names[i]);
        AdjNode* cur = g->heads[i];
        while (cur) {
            printf("-> %d(%d) ", cur->dest, cur->weight);
            cur = cur->next;
        }
        printf("\n");
    }
}

void display_suppliers_and_connections(const Graph *g) {
    printf("\nSuppliers and Their Connections:\n");
    for (int i = 1; i < g->n; ++i) {
        printf("%s (%d): ", g->names[i], i);
        AdjNode* cur = g->heads[i];
        while (cur) {
            printf("-> %s (%d) ", g->names[cur->dest], cur->dest);
            cur = cur->next;
        }
        printf("\n");
    }
}

int count_suppliers(const Graph *g) { return g->supplier_count; }

// ---------------- PRIORITY QUEUE (Linked List) ----------------
void init_pq(PriorityQueue* pq) { pq->front = NULL; }
int is_pq_empty(PriorityQueue* pq) { return pq->front == NULL; }

void push_pq(PriorityQueue* pq, int vertex, int distance) {
    PQNode* newNode = (PQNode*)malloc(sizeof(PQNode));
    newNode->vertex = vertex;
    newNode->distance = distance;
    newNode->next = NULL;

    if (pq->front == NULL || pq->front->distance > distance) {
        newNode->next = pq->front;
        pq->front = newNode;
    } else {
        PQNode* temp = pq->front;
        while (temp->next && temp->next->distance <= distance)
            temp = temp->next;
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

PQNode pop_pq(PriorityQueue* pq) {
    PQNode node = {-1, INT_MAX, NULL};
    if (is_pq_empty(pq)) return node;
    PQNode* temp = pq->front;
    node.vertex = temp->vertex;
    node.distance = temp->distance;
    pq->front = temp->next;
    free(temp);
    return node;
}

// ---------------- DIJKSTRA ----------------
int dijkstra(const Graph *g, int src, int dest, int dist_out[MAX_NODES], int parent[MAX_NODES]) {
    int n = g->n, dist[MAX_NODES], visited[MAX_NODES];
    for (int i = 0; i < n; ++i) { dist[i] = INT_MAX/2; visited[i] = 0; parent[i] = -1; }
    dist[src] = 0;

    PriorityQueue pq; init_pq(&pq);
    push_pq(&pq, src, 0);

    while (!is_pq_empty(&pq)) {
        PQNode u = pop_pq(&pq);
        if (visited[u.vertex]) continue;
        visited[u.vertex] = 1;

        AdjNode* cur = g->heads[u.vertex];
        while (cur) {
            int v = cur->dest, w = cur->weight;
            if (!visited[v] && dist[u.vertex] + w < dist[v]) {
                dist[v] = dist[u.vertex] + w;
                parent[v] = u.vertex;
                push_pq(&pq, v, dist[v]);
            }
            cur = cur->next;
        }
    }
    for (int i = 0; i < n; ++i) dist_out[i] = dist[i];
    return dist[dest] < INT_MAX/2;
}

int find_nearest_supplier(const Graph *g, int src_node) {
    int dist[MAX_NODES], parent[MAX_NODES];
    int minDist = INT_MAX, nearest = -1;
    for (int i = 0; i < g->supplier_count; ++i) {
        int s = g->suppliers[i];
        if (dijkstra(g, src_node, s, dist, parent) && dist[s] < minDist) {
            minDist = dist[s]; nearest = s;
        }
    }
    return nearest;
}

// ---------------- AUTO REFILL ----------------
void auto_refill(Product products[], int pcount, Graph *g, int threshold, int reorder_qty) {
    printf("\nAuto-refill check: threshold=%d, reorder_qty=%d\n", threshold, reorder_qty);
    for (int i = 0; i < pcount; ++i) {
        if (products[i].quantity < threshold) {
            printf("Product %s: qty=%d < %d -> ", products[i].id, products[i].quantity, threshold);
            int nearest = find_nearest_supplier(g, 0);
            if (nearest != -1) {
                printf("nearest supplier: %s (node %d). Reordering %d units...\n",
                       g->names[nearest], nearest, reorder_qty);
                products[i].quantity += reorder_qty;
                printf("Product %s new qty=%d\n", products[i].id, products[i].quantity);
            } else printf("no supplier reachable!\n");
        }
    }
}

void free_graph(Graph *g) {
    for (int i = 0; i < g->n; ++i) {
        AdjNode* cur = g->heads[i];
        while (cur) {
            AdjNode* tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        g->heads[i] = NULL;
    }
}
