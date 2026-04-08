#ifndef ARIAN_H
#define ARIAN_H

#define MAX_LINES 1000
#define MAX_LENGTH 100

int inputText(char text[MAX_LINES][MAX_LENGTH], int lines);

void showText(char text[MAX_LINES][MAX_LENGTH], int lines);

void showClipboard(char clipboard[MAX_LINES][MAX_LENGTH], int clip_lines);

void selectText(int lines, int *start, int *end);

void copyText(char text[MAX_LINES][MAX_LENGTH], int lines, char clipboard[MAX_LINES][MAX_LENGTH], int *clip_lines);

int cutText(char text[MAX_LINES][MAX_LENGTH], int lines, char clipboard[MAX_LINES][MAX_LENGTH], int *clip_lines);

int pasteText(char text[MAX_LINES][MAX_LENGTH], int lines, char clipboard[MAX_LINES][MAX_LENGTH], int clip_lines);

#endif