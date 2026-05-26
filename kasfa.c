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

int wordcounter(Line *head){
    Line *temp= head;
    int total = 0;

    while(temp != NULL){
        char *kata = temp->info;
        int i = 0;
        while(kata[i] != '\0'){
            if((!isspace(kata[i]) && (i == 0 || isspace(kata[i - 1])))){
                total++;
            }
            i++;
        }
        temp = temp->next;
    }

    return total;
}

int charcounter(Line *head){
    Line *temp = head;
    int total = 0;

    while(temp != NULL){
        char *kata = temp->info;
        int i = 0;
        while(kata[i] != '\0'){
            total++;
            i++;
        }

        //untuk menghitung newline nya
        if(temp->next != NULL){
            total++;
        }

        temp = temp->next;
    }

    return total;
}