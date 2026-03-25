#include <stdio.h>
#include <string.h>
#include "main.h"
#include "kasfa.h"

void find (char text[][max_chr],int linecount){
    char keyword [20];
    int found = 0;
    int i;

    printf("masukan kata yang ingin kamu cari: ");
    fgets(keyword,sizeof(keyword),stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    i = 0;
    while(i < linecount){

        if(strstr(text[i], keyword) != NULL){
            printf("Kata ada di baris ke %d: %s\n",i + 1, keyword);
            found = 1;
        }

        i++;
     }

        if (!found){
            printf("Kata tidak ditemukan\n");
        }
}