#include "functions.h"

/* ---------------- STACK ---------------- */
void init_stack(Stack* s) { s->top = NULL; }
int is_stack_empty(Stack* s) { return s->top == NULL; }

void push(Stack* s, Product p) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) { perror("malloc"); return; }
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

/* ---------------- PRODUCT MANAGEMENT ---------------- */
void init_products(Product products[], int *pcount) { *pcount = 0; }

int add_product(Product products[], int *pcount, const char *id, int qty, Stack* stack) {
    if (!id) return 0;
    if (qty < 0) {
        printf("Cannot add product: quantity cannot be negative\n");
        return 0;
    }
    if (qty > CAPACITY) {
        printf("Cannot add product: quantity exceeds maximum limit (%d)\n", CAPACITY);
        return 0;
    }
    for (int i = 0; i < *pcount; ++i) {
        if (strcmp(products[i].id, id) == 0) {
            products[i].quantity = qty;
            if (stack) push(stack, products[i]);
            return 1;
        }
    }

    if (*pcount >= MAX_PRODUCTS) {
        printf("Cannot add product: maximum product count reached\n");
        return 0;
    }

    /* Add new product */
    strncpy(products[*pcount].id, id, ID_LEN - 1);
    products[*pcount].id[ID_LEN-1] = '\0';
    products[*pcount].quantity = qty;
    if (stack) push(stack, products[*pcount]);
    (*pcount)++;

    return 1;
}

int remove_product(Product products[], int *pcount, const char *id, Stack* stack) {
    if (!id) return 0;
    for (int i = 0; i < *pcount; ++i) {
        if (strcmp(products[i].id, id) == 0) {
            if (stack) push(stack, products[i]);
            for (int j = i; j < (*pcount) - 1; j++) products[j] = products[j+1];
            (*pcount)--;
            return 1;
        }
    }
    return 0;
}

void display_inventory(Product products[], int pcount) {
    printf("\nCurrent Inventory:\n----------------------------------------------\n");
    printf("%-16s | %-10s | %-8s\n", "ID", "Quantity", "Capacity");
    printf("----------------------------------------------\n");
    for (int i = 0; i < pcount; ++i) {
        printf("%-16s | %-10d | %-8d\n", products[i].id, products[i].quantity, CAPACITY);
    }
    printf("----------------------------------------------\n");
}

void display_remaining_space(Product products[], int pcount) {
    printf("\nRemaining Space (per product):\n----------------------------------------------\n");
    printf("%-16s | %-10s | %-10s\n", "ID", "Quantity", "Remaining");
    printf("----------------------------------------------\n");
    for (int i = 0; i < pcount; ++i) {
        int remaining = CAPACITY - products[i].quantity;
        if (remaining < 0) remaining = 0;
        printf("%-16s | %-10d | %-10d\n", products[i].id, products[i].quantity, remaining);
    }
    printf("----------------------------------------------\n");
}

Product* find_product(Product products[], int pcount, const char *id) {
    if (!id) return NULL;
    for (int i = 0; i < pcount; ++i) if (strcmp(products[i].id, id) == 0) return &products[i];
    return NULL;
}

int count_products(Product products[], int pcount) { return pcount; }

/* ---------------- GRAPH ---------------- */
void add_edge(Graph *g, int u, int v, int w) {
    if (!g) return;
    AdjNode* newNode = (AdjNode*)malloc(sizeof(AdjNode));
    if (!newNode) { perror("malloc"); return; }
    newNode->dest = v;
    newNode->weight = w;
    newNode->next = g->heads[u];
    g->heads[u] = newNode;
}

void init_sample_graph(Graph *g) {
    if (!g) return;
    g->n = 4;
    for (int i = 0; i < g->n; ++i) g->heads[i] = NULL;

    strncpy(g->names[0], "Warehouse", sizeof(g->names[0]) - 1);
    strncpy(g->names[1], "Supplier_A", sizeof(g->names[1]) - 1);
    strncpy(g->names[2], "Supplier_B", sizeof(g->names[2]) - 1);
    strncpy(g->names[3], "Supplier_C", sizeof(g->names[3]) - 1);

    g->suppliers[0] = 1;
    g->suppliers[1] = 2;
    g->suppliers[2] = 3;
    g->supplier_count = 3;

    add_edge(g, 0, 1, 10); add_edge(g, 1, 0, 10);
    add_edge(g, 0, 2, 25); add_edge(g, 2, 0, 25);
    add_edge(g, 0, 3, 20); add_edge(g, 3, 0, 20);
    add_edge(g, 1, 2, 15); add_edge(g, 2, 1, 15);
    add_edge(g, 1, 3, 50); add_edge(g, 3, 1, 50);
    add_edge(g, 2, 3, 30); add_edge(g, 3, 2, 30);
}

void display_routes(const Graph *g) {
    if (!g) return;
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
    if (!g) return;
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

int count_suppliers(const Graph *g) { if (!g) return 0; return g->supplier_count; }

/* ---------------- PRIORITY QUEUE ---------------- */
void init_pq(PriorityQueue* pq) { if (pq) pq->front = NULL; }
int is_pq_empty(PriorityQueue* pq) { return (pq == NULL) || (pq->front == NULL); }

void push_pq(PriorityQueue* pq, int vertex, int distance) {
    if (!pq) return;
    PQNode* newNode = (PQNode*)malloc(sizeof(PQNode));
    if (!newNode) { perror("malloc"); return; }
    newNode->vertex = vertex;
    newNode->distance = distance;
    newNode->next = NULL;

    if (pq->front == NULL || pq->front->distance > distance) {
        newNode->next = pq->front;
        pq->front = newNode;
    } else {
        PQNode* temp = pq->front;
        while (temp->next && temp->next->distance <= distance) temp = temp->next;
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

PQNode pop_pq(PriorityQueue* pq) {
    PQNode out = {-1, INT_MAX, NULL};
    if (!pq || !pq->front) return out;
    PQNode* temp = pq->front;
    out.vertex = temp->vertex;
    out.distance = temp->distance;
    pq->front = temp->next;
    free(temp);
    return out;
}

/* ---------------- DIJKSTRA ---------------- */
int dijkstra(const Graph *g, int src, int dest, int dist_out[MAX_NODES], int parent[MAX_NODES]) {
    if (!g) return 0;
    int n = g->n;
    int dist[MAX_NODES];
    int visited[MAX_NODES];
    for (int i = 0; i < n; ++i) {
        dist[i] = INT_MAX/2;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[src] = 0;

    PriorityQueue pq; init_pq(&pq);
    push_pq(&pq, src, 0);

    while (!is_pq_empty(&pq)) {
        PQNode u = pop_pq(&pq);
        int uvertex = u.vertex;
        if (uvertex < 0 || uvertex >= n) continue;
        if (visited[uvertex]) continue;
        visited[uvertex] = 1;

        AdjNode* cur = g->heads[uvertex];
        while (cur) {
            int v = cur->dest;
            int w = cur->weight;
            if (!visited[v] && dist[uvertex] + w < dist[v]) {
                dist[v] = dist[uvertex] + w;
                parent[v] = uvertex;
                push_pq(&pq, v, dist[v]);
            }
            cur = cur->next;
        }
    }

    for (int i = 0; i < n; ++i) dist_out[i] = dist[i];
    return dist[dest] < INT_MAX/2;
}

int find_nearest_supplier(const Graph *g, int src_node) {
    if (!g) return -1;
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

/* ---------------- AUTO REFILL ---------------- */
void auto_refill(Product products[], int pcount, Graph *g, int threshold, int reorder_qty) {
    printf("\nAuto-refill check: threshold=%d, reorder_qty=%d\n", threshold, reorder_qty);
    for (int i = 0; i < pcount; ++i) {
        if (products[i].quantity < threshold) {
            printf("Product %s: qty=%d < %d -> ", products[i].id, products[i].quantity, threshold);
            int nearest = find_nearest_supplier(g, 0);
            if (nearest != -1) {
                printf("nearest supplier: %s (node %d). ", g->names[nearest], nearest);
                int needed = reorder_qty;
                if (products[i].quantity + needed > CAPACITY) needed = CAPACITY - products[i].quantity;
                if (needed <= 0) {
                    printf("Already at or above capacity. No reorder needed.\n");
                } else {
                    printf("Reordering %d units...\n", needed);
                    products[i].quantity += needed;
                    printf("Product %s new qty=%d\n", products[i].id, products[i].quantity);
                }
            } else printf("no supplier reachable!\n");
        }
    }
}

/* ---------------- FREE GRAPH ---------------- */
void free_graph(Graph *g) {
    if (!g) return;
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
