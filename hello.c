#include <stdio.h>
#include "raylib.h"
#include "chip.h"

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
