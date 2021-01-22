//source copied from https://stackoverflow.com/questions/6698161/getting-raw-input-from-console-using-c-or-c
#include <unistd.h>
#include <termios.h>
#include <stdio.h>

/* Initialize new terminal i/o settings */
static struct termios old, new1;
/*
stty -a show "...-igncr icrnl" so pressing [ENTER] show 10;
to make [ENTER] show 13, either:
1. uncomment new1.c_iflag &= ~ICRNL
2. stty -icrnl
*/
void initTermios(int echo) {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    new1 = old; /* make new settings same as old settings */
    new1.c_lflag &= ~ICANON; /* disable buffered i/o */
    new1.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
	//new1.c_iflag &= ~ICRNL;//[ENTER] is 1. if commented:10; 2.Not commented:13
	//
    tcsetattr(0, TCSANOW, &new1); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

int main(void)
{
    char c;
    initTermios(0);
    while (1) { read(0, &c, 1); printf("%d\n", c); }
}

