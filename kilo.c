//
// Created by joseph on 31/01/24.
//

/*** includes ***/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** data ***/

//struct copy of the original terminal settings
struct termios orig_termios;

/*** terminal ***/

void die(const char *s){
    perror(s);
    exit(1);
}

void disableRawMode(){
    //apply original terminal settings
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode(){
    //get original terminal settings
    if(tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    //set disableRawMode function to execute on program exit
    atexit(disableRawMode);

    //create a copy of terminal settings and apply custom flags to enable "raw" mode
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    //apply new terminal settings
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/*** init ***/

int main(){
    enableRawMode();

    while(1){
        char c = '\0';
        if(read(STDIN_FILENO, &c, 1) == -1)
            die("read");
        if(iscntrl(c)){
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
        if(c == 'q') break;
    }
    return 0;
}