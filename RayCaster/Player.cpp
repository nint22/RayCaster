/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
***************************************************************/

#include "Player.h"
#include "Utilities.h"
#include <math.h>

Player::Player( const Vector3f& worldPosition, const float facing )
	: m_worldPosition( worldPosition )
	, m_facing( facing )
	, m_keyFwd(false), m_keyBck(false), m_keyLeft(false), m_keyRight(false)
	, m_keyRotLeft(false), m_keyRotRight(false)
{

}

Player::~Player()
{
}

void Player::UpdateKeys( const SDL_Event& inputEvent )
{
	// Check for AWSD movement style
	bool keyDown = (inputEvent.type == SDL_KEYDOWN);
	if( inputEvent.key.keysym.sym == SDLK_w )
		m_keyFwd = keyDown;
	else if( inputEvent.key.keysym.sym == SDLK_a )
		m_keyLeft = keyDown;
	else if( inputEvent.key.keysym.sym == SDLK_d )
		m_keyRight = keyDown;
	else if( inputEvent.key.keysym.sym == SDLK_s )
		m_keyBck = keyDown;
	else if( inputEvent.key.keysym.sym == SDLK_LEFT )
		m_keyRotLeft = keyDown;
	else if( inputEvent.key.keysym.sym == SDLK_RIGHT )
		m_keyRotRight = keyDown;
}

void Player::Update( float dTime )
{
	// Movement vector
	Vector3f movement;
	float movementFactor = 24.0f * dTime;
	float rotFactor = 32.0f * dTime;
	float phi = (float)UtilPI / 2.0f;

	if(m_keyRotLeft)
	{
		m_facing += rotFactor;
	}
	if(m_keyRotRight)
	{
		m_facing -= rotFactor;
	}

	if(m_keyFwd)
	{
		movement += Vector3f( cos(m_facing), -sin(m_facing), 0.0f );
	}
	if(m_keyBck)
	{
		movement -= Vector3f( cos(m_facing), -sin(m_facing), 0.0f );
	}

	if(m_keyRight)
	{
		movement += Vector3f( cos(m_facing - phi), -sin(m_facing - phi), 0.0f );
	}
	if(m_keyLeft)
	{
		movement += Vector3f( cos(m_facing + phi), -sin(m_facing + phi), 0.0f );
	}

	m_worldPosition += movement * movementFactor;
}
