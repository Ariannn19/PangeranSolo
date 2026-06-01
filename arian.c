#include "arian.h"

Line* head = NULL;
Line* tail = NULL;
Kursor kursor;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 20;    
    cursorInfo.bVisible = FALSE; // Sembunyikan
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void showCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 20;
    cursorInfo.bVisible = TRUE;  // Munculkan
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

Line* buatBaris() {
    Line* barisBaru = (Line*)malloc(sizeof(Line));
    if (barisBaru == NULL) {
        printf("Gagal alokasi memori");
        exit(1);
    }
    barisBaru->info[0] = '\0';
    barisBaru->len = 0;
    barisBaru->next = NULL;
    barisBaru->prev = NULL;
    return barisBaru;
}

void initEditor() {
    Line* firstLine = buatBaris();
    head = tail = firstLine;
    kursor.currentLine = firstLine;
    kursor.kursorX = 0;

    kursor.select = NULL;
    kursor.startX = 0;
    kursor.isShift = false;
}

void insertChar(char c) {
    Line* curr = kursor.currentLine;
    if (curr->len < MAX_KOLOM - 1) {
        for (int i = curr->len; i >= kursor.kursorX; i--) {
        curr->info[i + 1] = curr->info[i];
        }        
        curr->info[kursor.kursorX] = c;
        curr->len++;
        kursor.kursorX++;

        curr->info[curr->len] = '\0';
    }
    else{
        char overflowChar;
        if (kursor.kursorX == curr->len){
            overflowChar = c;
            curr->info[MAX_KOLOM -1] = '\0';
        }
        else {
            overflowChar = curr->info[curr->len - 1];
            int i = curr->len - 1; 
            while (i > kursor.kursorX){
                curr->info[i] = curr->info[i - 1];
                i--;
            }
            curr->info[kursor.kursorX] = c;
            kursor.kursorX++;
        }
        wrapChar(curr, overflowChar);
        if (kursor.kursorX >= MAX_KOLOM -1){
            kursor.currentLine = curr->next;
            kursor.kursorX = 1;
        }
    }
}

void wrapChar(Line* curr, char overflowChar) {
    if (curr->next == NULL) {
        Line* newLine = buatBaris();
        newLine->prev = curr;
        curr->next = newLine;
        if(tail == curr){
            tail = newLine;
        }
    }
    Line* nextLine = curr->next;

    if (nextLine->len >= MAX_KOLOM -1){
        char nextOverflow = nextLine->info[nextLine->len - 1];

        for (int i = nextLine->len - 1; i > 0; i--) {
            nextLine->info[i] = nextLine->info[i - 1];
        }
        nextLine->info[0] = overflowChar;

        wrapChar(nextLine, nextOverflow);
    }
    else{
        for (int i = nextLine->len; i >= 0; i--) {
            nextLine->info[i + 1] = nextLine->info[i];
        }
        nextLine->info[0] = overflowChar;
        nextLine->len++;
    }
}

void gabungBaris(Line* atas, Line* bawah) {
    strcat(atas->info, bawah->info);
    atas->len = atas->len + bawah->len;

    atas->next = bawah->next;

    if(bawah->next != NULL){
        bawah->next->prev = atas;
    }

    if(tail == bawah){
        tail = atas;
    }

    free(bawah);
}

void backspace() {
    Line* curr = kursor.currentLine;
    if (kursor.kursorX == 0) {
        if (curr->prev == NULL){
            return;
        }
    }
    moveCursor(75);
    delete();
}

void delete() {
    Line* curr = kursor.currentLine;
    if (kursor.kursorX == curr->len) {
        if (curr->next == NULL){
            return;
        }

        Line* nextLine = curr->next;
        if (nextLine->len == 0) {
            curr->next = nextLine->next;
            if (nextLine->next != NULL) nextLine->next->prev = curr;
            if (tail == nextLine) tail = curr;
            free(nextLine);
            return;
        }        
        if(curr->len + nextLine->len <= MAX_KOLOM -1){
            gabungBaris(curr, nextLine);
            return;
        }

        if (curr->len >= MAX_KOLOM - 1) {
            return;
        }
        
        curr->info[curr->len] = nextLine->info[0];
        curr->len++;
        curr->info[curr->len] = '\0';

        for (int i = 0; i < nextLine->len - 1; i++) {
            nextLine->info[i] = nextLine->info[i + 1];
        }
        nextLine->len--;
        nextLine->info[nextLine->len] = '\0';

        if(nextLine->len == 0){
            curr->next = nextLine->next;
            if(nextLine->next != NULL){
                nextLine->next->prev = curr;
            }
            if (tail == nextLine){
                tail = curr;
            }
            free(nextLine);
        }
        return;
    }
    for (int i = kursor.kursorX; i < curr->len; i++) {
        curr->info[i] = curr->info[i + 1];
    }
    curr->len--;
    curr->info[curr->len] = '\0';
}

void enter() {
    Line* curr = kursor.currentLine;
    Line* barisBaru = buatBaris();

    strncpy(barisBaru->info, curr->info + kursor.kursorX, MAX_KOLOM - 1);
    barisBaru->info[MAX_KOLOM - 1] = '\0';
    barisBaru->len = strlen(barisBaru->info);

    curr->info[kursor.kursorX] = '\0';
    curr->len = kursor.kursorX;
    
    barisBaru->next = curr->next; 
    barisBaru->prev = curr;
    
    if (curr->next != NULL) {
        curr->next->prev = barisBaru;
    }
    curr->next = barisBaru;
    
    if (tail == curr) {
        tail = barisBaru;
    }
    
    kursor.currentLine = barisBaru;
    kursor.kursorX = 0;
}

blockArea panjangBlock() {
    blockArea area;
    area.startLine = NULL;
    area.endLine = NULL;
    area.startPos = 0;
    area.endPos = 0;

    if (kursor.isShift == false || kursor.select == NULL) {
        return area; 
    }

    Line* temp = head; 
    
    while (temp != NULL) { 
        if (temp == kursor.select && temp == kursor.currentLine) { 
            area.startLine = temp;
            area.endLine = temp;
            
            if (kursor.startX < kursor.kursorX) {
                area.startPos = kursor.startX;
                area.endPos = kursor.kursorX;
            } else {
                area.startPos = kursor.kursorX;
                area.endPos = kursor.startX;
            }
            break;
        } 
        else if (temp == kursor.select) { 
            area.startLine = kursor.select;
            area.startPos = kursor.startX;
            area.endLine = kursor.currentLine;
            area.endPos = kursor.kursorX;
            break;
        } 
        else if (temp == kursor.currentLine) { 
            area.startLine = kursor.currentLine;
            area.startPos = kursor.kursorX;
            area.endLine = kursor.select;
            area.endPos = kursor.startX;
            break;
        }
        temp = temp->next; 
    }
    
    return area; 
}

void moveCursor(int key) {
    switch (key) {
        case 75: /* Kiri */
            if (kursor.kursorX > 0) {
                kursor.kursorX--;
            }
            else if (kursor.currentLine->prev != NULL) {
                kursor.currentLine = kursor.currentLine->prev;
                kursor.kursorX = kursor.currentLine->len;
            }
            break; 

        case 77: /* Kanan */
            if (kursor.kursorX < kursor.currentLine->len) {
                kursor.kursorX++;
            }
            else if (kursor.currentLine->next != NULL) {
                kursor.currentLine = kursor.currentLine->next;
                kursor.kursorX = 0;
            }
            break;

        case 72: /* Atas */
            if (kursor.currentLine->prev != NULL) {
                kursor.currentLine = kursor.currentLine->prev;
                if (kursor.kursorX > kursor.currentLine->len) {
                    kursor.kursorX = kursor.currentLine->len;
                }
            }
            break;

        case 80: /* Bawah */
            if (kursor.currentLine->next != NULL) {
                kursor.currentLine = kursor.currentLine->next;
                if (kursor.kursorX > kursor.currentLine->len) {
                    kursor.kursorX = kursor.currentLine->len;
                }
            }
            break;
    }
}

void bersihkanMemori() {
    Line* curr = head;
    Line* Hapus;
    
    while (curr != NULL) {
        Hapus = curr;          
        curr = curr->next; 
        free(Hapus);               
    }
    head = NULL;
    tail = NULL;
    kursor.currentLine = NULL;
}

void tampilkanTeks() {
    hideCursor();
    gotoxy(0, 0);
    
    blockArea area = panjangBlock(); 
    
    Line* startLine = area.startLine;
    Line* endLine = area.endLine;
    int startPos = area.startPos;
    int endPos = area.endPos;
    bool inSelection = false;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    Line* temp = head;
    int indexBaris = 0;
    int targetBaris = 0;
    
    while (temp != NULL) {
        if (kursor.isShift && temp == startLine) {
            inSelection = true;
        }
        for (int i = 0; i < MAX_KOLOM; i++) {
            bool isHighlight = false;
            
            if (kursor.isShift && inSelection) {
                if (startLine == endLine) { 
                    // Kasus 1: Blokir hanya di satu baris yang sama
                    if (i >= startPos && i < endPos) isHighlight = true;
                } else if (temp == startLine) { 
                    // Kasus 2: Berada di baris pertama dari blok multi-baris
                    if (i >= startPos) isHighlight = true;
                } else if (temp == endLine) { 
                    // Kasus 3: Berada di baris terakhir dari blok multi-baris
                    if (i < endPos) isHighlight = true;
                } else { 
                    // Kasus 4: Baris ini terjepit di tengah-tengah blok area
                    isHighlight = true; 
                }
            }

            if (isHighlight) {
                SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED); 
            } else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); 
            }

            if (i < temp->len) {
                printf("%c", temp->info[i]);
            } else {
                printf(" ");
            }
        }

        if (kursor.isShift && temp == endLine) {
            inSelection = false;
        }

        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); 
        printf("\n");

        if (temp == kursor.currentLine) {
            targetBaris = indexBaris;
        }
        temp = temp->next;
        indexBaris++;
    }

    int i = 0;
    while (i < MAX_KOLOM) {
        printf(" ");
        i++;
    }
    printf("\n");

    gotoxy(kursor.kursorX, targetBaris);
    showCursor();
}