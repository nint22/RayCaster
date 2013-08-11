/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: Entity.cpp/h
 Desc: A simple renderable entity; has a sprite (image) and position
 in the game world. Derive from this class for entity-rendering.

***************************************************************/

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Utilities.h"
#include "VectorMath.h"

class Entity
{
public:

	// Initialize with position and sprite image
	Entity( const std::string& spriteFileName, const Vector3f& worldPosition );
	~Entity();

	// Get / set position
	void SetPosition( const Vector3f& worldPosition ) { m_worldPosition = worldPosition; }
	const Vector3f GetPosition() { return m_worldPosition; }

protected:

	Vector3<float> m_worldPosition;

};

#endif
