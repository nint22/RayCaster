/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: WorldView.cpp/h
 Desc: The world renderer. Takes the world map, player, and
 renders what the player sees.

***************************************************************/

#ifndef __WORLDVIEW_H__
#define __WORLDVIEW_H__

#include "Player.h"
#include "World.h"

class WorldView
{
public:

	WorldView( const World* world, const Player* player );
	~WorldView();

	void Render( SDL_Renderer* renderer, Vector2i m_windowSize );

protected:

	// Given a ray origin and direction (through heading)
	// Returns the point that was hit
	Vector3f CollisionCheck( Vector3f origin, float radians );

private:

	const World* m_gameWorld;
	const Player* m_player;

};

#endif
