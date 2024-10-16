#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "raylib.h"

#define windowsizeX  32
#define windowsizeY  64

// specification
struct Chip8{
	uint8_t mem[4096];
	bool display[windowsizeY][windowsizeX];
	uint16_t pc;
	uint16_t index;
	uint16_t stack[256];
	int sp;
	uint8_t timer;
	uint8_t stimer;
	uint8_t registers[16];
};

struct opcode{
	int op1;
	int op2;
	int op3;
	int op4;
	int com1;
	int com2;
	unsigned long NNN;
};


uint8_t font[80]=
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

void memtest(struct Chip8 * c){
	int i = 4096;
	for(i=4096;i>0;i--)
	c->mem[i] = i;
}

uint16_t resetchip(struct Chip8 * c){
	memtest(c);
	int i=0;
	for (i=0;i<=80;i++)
		c->mem[80+i] = font[i];
	c->pc = 0x200;
	c->mem[0x200] = 0x60;
	c->mem[0x201] = 69;
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
void clearscreen(){
	return ;
}

void	clearcanvas(bool  canvas[64][32]){
	for (int i = 0; i<windowsizeX; i++)
	{
		for (int j = 0; j<windowsizeY; j++)
		{ canvas[i][j] = false;
		}
	}
}

void draw(int rx, int ry, int amount, struct Chip8 * c){
	int x = c->registers[rx];
	int y = c->registers[ry];
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
			uint8_t a = 0x80 >> j; // Надо перепроверить
			canvas[x+i][y+j] = (dis[i] & a) != 0;
		}
	}

	//draw
	for (int i = 0; i<windowsizeX; i++)
	{
		for (int j = 0; j<windowsizeY; j++)
		{
			if (canvas[i][j])
				{
					if (c->display[i][j]) printf("detectedd collision");
					c->display[i][j] = !(c->display[i][j]);
				}
		}
	}
	return;
}

uint16_t Decode(struct Chip8 * c, struct opcode o){
	switch(o.op1)
	{
	case 0: clearscreen();
		break;
	case 1: jump(o.NNN, c);
		break;
	case 6: set_register(o.op2, o.com2,c);
		break;
	case 7: add_register(o.op2, o.com2,c);
		break;
	case 10: set_index(o.NNN, c);
		break;
	case 13: draw(o.op2,o.op3,o.op4, c);
		break;
	}
	return o.op1;
}

int main (){
	struct Chip8 chip;
	uint16_t i;
	i = resetchip(&chip);
	struct opcode op;
	if (i==0x200) printf("reset OK");

	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	SetTargetFPS(60);
 
	//Fetch/Decode/Execute loop
	printf("\nregister0 = %u",chip.registers[0]);
	i = Fetch(&chip, &op);
	if (i>0x200 && i<0xFFF && i == chip.pc) ;//Fetch OK
	
	i = Decode(&chip, op);
	printf("\nregister0 = %u",chip.registers[0]);

	//Display loop

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	BeginDrawing();
	ClearBackground(RAYWHITE);
	DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
	EndDrawing(); }
	CloseWindow();
	return 0;
}
