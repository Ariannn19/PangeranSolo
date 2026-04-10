#include "arian.h"

int cursorX = 0;
int cursorY = 0;
int lines = 1;
char text[MAX_LINES][MAX_LENGTH] = {0};

// Mengatur posisi kursor pada koordinat (x, y) di layar konsol
void gotoxy(int x, int y) {
    COORD c = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Membersihkan layar terminal dan mencetak seluruh teks yang ada di array
void render() {
    system("cls");
    // Mencetak setiap baris teks yang ada
    for (int i = 0; i < lines; i++) 
    {
        gotoxy(0, i);
        printf("%s", text[i]);
    }
    // Mencegah kursor melewati batas atas atau bawah jumlah baris
    if (cursorY >= lines) 
    {
        cursorY = lines - 1;
    }
    if (cursorY < 0) 
    {
        cursorY = 0;
    }
    // Mencegah kursor melewati panjang teks pada baris saat ini
    int len = strlen(text[cursorY]);
    if (cursorX > len) 
    {
        cursorX = len;
    }
    if (cursorX < 0) 
    {
    cursorX = 0;
    }
    // Mengembalikan kursor ke posisi aktif
    gotoxy(cursorX, cursorY);
}

// Memproses pergerakan kursor menggunakan tombol arrow
void moveCursor(int key) {
    int len = strlen(text[cursorY]);
    int newLen;
    if (key == 72) { // Panah Atas
        if (cursorY > 0) {
            cursorY--;
            // Sesuaikan posisi X jika baris di atasnya lebih pendek
            newLen = strlen(text[cursorY]);
            if (cursorX > newLen) cursorX = newLen;
        }
    }
    else if (key == 80) { // Panah Bawah
        if (cursorY < lines - 1) {
            cursorY++;
            // Sesuaikan posisi X jika baris di bawahnya lebih pendek
            newLen = strlen(text[cursorY]);
            if (cursorX > newLen) cursorX = newLen;
        }
    }
    else if (key == 75) { // Panah Kiri
        if (cursorX > 0) {
            cursorX--;
        }
        else if (cursorY > 0) {
            // Pindah ke akhir baris sebelumnya jika sudah berada di ujung kiri
            cursorY--;
            cursorX = strlen(text[cursorY]);
        }
    }
    else if (key == 77) { // Panah Kanan
        if (cursorX < len) {
            cursorX++;
        }
        else if (cursorY < lines - 1) {
            // Pindah ke awal baris selanjutnya jika kursor sudah berada di ujung kanan
            cursorY++;
            cursorX = 0;
        }
    }
}

// Menambahkan karakter baru pada posisi kursor saat ini
void insertChar(char ch) {
    int len = strlen(text[cursorY]);
    // Pastikan posisi kursor valid sebelum menyisipkan karakter
    if (cursorX > len){
        cursorX = len;
    }
    // Kondisi 1: Baris saat ini masih memiliki kapasitas
    if (len < MAX_LENGTH - 1) {
        // Geser sisa teks ke kanan untuk memberikan ruang bagi karakter baru
        for (int i = len; i >= cursorX; i--) {
            text[cursorY][i + 1] = text[cursorY][i];
        }
        text[cursorY][cursorX] = ch;
        cursorX++;
        return;
    }

    // Kondisi 2: Baris penuh, lakukan pergeseran karakter ke baris berikutnya
    char currentChar = ch;
    int currentY = cursorY;
    int insertX = cursorX;
    while (currentY < MAX_LINES - 1) {
        // Tambahkan baris baru jika diperlukan
        if (currentY >= lines) {
            lines++;
        }

        int currentLen = strlen(text[currentY]);

        // Jika baris tujuan memiliki ruang, sisipkan karakter dan hentikan pergeseran
        if (currentLen < MAX_LENGTH - 1) {
            for (int i = currentLen; i >= insertX; i--) {
                text[currentY][i + 1] = text[currentY][i];
            }
            text[currentY][insertX] = currentChar;
            break;
        }
        // Simpan karakter paling kanan yang akan terdorong ke bawah
        char overflowChar = text[currentY][MAX_LENGTH - 2];

        // Geser teks ke kanan untuk Menambahkan karakter saat ini
        for (int i = MAX_LENGTH - 2; i > insertX; i--) 
        {
            text[currentY][i] = text[currentY][i - 1];
        }
        text[currentY][insertX] = currentChar;
        // Pastikan baris tetap diakhiri dengan null terminator
        text[currentY][MAX_LENGTH - 1] = '\0';
        // Lanjutkan proses ke baris di bawahnya
        currentY++;
        currentChar = overflowChar;
        insertX = 0;
    }
    // Perbarui posisi kursor setelah pengetikan
    if (cursorX < MAX_LENGTH - 1) 
    {
        cursorX++;
    } 
    else 
    {
        cursorX = 0;
        if (cursorY < lines - 1) cursorY++;
    }
}

// Menghapus karakter pada posisi kursor dan menarik sisa teks ke kiri
void deleteChar() {
    int len = strlen(text[cursorY]);

    if (cursorX < len) {
        // Geser karakter di sebelah kanan kursor ke arah kiri
        for (int i = cursorX; i < len; i++) {
            text[cursorY][i] = text[cursorY][i + 1];
        }
    }
    else if (cursorY < lines - 1) 
    {
        // Jika kursor berada di akhir baris, tarik teks dari baris bawahnya
        int currLen = len;
        int nextLen = strlen(text[cursorY + 1]);
        int spaceLeft = MAX_LENGTH - 1 - currLen;
        // Tentukan jumlah karakter yang dapat dipindahkan ke atas
        int copyCount;
        if (nextLen > spaceLeft) 
        {
            copyCount = spaceLeft;
        } 
        else 
        {
            copyCount = nextLen;
        }

        // Salin karakter yang muat ke ujung baris saat ini
        if (copyCount > 0) 
        {
            strncpy(&text[cursorY][currLen], text[cursorY + 1], copyCount);
            text[cursorY][currLen + copyCount] = '\0';
        }

        // Jika baris bawah masih memiliki sisa karakter, geser ke awal baris tersebut
        if (nextLen > copyCount) 
        {
            for (int i = 0; i < nextLen - copyCount; i++) 
            {
                text[cursorY + 1][i] = text[cursorY + 1][copyCount + i];
            }
            text[cursorY + 1][nextLen - copyCount] = '\0';
        } 
        else 
        {
            // Jika baris bawah habis ditarik, geser semua baris di bawahnya ke atas
            for (int i = cursorY + 1; i < lines - 1; i++)
            strcpy(text[i], text[i + 1]);
            text[lines - 1][0] = '\0';
            lines--; 
        }
    }
}

// Menghapus karakter di belakang kursor
void backspace() {
    // Jika kursor tidak di awal baris, mundur satu langkah dan hapus karakter
    if (cursorX > 0) 
    {
        cursorX--;       
        deleteChar();    
    }
    // Jika kursor di awal baris (kecuali baris pertama), gabungkan dengan baris di atasnya
    else if (cursorY > 0) 
    { 
        cursorX = strlen(text[cursorY - 1]); 
        cursorY--;                           
        deleteChar();                        
    }
}

// Fungsi Enter
void enterKey() {
    // Abaikan jika jumlah baris telah mencapai batas maksimal
    if (lines >= MAX_LINES) return;

    // Simpan sisa teks di sebelah kanan kursor untuk dipindahkan
    char temp[MAX_LENGTH];
    strcpy(temp, &text[cursorY][cursorX]);
    text[cursorY][cursorX] = '\0'; 

    // Geser baris di bawahnya ke bawah untuk memberi ruang
    for (int i = lines; i > cursorY; i--) 
    {
        strcpy(text[i], text[i - 1]);
    }
    
    // Letakkan sisa teks yang disimpan ke baris yang baru
    strcpy(text[cursorY + 1], temp);
    
    // Perbarui jumlah baris dan pindahkan kursor ke awal baris baru
    lines++;
    cursorY++;
    cursorX = 0; 
}