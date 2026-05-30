#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include "arian.h"
#include "kasfa.h"

// --- FUNGSI MENU ---
void menuCounter() {
    for(int i = 22; i <= 29; i++) {
        gotoxy(0, i);
        printf("%-79s", " "); 
    }

    gotoxy(0, 22);
    printf("+=================================+");
    gotoxy(0, 23);
    printf("| [1] Word  [2] Char  [3] Line   |");
    gotoxy(0, 24);
    printf("| [ESC] Batal                    |");
    gotoxy(0, 25);
    printf("+=================================+");
    gotoxy(0, 26);
    printf("Pilih: ");

    int pilih = _getch();

    gotoxy(0, 27);
    switch (pilih) {
        case '1':
            printf("Total kata  : %d", wordcounter(head));
            break;
        case '2':
            printf("Total char  : %d", charcounter(head));
            break;
        case '3':
            printf("Total baris : %d", linecounter(head));
            break;
        default:
            tampilkanTeks();
            return;
    }

    gotoxy(0, 28);
    printf("Tekan sembarang tombol...");
    _getch();
    tampilkanTeks();
}

void menuFind() {
    gotoxy(0, 22);
    printf("+=================================+");
    gotoxy(0, 23);
    printf("| FIND                           |");
    gotoxy(0, 24);
    printf("+=================================+");
    gotoxy(0, 25);
    printf("Cari kata: ");

    showCursor();
    char keyword[MAX_KOLOM];
    scanf("%79s", keyword);     
    hideCursor();

    gotoxy(0, 26);
    find(head, keyword);        

    gotoxy(0, 30);
    printf("Tekan sembarang tombol...");
    _getch();
    tampilkanTeks();
}

// --- FUNGSI UTAMA ---
int main() {
    initEditor();
    tampilkanTeks();

    int c;
    while (1) {
        c = _getch();

        if (c == 0 || c == 224) {
            int next = _getch();
            if (next == 83) delete();       // Delete key
            else moveCursor(next);          // Arrow keys
        }
        else if (c == 8)  backspace();      // Backspace
        else if (c == 13) enter();          // Enter
        else if (c == 27) break;            // ESC keluar
        else if (c == 6)  menuFind();       // Ctrl+F
        else if (c == 23) menuCounter();    // Ctrl+W
        else if (c == 3)  copy();           // Ctrl+C (Fitur Kasfa)
        else if (c == 22) paste();          // Ctrl+V (Fitur Kasfa)
        else if (c >= 32 && c <= 126) {
            insertChar((char)c);
        }

        tampilkanTeks();
    }

    bersihkanMemori();
    return 0;
}