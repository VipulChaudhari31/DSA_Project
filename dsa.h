#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define FALSE 0
#define TRUE 1


// shared_variables.h
extern float minSupport;
extern int SUPPORT;
extern char * nullChar;

typedef struct nodes{   //nodes of fpTree
        int count;      //count of item instances
        int item;       //item name
        struct nodes *parent;   //parent address
        struct nodes *sibling;  //pointer to next instance of the same item
        struct nodes *child;
        struct nodes *level;
}node;

typedef struct itemsTables{
        int count;      //count of item instances
        char *item;     //item name
        node *reference;        //address of the instance in fpTree
        node *last_sibling;
}itemsTable;

typedef struct miningTables{
        int count;
        char item;
        node *reference;
        node *last_sibling;
}miningTable;

struct Ordering{
                int array[100];
                int len;
                int count;
};
typedef struct Ordering order;

void initialize_node(node* temp);
void initialize_items_table(itemsTable* temp);
void initialize_mining_table(miningTable* temp);
void initialize_order(order * temp);
int find_index(int item, miningTable miningCounter[], int miningCount);
void sortDesc(itemsTable itemsCounter[10000], int last_index);
void supportFilter(itemsTable itemsCounter[10000], int* item_count);
void displayItems(itemsTable itemsCounter[], int item_count);
void orderTransaction(char* transaction_data, itemsTable itemsCounter[], order table[], int order_row, int item_count );
int orderTable(itemsTable itemsCounter[10000], order table[10000], int item_count);
void constructBaseFPtree(itemsTable itemsCounter[], order table[], int row_count);
void constructFPtree(miningTable miningCounter[], int miningCount, order table[], int row_count);
void print_tree(itemsTable itemsCounter[], int item_count);
void copyToMiningTable(itemsTable itemsCounter[], miningTable miningCounter[], int item_count);
void mineFPtree(itemsTable itemsCounter[],miningTable old_miningCounter[],int old_miningCount,order old_table[],int old_row_count,int prefix[]);
