#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h> // WAJIB DITAMBAHKAN: Untuk sensor tombol SHIFT
#include "arian.h"
#include "kasfa.h"
char keyword[MAX_KOLOM];

// --- FUNGSI MENU ---
// (Fungsi menuCounter dan menuFind tetap sama persis seperti buatanmu, tidak perlu diubah)
// ...

// --- FUNGSI UTAMA ---
int main() {
    initEditor();
    tampilkanTeks();

    int c;
    while (1) {
        c = _getch();

        // 1. Deteksi Tombol Khusus (Panah & Delete)
        if (c == 0 || c == 224) {
            int next = _getch();
            if (next == 83) {
                delete();       // Delete key
            } else {
                // =========================================================
                // LOGIKA BLOK TEKS (Mendeteksi Shift + Panah)
                // =========================================================
                
                // Cek apakah tombol SHIFT (kiri atau kanan) sedang ditahan fisik oleh user
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    
                    // Jika sebelumnya belum nge-blok (baru pertama kali pencet Shift+Panah)
                    if (kursor.isShift == false) {
                        kursor.isShift = true;                  // Nyalakan mode blok
                        kursor.select = kursor.currentLine;     // Catat gerbong kereta awalnya
                        kursor.startX = kursor.kursorX;         // Catat indeks huruf awalnya
                    }
                } else {
                    // Jika panah ditekan TANPA SHIFT, batalkan mode blok teks
                    kursor.isShift = false;
                    kursor.select = NULL;
                }
                
                // =========================================================
                
                moveCursor(next);  // Biarkan panah bergerak seperti biasa
            }
        }
        else if (c == 8)  backspace();      // Backspace
        else if (c == 13) enter();          // Enter
        else if (c == 27) break;            // ESC keluar
        else if (c == 6){
            prinf("Kata yang mau dicari: ");
            fgets(keyword,sizeof(keyword),stdin);
            find(head,keyword);
        }       // Ctrl+F
        else if (c == 23) wordcounter(head);    // Ctrl+W
        else if (c == 3)  copy();           // Ctrl+C
        else if (c == 22) paste();          // Ctrl+V
        else if (c == 24) cut();            // Ctrl+X
        else if (c >= 32 && c <= 126) {
            
            // UX Teks Editor: Jika user mengetik huruf baru saat teks sedang diblok,
            // biasakan untuk mematikan bloknya agar tampilan warna birunya hilang.
            kursor.isShift = false;
            kursor.select = NULL;
            
            insertChar((char)c);
        }

        tampilkanTeks();
    }

    bersihkanMemori();
    return 0;
}