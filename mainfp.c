#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "dsa.h"
#define FALSE 0
#define TRUE 1
#define MAX_ITEM 10000
#define MAX_CART_SIZE 10

float minSupport = 0.05;
int SUPPORT = 0;
char *nullChar = "\0";

int main()
{
        FILE *stream = fopen("groceries_subset.csv", "r");
        if (stream == NULL)
        {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
        }
        char items[100][100];
        char ch;
        // char *data_item = (char *)malloc(1000 * sizeof(char));
        int data_item;
        itemsTable itemsCounter[MAX_ITEM];
        miningTable miningCounter[MAX_ITEM];
        order table[MAX_ITEM];
        int cart[MAX_CART_SIZE];
        int cartSize = 0;

        initialize_items_table(&itemsCounter[0]);
        // int order_row=-1;
        int last_index = -1, totalTrans = 0;
        totalTrans = processFile(stream, itemsCounter, &last_index);
        // printf("Total Trans:%d\n", totalTrans);

        fclose(stream);

        sortDesc(itemsCounter, last_index + 1);

        SUPPORT = ceil(totalTrans * minSupport);
        supportFilter(itemsCounter, &last_index);

        int row_count = orderTable(itemsCounter, table, last_index, items);
        constructBaseFPtree(itemsCounter, table, row_count);
        copyToMiningTable(itemsCounter, miningCounter, last_index);

        int prefix[1000];
        prefix[0] = -1;
        mineFPtree(itemsCounter, miningCounter, last_index, table, row_count, prefix);

        //     printf("Last:%d",last_index);
        //     print_tree(itemsCounter,table, last_index,items);
        // printf("Items:\n");
        // for (int i = 0; i < last_index; i++)
        // {
        //         printf("%s\n", items[i]);
        // }
        // int size=sizeof(table)
        displayOrderTable(table, row_count, items);
        displayMiningTable(miningCounter, last_index, itemsCounter, items);

        int choice;
        do
        {
                printf("\n\nMenu:\n");
                printf("1. Display Items\n");
                printf("2. Print Top Items\n");
                printf("3. Display Cart\n");
                printf("4. Add Item to Cart\n");
                printf("5. Remove Item from Cart\n");
                printf("6. Recommend Items\n");
                printf("7. See Implementation\n");
                printf("0. Exit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                        displayItems(itemsCounter, last_index);
                        printf("\n");
                        break;
                case 2:
                        printTopItems(itemsCounter, last_index);
                        printf("\n");
                        break;
                case 3:
                        displayCart(cart, cartSize, items);
                        printf("\n");
                        break;
                case 4:
                        displayItems(itemsCounter, last_index);
                        printf("Enter the item to add to the cart: ");
                        scanf("%d", &data_item);
                        addToCart(data_item - 1, cart, &cartSize, items);
                        printf("\n");
                        break;
                case 5:
                        displayCart(cart, cartSize, items);
                        printf("Enter the item to remove from the cart: ");
                        scanf("%d", &data_item);
                        removeFromCart(data_item - 1, cart, &cartSize, items);
                        printf("\n");
                        break;
                case 6:
                        // recommendItems(cart, cartSize, itemsCounter, last_index, items);
                        break;
                case 7:
                
                        implementaionMenu(itemsCounter,table,miningCounter,last_index,row_count,items);
                        printf("\n");
                        break;             

                case 0:
                        printf("Exiting...\n");
                        break;
                default:
                        printf("Invalid choice. Please try again.\n");
                        break;
                }
        } while (choice != 0);

        return 0;
}
