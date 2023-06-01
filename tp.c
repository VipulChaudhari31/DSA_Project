#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    // char* str[100]={NULL};
    char* str="hello,hii";
    char* token=strtok(str,",");
    printf("%s",token);
    return 0;
}