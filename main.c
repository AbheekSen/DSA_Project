#include "functions.h"

int main() {
    Product products[MAX_PRODUCTS];
    int pcount = 0;
    Graph g;
    Stack transactionStack;

    init_products(products, &pcount);
    init_sample_graph(&g);
    init_stack(&transactionStack);

    int choice;
    char id[ID_LEN];
    int qty;

    while (1) {
        printf("\n====== Warehouse Supply Chain Management ======\n");
        printf("1. Add Product\n2. Remove Product\n3. Display Inventory\n4. Display Routes\n");
        printf("5. Find Shortest Path to Supplier\n6. Auto-Refill Low Stock\n");
        printf("7. View Product Details\n8. Display Total Stock\n9. Count Suppliers and Products\n10. Display Suppliers and Connections\n0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter product ID: ");
                scanf("%s", id);
                printf("Enter quantity: ");
                scanf("%d", &qty);
                if (add_product(products, &pcount, id, qty, &transactionStack))
                    printf("Product %s added successfully.\n", id);
                break;

            case 2:
                printf("Enter product ID to remove: ");
                scanf("%s", id);
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
                    printf("To %s (%d): %d\n", g.names[i], i, dist[i]);
                }
                int nearest = find_nearest_supplier(&g, 0);
                if (nearest != -1)
                    printf("Nearest supplier: %s (node %d)\n", g.names[nearest], nearest);
                break;
            }

            case 6:
                auto_refill(products, pcount, &g, 10, 20);
                break;

            case 7:
                printf("Enter product ID: ");
                scanf("%s", id);
                Product* p = find_product(products, pcount, id);
                if (p) printf("Product ID: %s | Quantity: %d\n", p->id, p->quantity);
                else printf("Product not found.\n");
                break;

            case 8:
                printf("Total Stock: %d\n", total_stock(products, pcount));
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
                printf("Exiting program... Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
