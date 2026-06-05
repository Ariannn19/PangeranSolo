#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h> 
#include "arian.h"
#include "rasya.h"
#include "kasfa.h"

int main() {
    initEditor();
    RiwayatNode *riwayat = NULL;
    push_riwayat(&riwayat, head); 

    int ch;
    int running = 1;
    char filename[100]; 
    char keyword[100]; 

    system("cls");

    while (running) {
        tampilkanTeks();
        
        ch = _getch();

        if (ch == 0 || ch == 224) { 
            ch = _getch(); 
            if (ch == 83) { 
                kursor.isShift = false;
                kursor.select = NULL;
                delete(); 
            }
            else{
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    if (kursor.isShift == false) {
                        kursor.isShift = true;
                        kursor.select = kursor.currentLine; 
                        kursor.startX = kursor.kursorX;     
                    }
                    } else {
                        if(ch != 24 && ch != 17) {
                            kursor.isShift = false;
                            kursor.select = NULL;
                        }
                    }   
            }    
                moveCursor(ch);
        } else {
            if ((ch >= 32 && ch <= 126) || ch == 8 || ch == 13) {
                kursor.isShift = false;
                kursor.select = NULL;
            }
            
            switch (ch) {
                case 27: // Tombol ESC
                    running = 0;
                    break;
                    
                case 8:  // Tombol Backspace
                    backspace();
                    break;
                    
                case 13: // Tombol Enter
                    enter();
                    push_riwayat(&riwayat, head); 
                    break;
                    
                case 19: // Ctrl + S -> Save
                    system("cls"); 
                    printf(">>> SIMPAN DOKUMEN <<<\n");
                    printf("Masukkan nama file (contoh: dokumen.txt): ");
                    scanf("%s", filename); 
                    
                    save_ke_file(filename, head);
                    system("cls"); 
                    break;
                    
                case 15: // Ctrl + O -> Open/Load
                    system("cls");
                    printf(">>> BUKA DOKUMEN <<<\n");
                    printf("Masukkan nama file yang ingin dibuka: ");
                    scanf("%s", filename);
                    
                    Line* tempHead = load_dari_file(filename);
                    if (tempHead != NULL) { 
                        bersihkanMemori();
                        head = tempHead;
                        
                        Line* temp = head;
                        while (temp->next != NULL) {
                            temp = temp->next;
                        }
                        tail = temp;

                        kursor.currentLine = head;
                        kursor.kursorX = 0;
                        push_riwayat(&riwayat, head); 
                        system("cls"); 
                    } else {
                        printf("\n[!] File '%s' tidak ditemukan! Tekan tombol apa saja untuk kembali...", filename);
                        _getch(); 
                        system("cls");
                    }
                    break;
                    
                case 26: // Ctrl + Z -> Undo
                {
                    Line* undoHead = undo(&riwayat);
                    if (undoHead != NULL) {
                        head = undoHead;
                        Line* temp = head;
                        while (temp->next != NULL) {
                            temp = temp->next;
                        }
                        tail = temp;
                        kursor.currentLine = tail;
                        kursor.kursorX = tail->len;
                        system("cls");
                    }
                    break;
                }
                    
                case 25: // Ctrl + Y -> Redo
                {
                    Line* redoHead = redo(&riwayat);
                    if (redoHead != NULL) {
                        head = redoHead;
                        Line* temp = head;
                        while (temp->next != NULL) {
                            temp = temp->next;
                        }
                        tail = temp;
                        kursor.currentLine = tail;
                        kursor.kursorX = tail->len;
                        system("cls");
                    }
                    break;
                }

                case 6: // Ctrl + F -> Find
                    system("cls");
                    printf(">>> CARI KATA <<<\n");
                    printf("Masukkan kata yang ingin dicari: ");
                    scanf("%s", keyword);
                    printf("\n");
                    find(head, keyword); 
                    printf("\nTekan tombol apa saja untuk kembali ke editor...");
                    _getch();
                    system("cls");
                    break;

                case 23: // Ctrl + W -> Word Counter 
                    system("cls");
                    printf(">>> STATISTIK DOKUMEN <<<\n\n");
                    printf("--- PREVIEW DOKUMEN ---\n");
                    int total_lines = linecounter(head); 
                    int total_words = wordcounter(head);
                    int total_chars = charcounter(head);
                    printf("\n--- HASIL PERHITUNGAN ---\n");
                    printf("Total Baris    : %d\n", total_lines);
                    printf("Total Kata     : %d\n", total_words);
                    printf("Total Karakter : %d\n", total_chars);
                    printf("\nTekan tombol apa saja untuk kembali ke editor...");
                    _getch();
                    system("cls");
                    break;

                case 17: // Ctrl + Q -> Copy (Kode ASCII 17)
                    copy();
                    break;
                    
                case 5: // Ctrl + E -> Paste (Kode ASCII 5)
                    paste();
                    push_riwayat(&riwayat, head); 
                    break;
                    
                case 24: // Ctrl + X -> Cut
                    cut();
                    push_riwayat(&riwayat, head); 
                    break;
                    
                default:
                    if (ch >= 32 && ch <= 126) {
                        insertChar((char)ch);
                        
                        if (ch == 32) {
                            push_riwayat(&riwayat, head); 
                        } 
                    }
                    break;
            }
        }
    }

    bersihkanMemori();
    system("cls");
    printf("Keluar dari PangeranSolo. Sampai jumpa!\n");

    return 0;
}