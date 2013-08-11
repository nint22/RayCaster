/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: Player.cpp/h
 Desc: This class abstracts the player's first-person orientation,
 world location, movement, etc.

***************************************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "SDL.h"
#include "Utilities.h"
#include "VectorMath.h"

class Player
{
public:

	// Construct with initial position and facing
	Player( const Vector3f& worldPosition, const float facing );
	~Player();

	// Set world position
	void SetPosition( const Vector3f& worldPosition ) { m_worldPosition = worldPosition; }
	const Vector3f GetPosition() const { return m_worldPosition; }

	// Set facing
	void SetFacing( const float facing ) { m_facing = facing; }
	const float GetFacing() const { return m_facing; }

	// Process the given event map
	void UpdateKeys( const SDL_Event& inputEvent );

	// Update actual movement (should be called after giving all the key updates)
	void Update( float dTime );

protected:

	// Unit circle, radians, 0: X+ axis, while positive value grows counter-clockwise
	Vector3f m_worldPosition;
	float m_facing;

	// Movement-key state
	bool m_keyFwd, m_keyBck, m_keyLeft, m_keyRight;
	bool m_keyRotLeft, m_keyRotRight;
};

#endif
