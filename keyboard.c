#include "chip.h"
//keyboard
void cleankeyboard(bool *keyboard){

    for (int i=0;i<16;i++)
        keyboard[i] = false;
}
void assignkeys(int *realkeyboard){
realkeyboard[0] = '1';
realkeyboard[1] = '2';
realkeyboard[2] = '3';
realkeyboard[3] = '4';

realkeyboard[4] = 'q';
realkeyboard[5] = 'w';
realkeyboard[6] = 'e';
realkeyboard[7] = 'r';

realkeyboard[8] = 'a';
realkeyboard[9] = 's';
realkeyboard[10] = 'd';
realkeyboard[11] = 'f';

realkeyboard[12] = 'z';
realkeyboard[13] = 'x';
realkeyboard[14] = 'c';
realkeyboard[15] = 'v';
}
