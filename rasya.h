#ifndef RASYA_H
#define RASYA_H
#include "arian.h"

typedef struct RiwayatNode {
    Line *kertas_head;
    struct RiwayatNode* next;  // Pointer ke node berikutnya
    struct RiwayatNode* prev;  // Pointer ke node sebelumnya
} RiwayatNode;

void save_ke_file(char filename[], Line* head);   
Line* load_dari_file(char filename[]);

Line* salin_kertas(Line* head);
void push_riwayat(RiwayatNode **current_state, Line *kertas_sekarang);
Line* undo(RiwayatNode **current_state);
Line* redo(RiwayatNode **current_state);

#endif