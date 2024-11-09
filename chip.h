#define windowsizeY  32
#define windowsizeX  64
#include <stdint.h>
#include <stdbool.h>

// specification
struct Chip8{
	uint8_t mem[4096];
	bool display[windowsizeX][windowsizeY];
	uint16_t pc;
	uint16_t index;
	uint16_t stack[256];
	int sp;
	uint8_t timer;
	uint8_t stimer;
	uint8_t registers[16];
	bool keyboard[16];
	char realkeyboard[16];
	uint8_t keypressed;
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


void memtest(struct Chip8 * );

uint16_t resetchip(struct Chip8 * c);
uint16_t Fetch(struct Chip8 * c, struct opcode * o);
void set_index(unsigned long a, struct Chip8 * c);
void add_register(int reg,int value, struct Chip8 * c);
void set_register(int reg,int value, struct Chip8 * c);
void jump(unsigned long adress, struct Chip8 * c);
void clearscreen();
void	clearcanvas(bool  canvas[windowsizeX][windowsizeY]);
void draw(int rx, int ry, int amount, struct Chip8 * c);
uint16_t Decode(struct Chip8 * c, struct opcode o);
void clearscreen(struct Chip8 * c);
void assignkeys(char realkeyboard[]);
void cleankeyboard(bool * keyboard);
