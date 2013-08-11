/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: World.cpp/h
 Desc: The world map; the world is a discrete grid of cube
 locations, where a cube is one big wall-block. We define the
 texture associated with each map-tile in this file.

***************************************************************/

#ifndef __WORLD_H__
#define __WORLD_H__

#include "Utilities.h"
#include "VectorMath.h"

// World tile (the block at the given location)
class WorldTile
{
public:

	int m_tileId;

};

// World class implementation
class World
{
public:

	// Construct from a text file
	World( const std::string& worldFileName );
	~World();

	// Get world size
	const Vector2i GetWorldSize() const { return m_worldSize; }

	// Get tile at given world position
	const WorldTile* GetWorldTile( int x, int y ) const;

protected:

	// 2D array where it's allocated through a new WorldTile[width * height] call
	WorldTile* m_worldMap;
	Vector2i m_worldSize;

};

#endif
