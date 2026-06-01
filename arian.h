#ifndef ARIAN_H
#define ARIAN_H

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // Tambahan dari Arian untuk tipe data bool
#include <windows.h>

#define MAX_KOLOM 80

typedef struct Line {
    char info[MAX_KOLOM];
    int len;
    struct Line* next;
    struct Line* prev;
} Line;

typedef struct {
    Line* currentLine;
    int kursorX;
    Line* select;
    int startX;
    bool isShift;
} Kursor;

// Struct baru buatan Arian untuk menyimpan info blok teks
typedef struct {
    Line* startLine;
    Line* endLine;
    int startPos;
    int endPos;
} blockArea;

// WAJIB pakai extern agar tidak multiple definition error
extern Line* head;
extern Line* tail;
extern Kursor kursor;

// Semua prototipe fungsi (gabungan Arian versi lama dan baru)
Line* buatBaris();
void initEditor();
void moveCursor(int key);
void insertChar(char c);
void wrapChar(Line* curr, char overflowChar); // Fitur baru Arian
void backspace();
void delete();
void gabungBaris(Line* atas, Line* bawah);    // Fitur baru Arian
void enter();
blockArea panjangBlock();                     // Fitur baru Arian
void tampilkanTeks();
void bersihkanMemori();
void gotoxy(int x, int y);
void hideCursor();
void showCursor();

#endif