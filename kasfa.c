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

void wordcounter (char text[][max_chr], int linecount){
    int totalchr = 0;
    int totalkata = 0;

    int i = 0;
    while(i < linecount){
        int tung = strlen(text[i]);
        totalchr = totalchr + tung;

        int j = 0;
        while( j < tung){
            if((j == 0 && text[i][j] != ' ') || (j > 0 && text[i][j] != ' ' && text [i][j - 1] == ' ')){
                totalkata = totalkata + 1;
            }
            j = j + 1;
        }
        i = i + 1;
    }

    printf("\nJumlah baris: %d", linecount);
    printf("\nJumlah kata: %d", totalkata);
    printf("\nJumlah karakter: %d", totalchr);
}