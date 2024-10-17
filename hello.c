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
	struct opcode op;
	if (i==0x200) printf("reset OK");
	FILE *f = fopen("roms/logo.ch", "r");
	if (f == NULL) printf("Eroor in reading file");
	uint8_t a = 0;
	while(fread( &a, sizeof(a), 1, f) == 1 && ++i<3500)
		chip.mem[0x200+i] = a;
	const int screenWidth = 800;
	const int screenHeight = 450;
	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	SetTargetFPS(60);
	
	//Fetch/Decode/Execute loop
	//i = Fetch(&chip, &op);
	if (i>0x200 && i<0xFFF && i == chip.pc) ;//Fetch OK
	
	//i = Decode(&chip, op);

	//Display loop

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
	BeginDrawing();
	ClearBackground(RAYWHITE);
	int delta = 10;
	DrawRectangle(delta, delta, windowsizeY, windowsizeX, BLACK); 
	for (int i = 0; i< windowsizeY; i++)
		for (int j = 0; j< windowsizeX; j++)
		{
			if(chip.display[i][j]) DrawChipxel(i, j, delta, WHITE);
		}  
	EndDrawing(); }
	CloseWindow();
	return 0;
}
