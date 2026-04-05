#include <stdio.h>
#include "arian.h"

int main() {
    char text[MAX_LINES][MAX_LENGTH];
    char clipboard[MAX_LINES][MAX_LENGTH];
    int lines = 0, clip_lines = 0;
    int pilihan;

    do {
        printf("\n=== ARIAN TEXT EDITOR ===\n");
        printf("Jumlah baris: %d | Clipboard: %d baris\n", lines, clip_lines);
        
        // Menu lebih ringkas
        printf("1. Input Teks\n");
        printf("2. Tampilkan Teks\n");
        printf("3. Copy\n");
        printf("4. Cut\n");
        printf("5. Paste\n");
        printf("6. Keluar\n");
        printf("Pilih: ");
        
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1: lines = inputText(text, lines); break;
            case 2: showText(text, lines); break;
            case 3: copyText(text, lines, clipboard, &clip_lines); break;
            case 4: lines = cutText(text, lines, clipboard, &clip_lines); break;
            case 5: lines = pasteText(text, lines, clipboard, clip_lines); break;
            case 6: printf("Keluar...\n"); break;
            default: printf("Pilihan tidak valid!\n");
        }
    } while (pilihan != 6);

    return 0;
}