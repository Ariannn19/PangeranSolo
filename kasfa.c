#include <stdio.h>
#include <string.h>
#include "kasfa.h"

void find(Line *head,char keyword[]){
    int baris = 1;
    while(head != NULL){
        if(strstr(head->info,keyword) == 0){
            printf("Kata ditemukan pada baris %d: %s\n",baris,head->info);
        }
        head = head->next;
        baris++;
    }

    printf("Find selesai\n");
}