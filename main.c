#include<stdio.h>
#include "arian.h"
#include "kasfa.h"

char text[MAX_LINES][MAX_LENGTH];
int linecount = 0;


int main(){
    int choice;

    do{
        printf("======PangeranSolo======");
        printf("1. Input Text ");
        printf("2. Display text ");
        printf("3. Exit");

        printf("pilih: ");
        scanf("%d", choice);

        switch (choice)
        {
        case 1:
            inputText(text, linecount);
            break;
        case 2:
            displaytext(text, linecount);
        
        }
    }while(choice != 3);

    return 0;
}

