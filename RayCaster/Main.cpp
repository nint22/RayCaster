/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
***************************************************************/

#include <stdio.h>

#include "SDL.h"
#include "MainWindow.h"

int main( int argc, char* argv[] )
{
	// Create main window, and run the game
	MainWindow gameWindow;
	gameWindow.Run();

	return 0;
}
