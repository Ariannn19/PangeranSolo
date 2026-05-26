#ifndef ARIAN_H
#define ARIAN_H

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
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
} Kursor;

Line* head;
Line* tail;
Kursor kursor;

Line* buatBaris();
void initEditor();
void moveCursor(int key);
void insertChar(char c);
void wrapChar(Line* curr, char overflowChar);
void backspace();
void delete();
void gabungBaris(Line* atas, Line* bawah);
void enter();
void tampilkanTeks();
void bersihkanMemori();
void gotoxy(int x, int y);
void hideCursor();
void showCursor();

#endif


