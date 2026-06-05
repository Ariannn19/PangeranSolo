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
    char buffer[100];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        buffer[strcspn(buffer, "\r")] = '\0';
        
        // Buat node baru
        Line *newnode = (Line*)malloc(sizeof(Line));
        strncpy(newnode->info, buffer, MAX_KOLOM - 1);
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

Line* salin_kertas(Line* head) {
    Line* head_baru = NULL;
    Line* tail_baru = NULL;
    Line* current = head;

    while (current != NULL) {
        Line* newnode = (Line*)malloc(sizeof(Line));
        strcpy(newnode->info, current->info);
        newnode->len = current->len;
        newnode->next = NULL;
        newnode->prev = tail_baru;

        if (head_baru == NULL) {
            head_baru = newnode;
        } else {
            tail_baru->next = newnode;
        }
        tail_baru = newnode;
        current = current->next;
    }
    return head_baru;
}

void push_riwayat(RiwayatNode **current_state, Line *kertas_sekarang) {
    RiwayatNode *riwayat_baru = (RiwayatNode*)malloc(sizeof(RiwayatNode));
    
    riwayat_baru->kertas_head = salin_kertas(kertas_sekarang);
    riwayat_baru->prev = *current_state;
    riwayat_baru->next = NULL;

    if (*current_state != NULL) {
        (*current_state)->next = riwayat_baru;
    }

    *current_state = riwayat_baru;
}

Line* undo(RiwayatNode **current_state) {
    if (*current_state != NULL && (*current_state)->prev != NULL) {
        *current_state = (*current_state)->prev;
    }
    return salin_kertas((*current_state)->kertas_head);
}

Line* redo(RiwayatNode **current_state) {
    if (*current_state != NULL && (*current_state)->next != NULL) {
        *current_state = (*current_state)->next;
    }
    return salin_kertas((*current_state)->kertas_head);
}