#include <stdio.h>
#include <stdio.h>
#include "main.h"

linecount = 0;
void displaytext(){
    int i = 0;
    while(i < linecount){
        printf("%d: %s\n", i + 1, text[i]);
    }
}



