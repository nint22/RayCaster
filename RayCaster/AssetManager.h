/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
 This source file is developed and maintained by:
 + Jeremy Bridon jbridon@cores2.com
 
 File: AssetManager.cpp/h
 Desc: An asynchronous asset manager, which supports loading
 an asset for the run-time client. Implemented as singleton.

***************************************************************/

#ifndef __ASSETMANAGER_H__
#define __ASSETMANAGER_H__

#include "Utilities.h"
#include "SDL.h"

class AssetManager
{
public:

	// Access our singleton; will construct self if not yet build
	static const AssetManager* GetSingleton();

	// Get image asset
	virtual const void* GetImage( const std::string& fileName ) = NULL;

};

#endif
