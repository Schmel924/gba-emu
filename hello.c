#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "chip.h"
uint8_t inputDecode(char i){
	switch (i) {
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 12;
	case 'q': return 4;
	case 'w': return 5;
	case 'e': return 6;
	case 'r': return 13;
	case 'a': return 7;
	case 's': return 8;
	case 'd': return 9;
	case 'f': return 14;
	case 'z': return 10;
	case 'x': return 0;
	case 'c': return 11;
	case 'v': return 15;
	}
	return -1;
}

void drawDebug(struct Chip8 * c, struct opcode * op){
	DrawText(TextFormat("opcode is _%u", op->op1), 10*windowsizeX, 0, 1, BLACK);

	DrawText(TextFormat("opcode is _%u", c->registers[0] ), 10*windowsizeX, 0, 1, BLACK);
	DrawText(TextFormat("opcode is _%u", c->registers[1]), 10*windowsizeX, 10, 1, BLACK);
	DrawText(TextFormat("opcode is _%u", c->registers[2]), 10*windowsizeX, 20, 1, BLACK);
	DrawText(TextFormat("opcode is _%u", c->registers[3]), 10*windowsizeX, 30, 1, BLACK);

	DrawText(TextFormat("Keycode is _%u", c->keypressed ), 10*windowsizeX, 40, 1, BLACK);
}
void DrawChipxel(int i,int j,int delta,Color color)
{
	DrawPixel(delta+i,delta+j,color);
}

int main (int argc, char * * argv){
	struct Chip8 chip;
	uint16_t i;
	i = resetchip(&chip);
	int scale = 10;
	struct opcode op;
	FILE *f = NULL;
	if (i==0x200) printf("reset OK");
	if (argc == 1){
	f = fopen("chip8-test-suite/bin/3-corax+.ch8", "r");
	if (f == NULL) printf("Eroor in reading file");}
	else{
	f = fopen(argv[1], "r");
	if (f == NULL) printf("Eroor in reading file");}
	uint8_t a = 0;
	while(fread( &a, sizeof(a), 1, f) == 1 && i<3500){
		//printf("%x wrote in %d place\n",a,i);
		chip.mem[i] = a;
		i++;
	}
	chip.mem[0x1ff] = 2; //auto keyboard tester
	const int screenWidth = scale*windowsizeX+100;
	const int screenHeight = scale*windowsizeY;
	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	SetTargetFPS(30);
	char input = -1;
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		char in = GetCharPressed();
		if (in != 0) input = in;
		chip.keypressed = inputDecode(input);
		Fetch(&chip, &op);
		BeginDrawing();
		drawDebug(&chip, &op);
		//waitforok();
		Decode(&chip, op);
	ClearBackground(RAYWHITE);
	//int delta = 10;
	if(chip.drawopcode){
			updategraphics(&chip); 
		}
		{
	for (int i = 0; i< windowsizeX; i++)
		for (int j = 0; j< windowsizeY; j++)
		{
			if(chip.Notdisplay[i][j])
			DrawRectangle(scale*i, scale*j, scale, scale, BLACK); 
		}  
		}
	EndDrawing(); }
	CloseWindow();
	return 0;
}
