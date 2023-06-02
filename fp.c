#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "fp.h"
#define MAX_CART_SIZE 10

void initialize_node(node *temp)
{
        temp->count = 0;
        temp->item = -1;
        temp->parent = temp->sibling = temp->child = temp->level = NULL;
}
void initialize_items_table(itemsTable *temp)
{
        temp->count = 0;
        temp->item = nullChar;
        temp->reference = NULL;
        temp->last_sibling = NULL;
}
void initialize_mining_table(miningTable *temp)
{
        temp->count = 0;
        temp->item = -1;
        temp->reference = NULL;
        temp->last_sibling = NULL;
}
void initialize_order(order *temp)
{
        temp->len = 0;
        temp->count = 0;
}

Map initMap(int capacity)
{
        Map map;
        map.data = (KeyValuePair *)malloc(capacity * sizeof(KeyValuePair));
        map.size = 0;
        map.capacity = capacity;
        return map;
}

// Function to insert a key-value pair into the map
void insert(Map *map, int key, const char *value)
{
        // Check if the key already exists in the map
        for (int i = 0; i < map->size; i++)
        {
                KeyValuePair *pair = &(map->data[i]);
                if (pair->key == key)
                {
                        // Key already exists, check if the new token is a substring of the old token
                        if (strstr(pair->value, value) != NULL)
                        {
                                // New token already exists in the old token, no need to concatenate
                                return;
                        }

                        // Key already exists, concatenate the new value
                        size_t newLength = strlen(pair->value) + strlen(value) + 4;
                        char *newValue = (char *)malloc(newLength * sizeof(char));
                        strcpy(newValue, pair->value);
                        strcat(newValue, " , ");
                        strcat(newValue, value);
                        free(pair->value);
                        pair->value = newValue;
                        //     printf("Inserted: %s\n", pair->value);
                        return;
                }
        }

        // Key does not exist, create a new key-value pair
        KeyValuePair *pair = &(map->data[map->size]);
        pair->key = key;
        pair->value = (char *)malloc((strlen(value) + 1) * sizeof(char));
        strcpy(pair->value, value);

        map->size++;
}

char *get(Map *map, int key)
{
        for (int i = 0; i < map->size; i++)
        {
                KeyValuePair pair = (map->data[i]);
                if (pair.key == key)
                {
                        return pair.value;
                }
        }

        // Key not found
        return NULL;
}

int find_index(int item, miningTable miningCounter[], int miningCount)
{
        int i = -1;
        for (i = 0; i <= miningCount; ++i)
        {
                if (miningCounter[i].item == item)
                        break;
        }
        return i;
}
void sortDesc(itemsTable itemsCounter[10000], int last_index)
{
        // Sorts the entries in itemsCounter in descending order.

        int i, j;
        itemsTable temp;

        for (i = 0; i < last_index - 1; i++)
        {
                for (j = 0; j < (last_index - 1 - i); j++)
                {
                        if (itemsCounter[j].count < itemsCounter[j + 1].count)
                        {
                                temp = itemsCounter[j];
                                itemsCounter[j] = itemsCounter[j + 1];
                                itemsCounter[j + 1] = temp;
                        }
                }
        }
}

void supportFilter(itemsTable itemsCounter[10000], int *item_count)
{
        // Discards items from itemsCounter having frquency less than the support count.

        int last_index = *item_count;
        int supportCount = SUPPORT;

        int i = 0, j;
        while (i <= last_index)
        {
                if (itemsCounter[i].count < supportCount)
                {
                        for (j = i; j < last_index; j++)
                        {
                                itemsCounter[j] = itemsCounter[j + 1];
                        }
                        last_index--;
                }
                else
                {
                        i++;
                }
        }
        *item_count = last_index;
}
void displayItems(itemsTable itemsCounter[], int item_count)
{

        // display items in items counter along with frequenies.
        printf("SrNo. - Item \n");
        for (int j = 0; j <= item_count; j++)
        {
                printf("%4d  - %s \n", j + 1, itemsCounter[j].item);
        }
}
void orderTransaction(char *transaction_data, itemsTable itemsCounter[], order table[], int order_row, int item_count, char items[][100])
{

        // Orders transaction according to decreasing order of frequencies.

        char *data_item = (char *)malloc(1000 * sizeof(char));

        int k = 0;
        int trans_iter = 0;
        int found;
        initialize_order(&table[order_row]);

        for (int i = 0; i <= item_count; ++i)
        {
                trans_iter = 0;
                found = FALSE;
                while (transaction_data[trans_iter] != '\0' && found == FALSE)
                {

                        if (transaction_data[trans_iter] != ',')
                                data_item[k++] = transaction_data[trans_iter];
                        else
                        {
                                data_item[k] = '\0';
                                if (strcmp(data_item, itemsCounter[i].item) == 0)
                                {
                                        table[order_row].array[table[order_row].len++] = i;
                                        strcpy(items[i], data_item);
                                        found = TRUE;
                                }
                                free(data_item);
                                data_item = NULL;
                                data_item = (char *)malloc(1000 * sizeof(char));
                                k = 0;
                        }
                        trans_iter++;
                }
        }
}
int orderTable(itemsTable itemsCounter[10000], order table[10000], int item_count, char items[][100])
{

        // Orders table of transactions using orderTransaction function

        // displayItems(itemsCounter, item_count);

        FILE *stream = fopen("groceries_subset.csv", "r");
        if (stream == NULL)
        {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
        }

        char ch;
        char *transaction_data = (char *)malloc(1000 * sizeof(char));
        strcpy(transaction_data, "\0");
        char *data_item = (char *)malloc(1000 * sizeof(char));

        int i = 0, j = 0, items_i = 0, last_index = -1, flag, seen1 = 0, seen2 = 0, totalTrans = 0, order_row = -1;
        char prev_ch = '1';
        while ((ch = fgetc(stream)) != EOF)
        {

                if (isalpha(ch) || ch == ' ' || ch == '/' || ch == '-' || ch == '.')
                {

                        if (seen1 != 0)
                        {
                                data_item[i] = ch;
                                i++;
                                seen2 = 1;
                        }
                }

                else if (ch == ',' || ch == '\n')
                {
                        if (isdigit(prev_ch))
                        {
                                totalTrans++;
                                order_row++;
                                if (totalTrans > 1)
                                {
                                        orderTransaction(transaction_data, itemsCounter, table, order_row - 1, item_count, items);
                                        free(transaction_data);
                                        transaction_data = NULL;
                                        transaction_data = (char *)malloc(10000 * sizeof(char));
                                        strcpy(transaction_data, "\0");
                                }
                        }
                        if (seen1 == 0)
                        {
                                seen1 = 1;
                        }
                        else if (seen2 == 1)
                        {
                                seen2 = 0;
                                data_item[i] = ',';
                                data_item[i + 1] = '\0';
                                strcat(transaction_data, data_item);

                                free(data_item);
                                data_item = NULL;
                                data_item = (char *)malloc(100 * sizeof(char));
                                i = 0;
                        }
                }
                prev_ch = ch;
        }

        orderTransaction(transaction_data, itemsCounter, table, order_row, item_count, items);
        free(transaction_data);
        transaction_data = NULL;

        fclose(stream);
        return totalTrans;
}
void constructBaseFPtree(itemsTable itemsCounter[], order table[], int row_count)
{

        // constructs initial FP tree based on transactions from the file.

        node *parent, *root, *current, *level_traverse;
        // phase 1
        root = (node *)malloc(sizeof(node));
        initialize_node(root);
        for (int i = 0; i < row_count; ++i)
        {
                current = root;
                for (int j = 0; j < table[i].len; ++j)
                {
                        int found = FALSE;
                        parent = current;
                        current = parent->child;
                        if (current == NULL && found == FALSE)
                        {
                                current = (node *)malloc(sizeof(node));
                                initialize_node(current);
                                current->item = table[i].array[j];
                                current->count = 1;
                                current->parent = parent;
                                parent->child = current;
                        }
                        else
                        {
                                node **prev_level_pointer;
                                level_traverse = current;
                                while (level_traverse != NULL && found == FALSE)
                                {
                                        if (level_traverse->item == table[i].array[j])
                                        {
                                                found = TRUE;
                                                level_traverse->count++;
                                                break;
                                        }
                                        prev_level_pointer = &(level_traverse->level);
                                        level_traverse = level_traverse->level;
                                }
                                if (level_traverse == NULL)
                                {
                                        level_traverse = (node *)malloc(sizeof(node));
                                        initialize_node(level_traverse);
                                        level_traverse->item = table[i].array[j];
                                        level_traverse->count = 1;
                                        level_traverse->parent = parent;
                                        *prev_level_pointer = level_traverse;
                                }
                                current = level_traverse;
                        }
                        // setting sibling links
                        if (found == FALSE)
                        {
                                int l = current->item;
                                if (itemsCounter[l].reference == NULL)
                                        itemsCounter[l].reference = current;
                                node **prev_sib_link = &(itemsCounter[l].last_sibling->sibling);
                                itemsCounter[l].last_sibling = current;
                                if (itemsCounter[l].last_sibling != itemsCounter[l].reference)
                                        *prev_sib_link = current;
                        }
                }
        }
}
void constructFPtree(miningTable miningCounter[], int miningCount, order table[], int row_count)
{

        // Construct FP tree.

        node *parent, *root, *current, *level_traverse;
        // phase 1
        root = (node *)malloc(sizeof(node));
        initialize_node(root);
        for (int i = 0; i < row_count; ++i)
        {
                current = root;
                for (int j = 0; j < table[i].len; ++j)
                {
                        int found = FALSE;
                        parent = current;
                        current = parent->child;
                        if (current == NULL && found == FALSE)
                        {
                                current = (node *)malloc(sizeof(node));
                                initialize_node(current);
                                current->item = table[i].array[j];
                                current->count = table[i].count;
                                current->parent = parent;
                                parent->child = current;
                        }
                        else
                        {
                                node **prev_level_pointer;
                                level_traverse = current;
                                while (level_traverse != NULL && found == FALSE)
                                {
                                        if (level_traverse->item == table[i].array[j])
                                        {
                                                found = TRUE;
                                                level_traverse->count += table[i].count;
                                                break;
                                        }
                                        prev_level_pointer = &(level_traverse->level);
                                        level_traverse = level_traverse->level;
                                }
                                if (level_traverse == NULL)
                                {
                                        level_traverse = (node *)malloc(sizeof(node));
                                        initialize_node(level_traverse);
                                        level_traverse->item = table[i].array[j];
                                        level_traverse->count = table[i].count;
                                        level_traverse->parent = parent;
                                        *prev_level_pointer = level_traverse;
                                }
                                current = level_traverse;
                        }
                        // setting sibling links
                        if (found == FALSE)
                        {
                                int current_item = current->item;
                                int l = find_index(current_item, miningCounter, miningCount);

                                if (miningCounter[l].reference == NULL)
                                        miningCounter[l].reference = current;
                                node **prev_sib_link = &(miningCounter[l].last_sibling->sibling);
                                miningCounter[l].last_sibling = current;
                                if (miningCounter[l].last_sibling != miningCounter[l].reference)
                                        *prev_sib_link = current;
                        }
                }
        }
}
void print_tree(itemsTable itemsCounter[], order table[], int item_count, char items[][100])
{

        // prints FP tree

        node *current_node, *current_sibling;
        for (int i = 0; i <= item_count; ++i)
        {
                printf("item: %s count: %d item_sets: ", items[i], itemsCounter[i].count);
                current_sibling = itemsCounter[i].reference;

                int k = 0;
                while (current_sibling != NULL)
                {
                        k++;
                        current_node = current_sibling;
                        printf("\n");
                        while (current_node->parent != NULL)
                        {
                                // printf("%d ", current_node->item);
                                printf("%s ", items[current_node->item]);
                                current_node = current_node->parent;
                        }
                        current_sibling = current_sibling->sibling;
                }
                printf("%d\n", k);
        }
        printf("\n");
}

void copyToMiningTable(itemsTable itemsCounter[], miningTable miningCounter[], int item_count)
{

        // Copies contents of itemsCounter to mining Counter except the item string.

        for (int i = 0; i <= item_count; ++i)
        {
                initialize_mining_table(&miningCounter[i]);
                miningCounter[i].item = i;
                miningCounter[i].count = itemsCounter[i].count;
                miningCounter[i].last_sibling = itemsCounter[i].last_sibling;
                miningCounter[i].reference = itemsCounter[i].reference;
        }
}

int findItemIndex(char items[][100], char *item)
{
        for (int i = 0; i < 100; i++)
        {
                if (strcmp(items[i], item) == 0)
                {
                        return i;
                }
        }
}

void mineFPtree(itemsTable itemsCounter[], miningTable old_miningCounter[], int old_miningCount, order old_table[], int old_row_count, int prefix[], char items[][100], Map *pairs, int show)
{

        // Recursive function for mining FP tree.

        if (old_miningCount < 0)
        {
                // printf("\n--------------------- end ---------------------\n");
                return;
        }
        char prefix_string[1000] = "\0";
        int last_index = 0;
        insert(pairs, 0, "rolls");
        // printf("%d",prefix[last_index]);
        while (prefix[last_index] != -1)
        {
                strcat(prefix_string, itemsCounter[prefix[last_index]].item);
                strcat(prefix_string, ",");
                last_index++;
        }

        // printf("Prefix%s:",prefix_string);

        for (int i = 0; i <= old_miningCount; ++i)
        {

                order table[1000];
                miningTable miningCounter[1000];
                int new_prefix[1000];
                int miningCount = -1;
                int row_count = 0;

                int item = old_miningCounter[i].item;
                int item_total_count = old_miningCounter[i].count;

                // printf("main item: %d\n", item);
                // printf("prefix: \n");
                for (int j = 0; j < last_index; ++j)
                {
                        new_prefix[j] = prefix[j];
                        // printf("newprefix%d ", new_prefix[j]);
                }
                new_prefix[last_index] = item;
                // printf("%d\n", item);
                new_prefix[last_index + 1] = -1;

                // printf("Itemscounter:%s\n",itemsCounter[item].item);
                // printf("Prefix_string:{%s}\n",prefix_string);
                if (prefix_string[0] != '\0')
                {
                        char *token = strtok(prefix_string, ",");
                        int itemIndex = findItemIndex(items, token);
                        insert(pairs, itemIndex, itemsCounter[item].item);
                        if(show){
                        printf("%s %s : %d\n", prefix_string, itemsCounter[item].item, old_miningCounter[i].count);
                        }
                }
                // printf("transaction:: {%s%s}:%d\n", prefix_string, itemsCounter[item].item, old_miningCounter[i].count);
                // printf("transaction:: {%s}:%d\n", itemsCounter[item].item, old_miningCounter[i].count);

                node *current_node, *current_sibling;

                current_sibling = old_miningCounter[i].reference;

                int k = 0;
                while (current_sibling != NULL)
                {
                        initialize_order(&table[k]);
                        int sibling_count = current_sibling->count;
                        table[k].count = sibling_count;

                        current_node = current_sibling->parent;
                        while (current_node->parent != NULL)
                        {
                                int current_item = current_node->item;
                                int found = FALSE;
                                table[k].array[table[k].len++] = current_item;

                                for (int j = 0; j <= miningCount && found == FALSE; ++j)
                                {
                                        if (miningCounter[j].item == current_item)
                                        {
                                                found = TRUE;
                                                miningCounter[j].count += sibling_count;
                                        }
                                }

                                if (found == FALSE)
                                {
                                        miningCount++;
                                        initialize_mining_table(&miningCounter[miningCount]);
                                        miningCounter[miningCount].item = current_item;
                                        miningCounter[miningCount].count = sibling_count;
                                }
                                current_node = current_node->parent;
                        }
                        current_sibling = current_sibling->sibling;
                        k++;
                }
                row_count = k;
                int supportCount = SUPPORT;
                for (int j = 0; j < miningCount; ++j)
                {
                        for (int k = j + 1; k <= miningCount; ++k)
                        {
                                if (miningCounter[j].count < miningCounter[k].count)
                                {
                                        int temp = miningCounter[j].item;
                                        miningCounter[j].item = miningCounter[k].item;
                                        miningCounter[k].item = temp;
                                        temp = miningCounter[j].count;
                                        miningCounter[j].count = miningCounter[k].count;
                                        miningCounter[k].count = temp;
                                }
                        }
                }
                for (int j = 0; j < miningCount; ++j)
                {
                        if (miningCounter[j].count < supportCount)
                        {
                                miningCount = j - 1;
                                break;
                        }
                }

                if (miningCount >= 0 && miningCounter[miningCount].count < supportCount)
                        miningCount--;

                for (int k = 0; k < row_count; ++k)
                {
                        int found = FALSE;
                        int current_index = 0;
                        for (int j = 0; j <= miningCount; ++j)
                        {
                                for (int l = current_index; l < table[k].len; ++l)
                                {
                                        if (table[k].array[l] == miningCounter[j].item)
                                        {
                                                if (l != current_index)
                                                {
                                                        int temp = table[k].array[l];
                                                        table[k].array[l] = table[k].array[current_index];
                                                        table[k].array[current_index] = temp;
                                                }
                                                current_index++;
                                        }
                                }
                        }
                        table[k].len = current_index;
                }
                constructFPtree(miningCounter, miningCount, table, row_count);

                mineFPtree(itemsCounter, miningCounter, miningCount, table, row_count, new_prefix, items, pairs, show);
        }
}

void displayTopItems(itemsTable itemsCounter[], int item_count)
{

        // Sort the itemsCounter array in descending order based on count

        sortDesc(itemsCounter, item_count);

        printf("Top Selling Items:\n\n");
        int count = 0;
        for (int i = 0; i < item_count; ++i)
        {
                if (itemsCounter[i].count > 0)
                {
                        printf("%20s \t", itemsCounter[i].item);
                        count++;
                        if (count == 12)
                        {
                                break;
                        }
                        if (count % 3 == 0)
                        {
                                printf("\n");
                        }
                }
        }
}

void recommendItems(int cart[], int cartSize, char items[][100], Map pairs)
{

        if (cartSize == 0)
        {
                printf("\nYour cart is Empty\nTry our most selling products:\t  ");
                for (int i = 0; i < 5; i++)
                {
                        printf("%s    ,", items[i]);
                }
                return;
        }
        // Iterate over the items in the cart
        for (int i = 0; i < cartSize; i++)
        {
                char *rec = get(&pairs, cart[i]);
                if (rec != NULL)
                        printf("%d.%s=>  %s  \n", i + 1, items[i], rec);
        }
}

// void traverseChildNodes(node* parent, itemsTable itemsCounter[], int last_index, char items[][100]) {
//     node* current = parent->child;

//     // Traverse all child nodes
//     while (current != NULL) {
//         // Get the item name from itemsCounter
//         // char* itemName = itemsCounter[current->item].item;
//         char* itemName = items[current->item];

//         // Print the item name
//         printf("%s ", itemName);

//         // Recursively traverse child nodes of the current node
//         traverseChildNodes(current, itemsCounter, last_index, items);

//         current = current->sibling;
//     }
// }

void displayCart(int cart[], int cartSize, char items[][100])
{
        if (cartSize == 0)
        {
                printf("Cart is Empty");
                return;
        }
        printf("Cart items:\n");
        printf("SrNo. - Item \n");
        for (int i = 0; i < cartSize; i++)
        {
                printf("%4d - %s\n", cart[i], items[cart[i]]);
        }
}

void addToCart(int item, int cart[], int *cartSize, char items[][100])
{
        if (*cartSize >= MAX_CART_SIZE)
        {
                printf("Cart is full. Remove an item before adding more.\n");
                return;
        }

        cart[*cartSize] = item;
        (*cartSize)++;
        printf("%s added to the cart.\n", items[item]);
}

void removeFromCart(int item, int cart[], int *cartSize, char items[][100])
{
        int found = 0;
        for (int i = 0; i < *cartSize; i++)
        {
                // if (strcmp(cart[items[i]], item) == 0) {
                if (strcmp(items[cart[i]], items[item]) == 0)
                {
                        found = 1;
                        // Shift the remaining items in the cart
                        for (int j = i; j < *cartSize - 1; j++)
                        {
                                cart[j] = cart[j + 1];
                        }
                        (*cartSize)--;
                        break;
                }
        }

        if (found)
        {
                printf("%s removed from the cart.\n", items[item]);
        }
        else
        {
                printf("%s is not in the cart.\n", items[item]);
        }
}

void displayOrderTable(order table[], int row_count, char items[][100])
{
        printf("Order Table:\n");

        for (int i = 0; i < row_count; ++i)
        {
                printf("Row %d:\n", i + 1);
                // printf("Count: %d\n", table[i].count);
                printf("Items: ");
                for (int j = 0; j < table[i].len; ++j)
                {
                        printf("%s ", items[table[i].array[j]]);
                }
                printf("\n-----------------------\n");
        }
}

void displayMiningTable(miningTable miningCounter[], int miningCount, itemsTable itemsCounter[], char items[][100])
{
        printf("Mining Table:\n");
        for (int i = 0; i <= miningCount; ++i)
        {
                int item = miningCounter[i].item;
                printf("Item: %s\n", itemsCounter[item].item);
                printf("Count: %d\n", miningCounter[i].count);
                // printf("Reference: %p\n", miningCounter[i].reference);
                // printf("Last Sibling: %s\n", items[miningCounter[i].last_sibling->child->item]);
                printf("-----------------------\n");
        }
}

void implementaionMenu(itemsTable itemsCounter[], order table[], miningTable miningCounter[], int item_count, int row_count, char items[][100], Map *pairs)
{
        while (1)
        {
                int option;
                printf("\nData Structures Used:\n");
                printf("1. Display FP Tree\n");
                printf("2. Display Order Table\n");
                printf("3. Display Mining Table\n");
                printf("4. Frequent Itemsets\n");
                printf("5. Recommendation Map\n");
                printf("0. Exit\n");
                printf("Enter your option: ");
                scanf("%d", &option);

                switch (option)
                {
                case 1:
                        printf("Displaying FP Tree:\n");
                        print_tree(itemsCounter, table, item_count, items);
                        break;
                case 2:
                        printf("Displaying Order Table:\n");
                        displayOrderTable(table, row_count, items);
                        break;
                case 3:
                        printf("Displaying Mining Table:\n");
                        displayMiningTable(miningCounter, item_count, itemsCounter, items);
                        break;
                case 4:
                        printf("Displaying Frequent Itemsets:\n");
                        int prefix[10000];
                        prefix[0] = -1;
                        int last_index = 1000;
                        mineFPtree(itemsCounter, miningCounter, last_index, table, row_count, prefix, items, pairs, 1);
                        break;
                case 5:
                        printf("Displaying Recommendation Map:\n");
                        for (int i = 0; i < 100; i++)
                        {
                                char *rec = get(pairs, i);
                                if (rec != NULL)
                                {
                                        printf("%s =>  %s  \n", items[i], rec);
                                }
                        }
                        break;

                case 0:
                        printf("\n");
                        return;
                default:
                        printf("Invalid option\n");
                        break;
                }
        }
}

int processFile(FILE *stream, itemsTable itemsCounter[], int *last_index)
{

        // Processes the file and stores the items in itemsCounter.

        char ch;
        char *data_item = (char *)malloc(1000 * sizeof(char));
        int i = 0, j = 0, items_i = 0, flag, seen1 = 0, seen2 = 0, totalTrans = 0;
        char prev_ch = '1';

        while ((ch = fgetc(stream)) != EOF)
        {
                if (isalpha(ch) || ch == ' ' || ch == '/' || ch == '-' || ch == '.')
                {
                        if (seen1 != 0)
                        {
                                data_item[i] = ch;
                                i++;
                                seen2 = 1;
                        }
                }
                else if (ch == ',' || ch == '\n')
                {
                        if (isdigit(prev_ch))
                        {
                                totalTrans++;
                        }
                        if (seen1 == 0)
                        {
                                seen1 = 1;
                        }
                        else if (seen2 == 1)
                        {
                                seen2 = 0;
                                data_item[i] = '\0';

                                flag = 0;
                                for (j = 0; j <= *last_index && flag == 0; j++)
                                {
                                        if (strcmp(itemsCounter[j].item, data_item) == 0)
                                        {
                                                itemsCounter[j].count++;
                                                flag = 1;
                                        }
                                }
                                if (flag == 0)
                                {
                                        (*last_index)++;
                                        initialize_items_table(&itemsCounter[*last_index]);
                                        itemsCounter[*last_index].item = strdup(data_item);
                                        itemsCounter[*last_index].count = 1;
                                }

                                data_item = NULL;
                                data_item = (char *)malloc(100 * sizeof(char));
                                i = 0;
                                if (ch == '\n')
                                {
                                        seen1 = 0;
                                }
                        }
                }
                prev_ch = ch;
        }

        return totalTrans;
}
