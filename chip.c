#include <stdio.h>
#include "chip.h"

bool keyboard[16];

const uint8_t font[80]=
{
0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


void resetdisplay(struct Chip8 * c){
	for (int i = 0; i<windowsizeY; i++) 
	for (int j = 0; j<windowsizeX; j++)
			c->display[i][j] = false;
}

void resetmem(struct Chip8 * c){
	int i = 4096;
	for(i=4096;i>0;i--)
		c->mem[i] = 0;
}

uint16_t resetchip(struct Chip8 * c){
	resetmem(c);
	for (int i=0;i<=80;i++)
		c->mem[80+i] = font[i];
	c->pc = 0x200;
	for (int i = 0; i<16; i++) {
		c->registers[i]=0;
	}
	resetdisplay(c);
	c->index = 0;
	c->sp = 0;
	c->timer = 0;
	c->stimer = 0;
	return c->pc;
}

uint16_t Fetch(struct Chip8 * c, struct opcode * o){
	uint8_t com = c->mem[c->pc];
	o->op1 =  (com & 0xF0) >> 4;
	o->op2 =  (com & 0xF);
	o->com1 = com;

	com = c->mem[c->pc+1];
	o->op3 =  (com & 0xF0) >> 4;
	o->op4 =  (com & 0xF);
	o->com2 = com;
	
	o->NNN = ((o->op2 << 8) | com); 
//printf("Fetched inctruction %d\n it contain op1 %x\n op2 %x\n op3 %x\n op4 %x\n",c->pc, o->op1, o->op2, o->op3, o->op4);
	c->pc = c->pc + 2;
	return c->pc;
}

void set_index(unsigned long a, struct Chip8 * c){
	c->index = a;
}
void set_register(int reg,int value, struct Chip8 * c){
	c->registers[reg] = value;
}
void add_register(int reg,int value, struct Chip8 * c){
	c->registers[reg] = c->registers[reg]+value;
}
void jump(unsigned long adress, struct Chip8 * c){
	if ((adress > 0x200) && (adress < 0xFFF))
		c->pc = adress;
	else printf("Jump erroe!");
}
void clearscreen(struct Chip8 * c){
	resetdisplay(c);
	return ;
}

void	clearcanvas(bool  canvas[windowsizeY][windowsizeX]){
	for (int i = 0; i<windowsizeY; i++)
	{
		for (int j = 0; j<windowsizeX; j++)
		{ canvas[i][j] = false;
		}
	}
}

void draw(int rx, int ry, int amount, struct Chip8 * c){
	int x = c->registers[ry];
	int y = c->registers[rx];
	int dis[amount];
	bool canvas[windowsizeY][windowsizeX];
	clearcanvas(canvas);
	for (int i = 0; i<amount;i++)
	{
		dis[i] = c->mem[(c->index)+i];
	}
	// bounders check
	if ((x+amount > windowsizeX) || (y+8>windowsizeY)) printf("dipslay bounders unchekd\n");
	//decode dis
	for (int i = 0; i<amount; i++)
	{
		for (int j = 0; j<8; j++)
		{
			uint8_t a = 0x80 >> j;
			canvas[y+j][x+i] = (dis[i] & a) != 0;
		}
	}

	//draw
	for (int i = 0; i<windowsizeY; i++)
	{
		for (int j = 0; j<windowsizeX; j++)
		{
			if (canvas[i][j])
				{
					if (c->display[i][j]) {
						c->registers[15] = 1;
						printf("detectedd collision");
					}
					c->display[i][j] = !(c->display[i][j]);
				}
		}
	}
	return;
}

void condjump(uint8_t a, uint8_t b, bool condition, struct Chip8 * c)
{
    if ((a==b) == condition) c->pc = c->pc + 2;
}

void retfunc(struct Chip8 * c){
    c->sp = c->sp-1;
    c->pc = c->stack[c->sp];
	c->stack[c->sp] = 0;
}
void call(unsigned long adress, struct Chip8 * c){
    c->stack[c->sp] = c->pc;
    c->sp = c->sp +1;
    c->pc = adress;
}
void decimal(int value, struct Chip8 * c){
    int a,b,t;
    a = value / 100;
    b = (value/10)%10;
    t = value%10;
    c->mem[c->index+0] = a;
    c->mem[c->index+1] = b;
    c->mem[c->index+2] = t;
}
void Logical(uint8_t* a,uint8_t b, int op)
{
    if (op == 1) *a = *a|b;
    if (op == 2) *a = *a&b;
    if (op == 3) *a = *a^b;
}
void settimer(uint8_t value, uint8_t * timer){
    *timer = value;
}
void addIndex(uint8_t value, struct Chip8 * c){
    c->index = c->index + value;
    if (c->index > 0xFFF) c->registers[15] = 1;
}
void waitforinput(uint8_t *key, struct Chip8 * c){
    c->pc = c->pc - 2;
    for (int i = 0; i < 16; i ++)
            {
                if (keyboard[i]) *key = i;
                c->pc = c->pc + 2;
                break;  
            }
}
void findchar(uint8_t chara, struct Chip8 * c){
    c->index = 0x80+(chara&0x0F)*10;
    //TODO: if char == 0
}
void trueadd(uint8_t * a, uint8_t b, uint8_t * carry){
    int sum = *a +b ;
    if (sum > 255) {
        *a = 255; *carry = 1;
    } 
    else
        {
        *a = sum; 
        *carry = 0;
    }
}
void stor(int x, struct Chip8 * c){
    int index = c->index;
    for (int i = 0; i <=x; i++)
            {
                c->mem[index+i] = c->registers[i];
            }
}
void load(int x, struct Chip8 * c){
    int index = c->index;
    for (int i = 0; i <=x; i++)
            {
                c->registers[i] = c->mem[index+i];
            }
}
void subtract5(uint8_t * a,uint8_t b, uint8_t * carry){
   if (b > *a) *carry = 0;
   else *carry = 1;
   *a = *a-b;
}
void  ofjump(unsigned long adress, struct Chip8 * c){
    c->pc = adress + c->registers[0];
}
void subtract7(uint8_t * a,uint8_t b,  uint8_t * carry){
   if (b < *a) *carry = 0;
   else *carry = 1;
   *a = b - *a ;
}
void ShiftR(uint8_t * a,uint8_t b, uint8_t * carry){
//TODO configurable bit about using b
    *carry  = b & 0x01;
    *a = b >> 1;
}
void ShiftL(uint8_t * a,uint8_t b, uint8_t * carry){
//TODO configurable bit about using b
    *carry  = b & 0x80;
    *a = b << 1;
}
uint16_t Decode(struct Chip8 * c, struct opcode o){
	switch(o.op1)
	{
		case 0: 
		switch(o.op4)
		{
			case 0: clearscreen(c); 
				break;
			case 0xE: retfunc(c); 
				break;
		}
			break;
		case 1: jump(o.NNN, c);
			break;
		case 2: call(o.NNN, c);
			break;
		case 3: condjump(c->registers[o.op2],o.com2,true, c);
			break;   
		case 4: condjump(c->registers[o.op2],o.com2,false, c);
			break;
		case 5: condjump(c->registers[o.op2],c->registers[o.op3],true, c);
			break;   
		case 6: set_register(o.op2, o.com2,c);
			break;
		case 7: add_register(o.op2, o.com2,c); 
			break;
		case 8: 
           switch(o.op4)
        {
			case 0: set_register(o.op2, c->registers[o.op3],c); 
				break;
        	case 1:
        	case 2:
        	case 3: Logical(&c->registers[o.op2],c->registers[o.op3],o.op4); 
				break;
        	case 4: trueadd(&c->registers[o.op2],c->registers[o.op3], &c->registers[15]); 
				break;
        	case 5: subtract5(&c->registers[o.op2],c->registers[o.op3],  &c->registers[15]); 
				break;
        	case 6: ShiftR(&c->registers[o.op2],c->registers[o.op3], &c->registers[15]); 
				break;
        	case 0xE: ShiftL(&c->registers[o.op2],c->registers[o.op3], &c->registers[15]); 
				break;
        	case 7: subtract7(&c->registers[o.op2],c->registers[o.op3],  &c->registers[15]); 
				break;
        }
        	break;
		case 9: condjump(c->registers[o.op2],c->registers[o.op3],false, c);
        	break;
		case 0xA: set_index(o.NNN, c);
			break;
		case 0xB: ofjump(o.NNN, c);
        	break;
 		case 0xD: draw(o.op2,o.op3,o.op4, c);
			break;
	    case 0xF:
        switch (o.com2){
			case 0x07: set_register(o.op2, c->timer, c);
            	break;
	        case 0x15: settimer(c->registers[o.op2], &c->timer);
	            break;
	        case 0x18: settimer(c->registers[o.op2], &c->stimer);
	            break;
	        case 0x1E: addIndex(c->registers[o.op2], c);
	            break;
	        case 0x0A: waitforinput(&c->registers[o.op2], c);
	            break;
	        case 0x29: findchar(c->registers[o.op2], c);
	            break;
	        case 0x33: decimal(c->registers[o.op2], c);
	            break;
	        case 0x55: stor(o.op2, c);
	            break;
	        case 0x65: load(o.op2, c); 
	            break;
        }
		break;
 }
	return o.op1;
}

