//
// Created by joseph on 31/01/24.
//
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>

struct termios orig_termios;

void disableRawMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode(){
    struct terminos raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int main(){
    enableRawMode();

    char c;
    while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    return 0;
}