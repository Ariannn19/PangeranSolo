#include <stdio.h>
#include <string.h>
#include "kasfa.h"
char clipboard[MAX_KOLOM];

void find(Line *head,char keyword[]){
    int baris = 1;
    while(head != NULL){
        if(strstr(head->info,keyword) != NULL){
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

int linecounter(Line *head){
    int baris = 0;
    while(head != NULL){
        printf("[%d] '%s'\n", baris+1, head->info);
        baris++;
        head = head->next;
    }

    return baris;
}

void copy(){
    if(kursor.currentLine != NULL){
        strcpy(clipboard,kursor.currentLine->info);
        //printf("Teks berhasil di copy\n");
    }else{
        //printf("Tidak ada teks\n");
    }
}

void paste(){
    if(strlen(clipboard) == 0){
        printf("Clipboard Sedang Kosong\n");
        return;
    }

    Line *baru = buatBaris();
    strcpy(baru->info,clipboard);
    baru->len = strlen(clipboard);

    Line *curr = kursor.currentLine;

    baru->next = curr->next;
    baru->prev = curr;

    if(curr->next != NULL){
        curr->next->prev = baru;
    }

    curr->next = baru;

    if(tail == curr){
        tail = baru;
    }

    kursor.currentLine = baru;
    kursor.kursorX = baru->len;

}

void cut(){
        copy();
        Line *curr = kursor.currentLine;
        Line *x;
        Line *kanan;
        Line *kiri;

        //jika curr hanya satu baris
        if(curr == head && curr == tail){
            curr->info[0] = '\0';
            curr->len = 0;
            kursor.kursorX = curr->len;
            return;
        }

        //jika potong baris palng atas
        if(curr == head){
            x = curr;
            head = x->next;
            kursor.currentLine = head;
            head->prev = NULL ;
            kursor.kursorX = 0;
            free(x);
            return;
        }

        //jika yang dipotong node paling bawah
        if(curr == tail){
            x = curr;
            tail = x->prev;
            kursor.currentLine = tail;
            kursor.kursorX = tail->len;
            tail->next = NULL;
            free(x);
            return;
        }

        //jika yang dipotong  di tengah tengah
        if(curr != head && curr != tail){
            x = curr;
            kanan = x->next;
            kiri = x->prev;
            kanan->prev = kiri;
            kiri->next= kanan;
            kursor.currentLine = kanan;
            kursor.kursorX = 0;
            free(x);
            return;
        }
}