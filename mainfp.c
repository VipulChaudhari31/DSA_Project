#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "dsa.h"
#define FALSE 0
#define TRUE 1

float minSupport = 0.05;
int SUPPORT = 0;
char *nullChar = "\0";

int main(){

        FILE* stream = fopen("groceries_subset.csv", "r");
        if(stream == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

        char ch;
        char *data_item = (char*)malloc(1000*sizeof(char));
        itemsTable itemsCounter[10000];
        miningTable miningCounter[10000];
        order table[10000];

        initialize_items_table(&itemsCounter[0]);

        int i=0, j=0, items_i=0, last_index=-1, flag, seen1 = 0, seen2 = 0, totalTrans = 0;
        char prev_ch = '1';
    while((ch = fgetc(stream)) != EOF){

        if(isalpha(ch) || ch==' ' ||ch == '/' || ch == '-' || ch == '.'){

                if(seen1!=0){
                        data_item[i]=ch;
                        i++;
                        seen2=1;
                }
        }

        else if(ch==','|| ch=='\n'){
                        if(isdigit(prev_ch)) totalTrans++;
                if(seen1 == 0){
                        seen1 = 1;
                }
                else if(seen2==1){
                                seen2 = 0;
                                data_item[i]='\0';
                                // 
                                flag=0;
                                for(j=0;j<=last_index && flag == 0;j++){
                                        if(strcmp(itemsCounter[j].item,data_item) == 0){
                                                itemsCounter[j].count++;
                                                flag = 1;
                                                // 
                                                break;
                                        }
                                }
                                if(flag==0){
                                        last_index++;
                                        initialize_items_table(&itemsCounter[last_index]);
                                        itemsCounter[last_index].item = data_item;
                                        itemsCounter[last_index].count = 1;
                                }
                                // 
                                data_item = NULL;
                                data_item = (char*)malloc(100*sizeof(char));
                                i=0;
                                if(ch=='\n'){
                                        seen1 = 0;
                                }
                        }
                }
                prev_ch = ch;
    }

        fclose(stream);

    sortDesc(itemsCounter, last_index+1);

        SUPPORT = ceil(totalTrans*minSupport);
    supportFilter(itemsCounter, &last_index);


    int row_count = orderTable(itemsCounter,table, last_index);
        constructBaseFPtree(itemsCounter, table, row_count);
        copyToMiningTable(itemsCounter, miningCounter, last_index);

        int prefix[1000];
        prefix[0] = -1;
        mineFPtree(itemsCounter, miningCounter, last_index, table, row_count, prefix);
        return 0;
}

