#include <stdio.h>
#include <string.h>
#include "arian.h"

// --- FUNGSI INPUT & SHOW TETAP SAMA ---
int inputText(char text[MAX_LINES][MAX_LENGTH], int lines) {
    printf("Masukkan teks (ketik 'EXIT' di baris baru untuk selesai):\n");
    while (lines < MAX_LINES) {
        printf("%d: ", lines + 1);
        if (fgets(text[lines], MAX_LENGTH, stdin) == NULL) break;
        text[lines][strcspn(text[lines], "\n")] = '\0';
        if (strcmp(text[lines], "EXIT") == 0) break;
        lines++;
    }
    return lines;
}

void showText(char text[MAX_LINES][MAX_LENGTH], int lines) {
    printf("\n---------- ISI DOKUMEN ----------\n");
    if (lines == 0) {
        printf("(Kosong)\n");
    } else {
        for (int i = 0; i < lines; i++) {
            printf("[%02d] %s\n", i + 1, text[i]);
        }
    }
    printf("---------------------------------\n");
}

// FUNGSI TAMPILKAN CLIPBOARD (Tambahkan kode ini)
void showClipboard(char clipboard[MAX_LINES][MAX_LENGTH], int clip_lines) {
    printf("\n-------- ISI CLIPBOARD --------\n");
    if (clip_lines == 0) {
        printf("(Kosong)\n");
    } else {
        for (int i = 0; i < clip_lines; i++) {
            printf("[%02d] %s\n", i + 1, clipboard[i]);
        }
    }
    printf("-------------------------------\n");
}

// --- FUNGSI SELECT SEBAGAI PEMBANTU ---
void selectText(int lines, int *start, int *end) {
    if (lines == 0) {
        printf("Teks kosong, tidak ada yang bisa dipilih.\n");
        *start = 0; *end = 0;
        return;
    }
    printf("Pilih Baris Mulai (1-%d): ", lines);
    scanf("%d", start);
    printf("Pilih Baris Selesai (%d-%d): ", *start, lines);
    scanf("%d", end);
    getchar(); // Bersihkan buffer enter

    if (*start < 1 || *end > lines || *start > *end) {
        printf("Seleksi tidak valid!\n");
        *start = 0; *end = 0; // Reset jika error
    }
}

// --- FUNGSI COPY ---
void copyText(char text[MAX_LINES][MAX_LENGTH], int lines, 
              char clipboard[MAX_LINES][MAX_LENGTH], int *clip_lines) {
    int start, end;
    
    // Panggil fungsi select di sini!
    selectText(lines, &start, &end);

    if (start == 0 || end == 0) {
        printf("Gagal melakukan Copy.\n");
        return; // Batal jika seleksi salah/kosong
    }

    *clip_lines = 0;
    for (int i = start - 1; i < end; i++) {
        strcpy(clipboard[*clip_lines], text[i]);
        (*clip_lines)++;
    }
    printf("%d baris berhasil di-copy ke clipboard.\n", *clip_lines);
}

// --- FUNGSI CUT ---
int cutText(char text[MAX_LINES][MAX_LENGTH], int lines, 
            char clipboard[MAX_LINES][MAX_LENGTH], int *clip_lines) {
    int start, end;
    
    // Panggil fungsi select di sini!
    selectText(lines, &start, &end);

    if (start == 0 || end == 0) {
        printf("Gagal melakukan Cut.\n");
        return lines; // Batal jika seleksi salah/kosong
    }
    
    // 1. Simpan ke clipboard
    *clip_lines = 0;
    for (int i = start - 1; i < end; i++) {
        strcpy(clipboard[*clip_lines], text[i]);
        (*clip_lines)++;
    }

    // 2. Hapus baris dengan menggeser array ke atas
    int jumlah_hapus = end - start + 1;
    for (int i = end; i < lines; i++) {
        strcpy(text[i - jumlah_hapus], text[i]);
    }
    printf("%d baris berhasil di-cut.\n", jumlah_hapus);
    return lines - jumlah_hapus;
}

// --- FUNGSI PASTE
int pasteText(char text[MAX_LINES][MAX_LENGTH], int lines, 
              char clipboard[MAX_LINES][MAX_LENGTH], int clip_lines) {
    
    if (clip_lines == 0) {
        printf("Clipboard kosong!\n");
        return lines;
    }

    int pos, index;
    
    // 1. Tentukan baris target
    printf("Paste di baris ke berapa (1-%d)? ", lines);
    scanf("%d", &pos);
    getchar(); // Bersihkan enter

    if (pos < 1 || pos > lines) {
        printf("Posisi baris tidak valid!\n");
        return lines;
    }

    // 2. Tampilkan teks dan tentukan indeks pemotongan
    int len = strlen(text[pos - 1]);
    printf("Isi baris [%d]: %s\n", pos, text[pos - 1]);
    printf("Paste mulai di huruf ke berapa (0 - %d)? ", len);
    scanf("%d", &index);
    getchar(); // Bersihkan enter

    // Jika user iseng memasukkan angka minus atau kelebihan
    if (index < 0) index = 0;
    if (index > len) index = len;

    // 3. Gabungkan seluruh isi clipboard dengan pemisah spasi
    // Kita siapkan penampung besar (5000) agar tidak error saat digabung
    char combined_clip[5000] = ""; 
    for (int i = 0; i < clip_lines; i++) {
        strcat(combined_clip, clipboard[i]);
        if (i < clip_lines - 1) {
            strcat(combined_clip, " "); // Tambahkan spasi antar baris clipboard
        }
    }

    // 4. Rakit teks baru (Bagian Kiri + Clipboard Gabungan + Bagian Kanan)
    char temp_line[6000] = "";
    strncpy(temp_line, text[pos - 1], index); // Ambil potongan kiri dari teks asli
    temp_line[index] = '\0';                  // Beri batas akhir
    strcat(temp_line, combined_clip);         // Tempel isi clipboard di tengah
    strcat(temp_line, text[pos - 1] + index); // Sambung dengan potongan kanan

    // 5. Cek batas maksimal (MAX_LENGTH)
    int total_len = strlen(temp_line);
    int max_chars_per_line = MAX_LENGTH - 1; // Sisakan 1 untuk karakter '\0'

    if (total_len <= max_chars_per_line) {
        // Jika masih muat 1 baris, langsung timpa baris aslinya
        strcpy(text[pos - 1], temp_line);
    } else {
        // Jika kepanjangan, kita harus pecah ke baris baru
        int needed_lines = total_len / max_chars_per_line;
        if (total_len % max_chars_per_line != 0) needed_lines++; 
        
        int tambahan = needed_lines - 1;

        // Pastikan dokumen tidak kepenuhan jika barisnya bertambah
        if (lines + tambahan > MAX_LINES) {
            printf("Gagal: Kapasitas dokumen tidak cukup menampung teks tumpahan!\n");
            return lines;
        }

        // Geser sisa dokumen ke bawah untuk memberi ruang tumpahan
        for (int i = lines - 1; i >= pos; i--) {
            strcpy(text[i + tambahan], text[i]);
        }
        lines += tambahan; // Perbarui total baris dokumen

        // Potong-potong temp_line sesuai MAX_LENGTH dan masukkan ke barisnya
        for (int i = 0; i < needed_lines; i++) {
            strncpy(text[pos - 1 + i], temp_line + (i * max_chars_per_line), max_chars_per_line);
            text[pos - 1 + i][max_chars_per_line] = '\0';
        }
        
        printf("Teks melampaui batas dan otomatis tumpah ke baris baru.\n");
    }

    printf("Paste di tengah kalimat berhasil!\n");
    return lines;
}