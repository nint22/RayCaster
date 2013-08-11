/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: MainWindow.cpp/h
 Desc: The high-level renderer-manager that takes care of high-
 level game logic with the renderable. This class really is for
 inter-object communication, and high-level window management.

***************************************************************/

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "SDL.h"
#include "VectorMath.h"

#include "World.h"
#include "Player.h"
#include "MinimapView.h"
#include "WorldView.h"

class MainWindow
{
public:

	// Construct / destruct window
	MainWindow();
	~MainWindow();

	// Start the main loop; once this is called, it only returns on application exit
	// Keeps looping until "Update(...)" returns false
	void Run();

protected:

	// Main update logic; if false is ever returned, we must stop rendering..
	bool Update(float dTime);

	// High-level rendering logic
	void Render(float dTime);

private:

	SDL_Window* m_window;
	SDL_Renderer* m_renderer; // This is kind of like a main context that must be used by SDL for most render-related calls
	Vector2i m_windowSize;

	World* m_gameWorld;
	Player* m_player;

	WorldView* m_worldView;
	MinimapView* m_minimapView;

};

#endif
