#include <Windows.h>

#include "Core/wire_window.h"

int main(int argc, char** argv)
{
	// Window Creation
	wire_window* window = malloc(sizeof(wire_window));
	wire_window* window2 = malloc(sizeof(wire_window));
	if (!window)
		return 1;
	window->height = 400;
	window->width = 600;
	window->name = "Hey there";
	create_window(window);
	if (!window)
		return 1;
	window2->height = 600;
	window2->width = 400;
	window2->name = "Hey there2";
	create_window(window2);

	// Main Loop
	while (1);

	// Cleanup

	// Exit
}