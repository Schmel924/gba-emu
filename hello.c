#include <stdio.h>
#include "raylib.h"
#include "chip.h"

void DrawChipxel(int i,int j,int delta,Color color)
{
	DrawPixel(delta+i,delta+j,color);
}

int main (){
	struct Chip8 chip;
	uint16_t i;
	i = resetchip(&chip);
	int scale = 10;
	struct opcode op;
	if (i==0x200) printf("reset OK");
	FILE *f = fopen("roms/logo.ch", "r");
	if (f == NULL) printf("Eroor in reading file");
	uint8_t a = 0;
	while(fread( &a, sizeof(a), 1, f) == 1 && i<3500){
		printf("%x wrote in %d place\n",a,i);
		chip.mem[i] = a;
		i++;
	}
	const int screenWidth = scale*windowsizeY;
	const int screenHeight = scale*windowsizeX;
	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	SetTargetFPS(60);
	//Fetch/Decode/Execute loop
	while(op.op1 != 1) {
		Fetch(&chip, &op);
		//printf("in %x pos we have op %x\n", chip.pc, chip.mem[chip.pc]);
		Decode(&chip, op);
	}
	//Display loop

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	BeginDrawing();
	ClearBackground(RAYWHITE);
	int delta = 10;
	for (int i = 0; i< windowsizeY; i++)
		for (int j = 0; j< windowsizeX; j++)
		{
			if(chip.display[i][j])
			DrawRectangle(scale*i, scale*j, scale, scale, BLACK); 
		}  
	EndDrawing(); }
	CloseWindow();
	return 0;
}
