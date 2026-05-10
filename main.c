#include "kasfa.h"
#include "arian.h"
#include "rasya.h"

stack riwayat; // Mendeklarasikan stack untuk menyimpan riwayat (undo/redo)

int main() {
    char pilihan; 
    char nama_file[50];
    
    // Memastikan jumlah baris minimal adalah 1 saat aplikasi pertama kali dijalankan
    if(lines == 0) lines = 1; 

    // Loop utama aplikasi (Menu)
    while(1) {
        system("cls"); // Membersihkan layar konsol
        
        // Menampilkan Menu Utama
        printf("\n"
               "  =======================================\n"
               "  ||                                   ||\n"
               "  ||     PANGERAN SOLO TEXT EDITOR     ||\n"
               "  ||                                   ||\n"
               "  =======================================\n"
               "  ||                                   ||\n"
               "  ||  [1] New File                     ||\n"
               "  ||  [2] Open File                    ||\n"
               "  ||  [3] Help & Shortcuts             ||\n"
               "  ||  [4] Exit                         ||\n"
               "  ||                                   ||\n"
               "  =======================================\n"
               "  Pilihan Anda (1-4): ");
        
        pilihan = _getch(); // Membaca input pengguna 

        // Jika user memilih New File (1) atau Open File (2)
        if (pilihan == '1' || pilihan == '2') {
            
            if (pilihan == '1') {
                // Persiapan membuat file baru: reset kursor, baris, dan kosongkan teks
                cursorX = 0; cursorY = 0; lines = 1;
                text[0][0] = '\0'; 
            } 
            else {
                // Membuka file yang sudah ada
                system("cls");
                printf("\nMasukkan nama file untuk dibuka: ");
                scanf("%s", nama_file); getchar(); // Membersihkan buffer enter setelah scanf

                // Memanggil fungsi untuk memuat isi file ke array 'text'
                int loadedLines = load_dari_file(nama_file, text);
                if (loadedLines > 0) {
                    lines = loadedLines; // Update total baris sesuai isi file
                    cursorX = 0; cursorY = 0;
                } else {
                    printf("\nGagal memuat file. Tekan tombol apa saja untuk kembali...");
                    _getch();
                    continue; // Kembali ke menu utama jika file gagal dimuat
                }
            }

            // Inisialisasi stack untuk Undo/Redo dan simpan status kanvas pertama kali
            init_stack(&riwayat);
            push(&riwayat, text, lines); 

            // Menampilkan Splash Screen sebelum masuk ke layar editor utama
            system("cls");
            printf("\n"
                   "  ============================================================\n"
                   "  ||                  MEMASUKI MODE EDITOR                  ||\n"
                   "  ============================================================\n"
                   "  || PETUNJUK SHORTCUT:                                     ||\n"
                   "  || - [ESC]    : Keluar ke Menu Utama & Simpan Riwayat     ||\n"
                   "  || - [Ctrl+S] : Simpan File (Save)                        ||\n"
                   "  || - [Ctrl+Z] : Batal (Undo)                              ||\n"
                   "  || - [Ctrl+Y] : Ulangi (Redo)                             ||\n"
                   "  || - [Ctrl+F] : Cari Kata (Find)                          ||\n"
                   "  || - [Ctrl+W] : Hitung Kata & Karakter (Word Counter)     ||\n"
                   "  || - [Ctrl+R] : Tampilan Wrap Text                        ||\n"
                   "  ============================================================\n"
                   "  Tekan tombol apa saja untuk mulai mengetik...");
            _getch(); 

            render(); // Menampilkan isi teks ke layar untuk pertama kalinya
            
            // Loop menangkap setiap ketikan user
            while(1) {
                int ch = _getch(); // Menangkap kode ASCII tombol yang ditekan
                
                // Menangani tombol panah (Arrow Keys memiliki 2 byte kode, diawali 224 atau 0)
                if (ch == 224 || ch == 0) { 
                    ch = _getch();  
                    moveCursor(ch); 
                }
                else if (ch == 27) { // 27 kode ASCII  tombol ESC
                    push(&riwayat, text, lines); // Simpan state terakhir sebelum keluar
                    break; // Keluar dari loop, kembali ke Menu Utama
                }
                else if (ch == 26) { // 26 ASCII Ctrl+Z 
                    pop(&riwayat, text, &lines);
                    cursorX = 0; cursorY = 0; // Kembalikan kursor ke awal setelah undo
                }
                else if (ch == 25) { // 25 ASCII Ctrl+Y
                    redo(&riwayat, text, &lines);
                    cursorX = 0; cursorY = 0; 
                }
                else if (ch == 19) { // 19 ASCII Ctrl+S 
                    system("cls");
                    printf("\n--- MODE SAVE ---\nMasukkan nama file (Tambahkan ekstensi .txt): ");
                    scanf("%s", nama_file); getchar(); 
                    save_ke_file(nama_file, text, lines); // Simpan array teks ke file eksternal
                    printf("File %s berhasil disimpan! Tekan apa saja untuk lanjut...", nama_file);
                    _getch();
                }
                // Mengelompokkan fitur shortcut tambahan (Ctrl+F, Ctrl+W, Ctrl+R)
                else if (ch == 6 || ch == 23 || ch == 18) { 
                    system("cls");
                    if (ch == 6) {        // 6 ASCII Ctrl+F
                        printf("\n--- MODE CARI KATA ---\n"); find(text, lines);
                    } else if (ch == 23) { // 23 ASCII Ctrl+W
                        printf("\n--- ANALISIS DOKUMEN ---\n"); wordcounter(text, lines);
                    } else if (ch == 18) { // 18 ASCII Ctrl+R
                        printf("\n--- TAMPILAN WRAP TEXT ---\n"); wraptext(text, lines);
                    }
                    printf("\nTekan tombol apa saja untuk kembali ke editor...");
                    _getch();
                }
                else if (ch == '\r') { // Tombol Enter
                    enterKey(); // Buat baris baru
                    push(&riwayat, text, lines); // Simpan riwayat tiap kali ganti baris
                }
                else if (ch == '\b') { // Tombol Backspace
                    backspace(); 
                }
                // Menangkap karakter yang bisa diketik (huruf, angka, simbol)
                else if (ch >= 32 && ch <= 126) { 
                    insertChar((char)ch); // Masukkan karakter ke dalam teks
                }
                
                render(); // refresh layar setiap kali ada perubahan/input
            }
        } 
        // Jika user memilih menu Help (3)
        else if (pilihan == '3') {
            system("cls");
            printf("\n  === PANDUAN PENGGUNAAN ===\n"
                   "  Navigasi:\n"
                   "  - [Panah] Pindah Kursor\n"
                   "  - [ESC]   Keluar dari Editor ke Menu Utama\n\n"
                   "  Shortcut:\n"
                   "  - [Ctrl+S] Save File\n"
                   "  - [Ctrl+Z] Undo (Batal)\n"
                   "  - [Ctrl+Y] Redo (Ulangi)\n"
                   "  - [Ctrl+F] Cari Kata (Find)\n"
                   "  - [Ctrl+W] Hitung Kata & Karakter (Word Count)\n"
                   "  - [Ctrl+R] Tampilan Wrap Text\n\n"
                   "  Tekan tombol apa saja untuk kembali...");
            _getch();
        } 
        // Jika user memilih menu Exit (4)
        else if (pilihan == '4') {
            system("cls");
            printf("\n  Terima kasih telah menggunakan Pangeran Solo Text Editor!\n");
            break; // Hentikan loop
        }
    }
    
    return 0;
}