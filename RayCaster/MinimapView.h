/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: MinimapView.cpp/h
 Desc: Renders a preview of the world (minimap) on-screen.

***************************************************************/

#ifndef __MINIMAPVIEW_H__
#define __MINIMAPVIEW_H__

#include "VectorMath.h"
#include "World.h"
#include "Player.h"

class MinimapView
{
public:

	MinimapView( Vector2i pos, int tileSize, const World* gameWorld, const Player* player );
	~MinimapView();

	void Render( SDL_Renderer* renderer );

private:
	
	const World* m_gameWorld;
	const Player* m_player;

	Vector2i m_minimapPos;
	int m_minimapTileSize;

};

#endif
