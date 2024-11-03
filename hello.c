#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "chip.h"

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
	const int screenWidth = scale*windowsizeX;
	const int screenHeight = scale*windowsizeY;
	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	SetTargetFPS(30);

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		int input = GetKeyPressed();
		for (int i = 0; i<16; i++){
			if (chip.keyboard[i] && IsKeyDown(chip.realkeyboard[i])) 
				continue;
			else 
				chip.keyboard[i] = false;
			if (chip.realkeyboard[i]==input) 
				chip.keyboard[i] = true; 
		}
		Fetch(&chip, &op);
		Decode(&chip, op);
	BeginDrawing();
	ClearBackground(RAYWHITE);
	int delta = 10;
	for (int i = 0; i< windowsizeX; i++)
		for (int j = 0; j< windowsizeY; j++)
		{
			if(chip.display[i][j])
			DrawRectangle(scale*i, scale*j, scale, scale, BLACK); 
		}  
	EndDrawing(); }
	CloseWindow();
	return 0;
}
