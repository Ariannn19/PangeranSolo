#ifndef ARIAN_H
#define ARIAN_H

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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
    int offsetY;
} Kursor;

typedef struct {
    Line* startLine;
    Line* endLine;
    int startPos;
    int endPos;
} blockArea;

extern Line* head;
extern Line* tail;
extern Kursor kursor;

Line* buatBaris();
void initEditor();
void moveCursor(int key);
void insertChar(char c);
void wrapChar(Line* curr, char overflowChar);
void bersihkanMemori();
void backspace();
void delete();
void gabungBaris(Line* atas, Line* bawah);
void enter();
blockArea panjangBlock();
void tampilkanTeks();
void gotoxy(int x, int y);
void hideCursor();
void showCursor();

#endif