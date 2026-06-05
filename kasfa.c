#include <stdio.h>
#include <string.h>
#include "kasfa.h"
char clipboard[MAX_KOLOM];


void find(Line *head,char keyword[]){
    int baris = 1;
    int ketemu = 0;
    while(head != NULL){
        if(strstr(head->info,keyword) != NULL){
            printf("Kata ditemukan pada baris %d: %s\n",baris,head->info);
            ketemu = 1;
        }
        head = head->next;
        baris++;
    }

    if(ketemu !=  1){
        printf("Kata tidak ditemukan :( \n");
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

int linecounter(Line *head){
    int baris = 0;
    while(head != NULL){
        baris++;
        head = head->next;
    }
    return baris;
}

void copy(){
    int i,j = 0;
    if(kursor.isShift == true && kursor.select != NULL){
        blockArea area = panjangBlock();
        Line* temp = area.startLine;
        while(temp != NULL){
            int btsAwal,btsAkhir;

            if(temp == area.startLine){
                btsAwal = area.startPos;
            }else{
                btsAwal = 0;    
            }

            if(temp == area.endLine){
                btsAkhir = area.endPos;
            }else{
                btsAkhir = temp->len;
            }

            i = btsAwal;
            while(i < btsAkhir){
                clipboard[j] = temp->info[i];
                i++;
                j++;
            }

            if(temp == area.endLine){
                break;
            }else{
                clipboard[j] = '\n';
                j++;
                temp = temp->next;
            }
        }
        clipboard[j] = '\0';
    }
    
}

void paste(){
    if(strlen(clipboard) == 0){
        return;
    }

    int batas = strlen(clipboard);
    int i = 0;
    while(i < batas){
        if(clipboard[i] == '\n'){
            enter();
        }else{
            insertChar(clipboard[i]);
        }
        i++;
    }
}

void cut() {
    int i,j;
    Line* hapus;
    if(kursor.isShift == true && kursor.select != NULL){
        blockArea area = panjangBlock();
        copy();

        if(area.startLine == area.endLine){
            int jumlah_potong = area.endPos - area.startPos;
            i = area.endPos;

            while(i <= area.startLine->len){
                area.startLine->info[i-jumlah_potong] = area.startLine->info[i];
                i++;
            }
            area.startLine->len = area.startLine->len - jumlah_potong;
            kursor.currentLine = area.startLine;
            kursor.kursorX = area.startPos;
        }else{
            area.startLine->info[area.startPos] = '\0';
            area.startLine->len = area.startPos;

            i = area.endPos;
            j = 0;
            while(i <= area.endLine->len){
                area.endLine->info[j] = area.endLine->info[i];
                i++;
                j++;
            }
            area.endLine->len = area.endLine->len - area.endPos;

            hapus = area.startLine->next;
            while(hapus != area.endLine){
                Line* nextHps = hapus->next;
                free(hapus);
                hapus = nextHps;
            }

            area.startLine->next = area.endLine;
            area.endLine->prev = area.startLine;

            if(area.startLine->len + area.endLine->len < MAX_KOLOM){
                gabungBaris(area.startLine,area.endLine);
            }

            kursor.currentLine = area.startLine;
            kursor.kursorX =  area.startPos;
        }
        kursor.isShift = false;
        kursor.select = NULL;
    }

}