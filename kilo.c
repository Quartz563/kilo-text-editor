//
// Created by joseph on 31/01/24.
//
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

//struct copy of the original terminal settings
struct termios orig_termios;

void disableRawMode(){
    //apply original terminal settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode(){
    //get original terminal settings
    tcgetattr(STDIN_FILENO, &orig_termios);
    //set disableRawMode function to execute on program exit
    atexit(disableRawMode);
    //create a copy of terminal settings and apply custom flags to enable "raw" mode
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_iflag &= ~(ICRN | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    //apply new terminal settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int main(){
    enableRawMode();

    while(1){
        char c = '\0';
        read(STDIN_FILENO, &c, 1);
        if(iscntrl(c)){
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
        if(c == 'q') break;
    }
    return 0;
}