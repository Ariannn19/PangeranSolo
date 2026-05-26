#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rasya.h"

void save_ke_file(char filename[], Line *head) {
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Gagal membuka file untuk disimpan.\n");
        return;
    }
    
    Line *current = head;
    while (current != NULL) {
        fprintf(file, "%s\n", current->info);
        current = current->next;
    }
    
    fclose(file);
}

Line* load_dari_file(char filename[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return NULL;

    Line *head = NULL;
    Line *tail = NULL;
    char buffer[MAX_KOLOM];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        
        // Buat node baru
        Line *newnode = (Line*)malloc(sizeof(Line));
        strcpy(newnode->info, buffer);
        newnode->len = strlen(buffer);
        newnode->next = NULL;
        newnode->prev = tail;

        // Sambungkan node
        if (head == NULL) {
            head = newnode;
        } else {
            tail->next = newnode;
        }
        tail = newnode;
    }
    
    fclose(file);
    return head;
}

void push_riwayat(RiwayatNode **current_state, Line *kertas_sekarang) {
    
    Line *head_baru = NULL;
    Line *tail_baru = NULL;
    Line *current = kertas_sekarang;
    
    while (current != NULL) {
        // Buat salinan node baru
        Line *newnode = (Line*)malloc(sizeof(Line));
        strcpy(newnode->info, current->info);
        newnode->len = current->len;
        newnode->next = NULL;
        newnode->prev = tail_baru;
        
        // Sambungkan salinan node
        if (head_baru == NULL) {
            head_baru = newnode;
        } else {
            tail_baru->next = newnode;
        }
        tail_baru = newnode;
        
        // Maju ke baris asli berikutnya
        current = current->next;
    }

    RiwayatNode *riwayat_baru = (RiwayatNode*)malloc(sizeof(RiwayatNode));
    riwayat_baru->kertas_head = head_baru; // Masukkan hasil fotokopian
    riwayat_baru->prev = *current_state;
    riwayat_baru->next = NULL;

    // Putus rantai masa depan jika ada
    if (*current_state != NULL) {
        (*current_state)->next = riwayat_baru;
    }
    
    // Pindahkan kursor waktu ke riwayat terbaru
    *current_state = riwayat_baru;
}

Line* undo(RiwayatNode **current_state) {
    if (*current_state != NULL && (*current_state)->prev != NULL) {
        *current_state = (*current_state)->prev;
    }
    return (*current_state)->kertas_head;
}

Line* redo(RiwayatNode **current_state) {
    if (*current_state != NULL && (*current_state)->next != NULL) {
        *current_state = (*current_state)->next;
    }
    return (*current_state)->kertas_head;
}