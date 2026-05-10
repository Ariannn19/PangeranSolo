#ifndef RASYA_H
#define RASYA_H

typedef struct {
    char riwayat_kertas[100][100][100];
    int riwayat_baris[100];
    int top;
    int max_top;
} stack;

void save_ke_file(char filename[], char kertas [100][100], int jumlah_baris);
int load_dari_file(char filename[], char kertas [100][100]);

void init_stack(stack *s);
void push(stack *s, char kertas_sekarang[100][100], int jumlah_baris);
void pop(stack *s, char kertas_tujuan[100][100], int *jumlah_baris_tujuan);
void redo(stack *s, char kertas_tujuan[100][100], int *jumlah_baris_tujuan);
int is_empty(stack *s);

#endif