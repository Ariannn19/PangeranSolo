#include "arian.h"

int main() {
    initEditor(); 
    int ch;
    while (1) {
        tampilkanTeks();

        ch = _getch();

        if (ch == 27) {
            break; 
        }

        if (ch == 224 || ch == 0) { 
            ch = _getch(); 

            if (ch == 83) {         
                kursor.isShift = false; 
                kursor.select = NULL;
                delete();
            } else {
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    if (kursor.isShift == false) {
                        kursor.isShift = true;
                        kursor.select = kursor.currentLine;
                        kursor.startX = kursor.kursorX;
                    }
                } else {

                    kursor.isShift = false;
                    kursor.select = NULL;
                }

                moveCursor(ch);     
            }
        }
        else if (ch == 8) {
            kursor.isShift = false; 
            kursor.select = NULL;
            backspace();
        }
        else if (ch == 13) {
            kursor.isShift = false; 
            kursor.select = NULL;
            enter();
        }
        
        else if (ch >= 32 && ch <= 126) {
            kursor.isShift = false;
            kursor.select = NULL;
            insertChar((char)ch);
        }
    }
    
    return 0;
}