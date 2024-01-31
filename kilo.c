//
// Created by joseph on 31/01/24.
//
#include <unistd.h>

int main(){
    char c;
    while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    return 0;
}