#include "functions.h"

int main(void) {
    Product products[MAX_PRODUCTS];
    int pcount = 0;
    Graph g;
    Stack transactionStack;

    init_products(products, &pcount);
    init_sample_graph(&g);
    init_stack(&transactionStack);

    int choice = -1;
    char id[ID_LEN];
    int qty;

    while (1) {
        printf("\n====== Warehouse Supply Chain Management ======\n");
        printf("1. Add Product\n2. Remove Product\n3. Display Inventory\n4. Display Routes\n");
        printf("5. Find Shortest Path to Supplier\n6. Auto-Refill Low Stock\n");
        printf("7. View Product Details\n8. Display Remaining Space\n");
        printf("9. Count Suppliers and Products\n10. Display Suppliers and Connections\n0. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            /* clear bad input */
            int c; while ((c = getchar()) != EOF && c != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter product ID: ");
                if (scanf("%15s", id) != 1) { printf("Failed to read ID\n"); break; }
                printf("Enter quantity: ");
                if (scanf("%d", &qty) != 1) { printf("Failed to read quantity\n"); break; }
                if (add_product(products, &pcount, id, qty, &transactionStack))
                    printf("Product %s added/updated successfully.\n", id);
                break;

            case 2:
                printf("Enter product ID to remove: ");
                if (scanf("%15s", id) != 1) { printf("Failed to read ID\n"); break; }
                if (remove_product(products, &pcount, id, &transactionStack))
                    printf("Product %s removed successfully.\n", id);
                else printf("Product not found.\n");
                break;

            case 3:
                display_inventory(products, pcount);
                break;

            case 4:
                display_routes(&g);
                break;

            case 5: {
                int dist[MAX_NODES], parent[MAX_NODES];
                printf("\nShortest Distances from Warehouse (0):\n");
                for (int i = 1; i < g.n; ++i) {
                    dijkstra(&g, 0, i, dist, parent);
                    if (dist[i] >= INT_MAX/2) printf("To %s (%d): unreachable\n", g.names[i], i);
                    else printf("To %s (%d): %d\n", g.names[i], i, dist[i]);
                }
                {
                    int nearest = find_nearest_supplier(&g, 0);
                    if (nearest != -1) printf("Nearest Supplier: %s (%d)\n", g.names[nearest], nearest);
                    else printf("No supplier reachable.\n");
                }
                break;
            }

            case 6:
                auto_refill(products, pcount, &g, 10, 20);
                break;

            case 7:
                printf("Enter product ID: ");
                if (scanf("%15s", id) != 1) { printf("Failed to read ID\n"); break; }
                {
                    Product* p = find_product(products, pcount, id);
                    if (p) printf("Product ID: %s | Quantity: %d\n", p->id, p->quantity);
                    else printf("Product not found.\n");
                }
                break;

            case 8:
                display_remaining_space(products, pcount);
                break;

            case 9:
                printf("Number of Products: %d\n", count_products(products, pcount));
                printf("Number of Suppliers: %d\n", count_suppliers(&g));
                break;

            case 10:
                display_suppliers_and_connections(&g);
                break;

            case 0:
                free_graph(&g);
                printf("Exiting... Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

