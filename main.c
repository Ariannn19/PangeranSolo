// ... (bagian atas main dan fungsi menu biarkan sama persis)

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
        
        // --- TAMBAHKAN FITURMU DI SINI ---
        else if (c == 3)  copy();           // Ctrl+C untuk Copy
        else if (c == 22) paste();          // Ctrl+V untuk Paste
        // ---------------------------------

        else if (c >= 32 && c <= 126) {
            insertChar((char)c);
        }

        // Setelah aksi apapun selesai, layar digambar ulang
        tampilkanTeks();
    }

    bersihkanMemori();
    return 0;
}