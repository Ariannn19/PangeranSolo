#include <stdio.h>
#include <string.h>
#include "rasya.h"

void save_ke_file(char filename[], char kertas [100][100], int jumlah_baris) 
{
    FILE *file = fopen(filename, "w"); // Membuka file untuk ditulis

    if (file == NULL)   
    {
        printf("Gagal membuka file untuk disimpan.\n");
        return;
    }

    for(int i = 0; i < jumlah_baris; i++) {
        fprintf(file, "%s\n", kertas[i]); 
    }

    fclose(file);
}

int load_dari_file(char filename[], char kertas [100][100]) 
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("File %s tidak ditemukan.\n", filename);
        return 0;
    }

    char buffer[100];
    int jumlah_baris = 0;

    while(fgets(buffer, sizeof(buffer), file) != NULL && jumlah_baris < 100) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Menghapus newline
        strncpy(kertas[jumlah_baris], buffer, 99);
        jumlah_baris++;
    }

    fclose(file);
    printf("Berhasil memuat %d baris dari file %s.\n", jumlah_baris, filename);
    return jumlah_baris;
}


void init_stack(stack *s) { 
    s->top = -1; 
    s->max_top = -1; 
}

int is_empty(stack *s) { 
    if (s->top == -1) {
        return 1; 
    } else {
        return 0; 
    }
}

void push(stack *s, char kertas_sekarang[100][100], int jumlah_baris) { 
    if (s->top >= 99) { 
        printf("Riwayat penuh.\n"); 
        return;
    }
    s->top++; 
    s->max_top = s->top; 
    s->riwayat_baris[s->top] = jumlah_baris;

    for (int i = 0; i < jumlah_baris; i++) { 
        strcpy(s->riwayat_kertas[s->top][i], kertas_sekarang[i]);
    }
}

void pop(stack *s, char kertas_tujuan[100][100], int *jumlah_baris_tujuan) { 
    if (is_empty(s) == 1) {
        printf("Tidak ada riwayat untuk di-undo.\n");
        return;
    }

    s->top--; 

    if(s->top == -1) {
        *jumlah_baris_tujuan = 0; 
        return;
    }

    *jumlah_baris_tujuan = s->riwayat_baris[s->top];
    for (int i = 0; i < *jumlah_baris_tujuan; i++) {
        strcpy(kertas_tujuan[i], s->riwayat_kertas[s->top][i]);
    }
}

void redo(stack *s, char kertas_tujuan[100][100], int *jumlah_baris_tujuan) { 
    if (s->top >= s->max_top) {  
        printf("Tidak ada riwayat untuk di-redo.\n");
        return;
    }
        
    s->top++; 

     *jumlah_baris_tujuan = s->riwayat_baris[s->top]; 
     
    for (int i = 0; i < *jumlah_baris_tujuan; i++) { 
        strcpy(kertas_tujuan[i], s->riwayat_kertas[s->top][i]);
    }
}