#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "arian.h"

int main() {
    int ch;

    system("cls"); 
    
    initEditor();

    while (1) {
        tampilkanTeks();

        ch = _getch();

        if (ch == 27) break; // ESC untuk keluar

        if (ch == 224 || ch == 0) { 
            ch = _getch();

            if (ch == 83) {         
                delete();
            } else {
                moveCursor(ch);     
            }
        }
        else if (ch == 8) {
            backspace();
        }
        else if (ch == 13) {
            enter();
        }
        else if (ch >= 32 && ch <= 126) {
            insertChar((char)ch);
        }
    }

    system("cls");
    printf("Keluar dari editor...\n");
    
    bersihkanMemori();

    return 0;
}