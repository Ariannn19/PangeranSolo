#include "arian.h"

int cursorX = 0;
int cursorY = 0;
int lines = 1;
char text[MAX_LINES][MAX_LENGTH] = {0};

void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void render() {
    system("cls");

    for (int i = 0; i < lines; i++) {
        gotoxy(0, i);
        printf("%s", text[i]);
    }

    if (cursorY >= lines) cursorY = lines - 1;
    if (cursorY < 0) cursorY = 0;

    int len = strlen(text[cursorY]);
    if (cursorX > len) cursorX = len;
    if (cursorX < 0) cursorX = 0;

    gotoxy(cursorX, cursorY);
}

void moveCursor(int key) {
    int len = strlen(text[cursorY]);
    int newLen;

    if (key == 72) { // UP
        if (cursorY > 0) {
            cursorY--;
            newLen = strlen(text[cursorY]);
            if (cursorX > newLen) cursorX = newLen;
        }
    }
    else if (key == 80) { // DOWN
        if (cursorY < lines - 1) {
            cursorY++;
            newLen = strlen(text[cursorY]);
            if (cursorX > newLen) cursorX = newLen;
        }
    }
    else if (key == 75) { // LEFT
        if (cursorX > 0) {
            cursorX--;
        }
        else if (cursorY > 0) {
            cursorY--;
            cursorX = strlen(text[cursorY]);
        }
    }
    else if (key == 77) { // RIGHT
        if (cursorX < len) {
            cursorX++;
        }
        else if (cursorY < lines - 1) {
            cursorY++;
            cursorX = 0;
        }
    }
}

void insertChar(char ch) {
    int len = strlen(text[cursorY]);

    // Pastikan cursor valid
    if (cursorX > len) cursorX = len;

    // Input saat masih ada ruang
    if (len < MAX_LENGTH - 1) {
        for (int i = len; i >= cursorX; i--) {
            text[cursorY][i + 1] = text[cursorY][i];
        }
        text[cursorY][cursorX] = ch;
        cursorX++;
        return;
    }

    // 
    char currentChar = ch;
    int currentY = cursorY;
    int insertX = cursorX;

    while (currentY < MAX_LINES - 1) {
        if (currentY >= lines) {
            lines++;
        }

        int currentLen = strlen(text[currentY]);

        // Kalau baris ini masih ada ruang
        if (currentLen < MAX_LENGTH - 1) {
            for (int i = currentLen; i >= insertX; i--) {
                text[currentY][i + 1] = text[currentY][i];
            }
            text[currentY][insertX] = currentChar;
            break;
        }

        // Baris penuh  ambil karakter paling kanan
        char overflowChar = text[currentY][MAX_LENGTH - 2];

        // Geser ke kanan 
        for (int i = MAX_LENGTH - 2; i > insertX; i--) {
            text[currentY][i] = text[currentY][i - 1];
        }

        text[currentY][insertX] = currentChar;
        text[currentY][MAX_LENGTH - 1] = '\0';

        // lanjut ke bawah
        currentY++;
        currentChar = overflowChar;
        insertX = 0;
    }

    // Update cursor
    if (cursorX < MAX_LENGTH - 1) {
        cursorX++;
    } else {
        cursorX = 0;
        if (cursorY < lines - 1) cursorY++;
    }
}


void deleteChar() {
    int len = strlen(text[cursorY]);

    if (cursorX < len) {
        // Hapus karakter di posisi kursor (geser sisa teks ke kiri)
        for (int i = cursorX; i < len; i++) {
            text[cursorY][i] = text[cursorY][i + 1];
        }
    }
    else if (cursorY < lines - 1) {
        // Kursor ada di ujung teks: Tarik teks dari baris bawah ke atas
        int currLen = len;
        int nextLen = strlen(text[cursorY + 1]);
        int spaceLeft = MAX_LENGTH - 1 - currLen;

        int copyCount = (nextLen > spaceLeft) ? spaceLeft : nextLen;

        // Salin teks yang muat dari baris bawah ke ujung baris saat ini
        if (copyCount > 0) {
            strncpy(&text[cursorY][currLen], text[cursorY + 1], copyCount);
            text[cursorY][currLen + copyCount] = '\0';
        }

        // Jika baris bawah masih punya sisa, geser sisanya ke kiri
        if (nextLen > copyCount) {
            for (int i = 0; i < nextLen - copyCount; i++) {
                text[cursorY + 1][i] = text[cursorY + 1][copyCount + i];
            }
            text[cursorY + 1][nextLen - copyCount] = '\0';
        } else {
            // Baris bawah habis ditarik: BULDOSER baris-baris di bawahnya ke atas
            for (int i = cursorY + 1; i < lines - 1; i++)
            strcpy(text[i], text[i + 1]);
            text[lines - 1][0] = '\0';
            lines--;
        }
    }
}

void backspace() {
    // Skenario 1: Kursor ada di tengah atau akhir baris
    if (cursorX > 0) {
        cursorX--;       // Mundur satu langkah ke kiri
        deleteChar();    // Geser karakter di posisi baru tersebut
    }
    // Skenario 2: Kursor ada di paling kiri (indeks 0) dan bukan baris paling atas
    else if (cursorY > 0) { 
        cursorX = strlen(text[cursorY - 1]); // Loncat ke ujung teks di baris atasnya
        cursorY--;                           // Naik ke baris atasnya
        deleteChar();                        // geser baris bawah untuk digabung ke atas
    }
}

void enterKey() {
    if (lines >= MAX_LINES) return;

    char temp[MAX_LENGTH];
    strcpy(temp, &text[cursorY][cursorX]);
    text[cursorY][cursorX] = '\0';

    for (int i = lines; i > cursorY; i--) {
        strcpy(text[i], text[i - 1]);
    }
    strcpy(text[cursorY + 1], temp);
    lines++;
    cursorY++;
    cursorX = 0;
}