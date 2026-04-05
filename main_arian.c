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
        
        // Menu sudah ditambahkan opsi "Tampilkan Clipboard"
        printf("1. Input Teks\n");
        printf("2. Tampilkan Teks\n");
        printf("3. Tampilkan Clipboard\n"); // <-- MENU BARU
        printf("4. Copy\n");
        printf("5. Cut\n");
        printf("6. Paste\n");
        printf("7. Keluar\n");
        printf("Pilih: ");
        
        scanf("%d", &pilihan);
        getchar();

        // Sesuaikan angka pada case
        switch (pilihan) {
            case 1: lines = inputText(text, lines); break;
            case 2: showText(text, lines); break;
            case 3: showClipboard(clipboard, clip_lines); break;
            case 4: copyText(text, lines, clipboard, &clip_lines); break;
            case 5: lines = cutText(text, lines, clipboard, &clip_lines); break;
            case 6: lines = pasteText(text, lines, clipboard, clip_lines); break;
            case 7: printf("Keluar...\n"); break;
            default: printf("Pilihan tidak valid!\n");
        }
    } while (pilihan != 7); // Jangan lupa ubah angka ini jadi 7

    return 0;
}