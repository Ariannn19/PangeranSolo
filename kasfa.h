#ifndef kasfa_H
#define kasfa_H
#define MAX_LINES 1000
#define MAX_LENGTH 100

//extern char text[MAX_LINES][MAX_LENGTH];
//extern int linecount;

void displaytext (char text[][MAX_LENGTH], int linecount);
void find (char text[][MAX_LENGTH],int linecount);
void wordcounter (char text[][MAX_LENGTH], int linecount);
void wraptext(char text[][MAX_LENGTH], int linecount);

#endif