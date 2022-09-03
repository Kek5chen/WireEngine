#include <Windows.h>

#include "Core/wire_window.h"

int main(int argc, char** argv)
{
	// Window Creation
	wire_window* window = malloc(sizeof(wire_window));
	if (!window)
		return 1;
	window->height = 400;
	window->width = 600;
	window->name = "Hey there";
	create_window(window);

	// Main Loop
	while (!GetAsyncKeyState(VK_END));
	close_window(window);

	while (GetAsyncKeyState(VK_END));
	// Cleanup

	// Exit
}