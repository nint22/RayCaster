/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
***************************************************************/

#include "AssetManager.h"

class AssetManager_Impl : public AssetManager
{
public:

	const void* GetImage( const std::string& fileName )
	{
		// Todo: implement the dictionary look-up
		return NULL;
	}

};

static const AssetManager* GetSingleton()
{
	static AssetManager_Impl* singleton = NULL;

	if( singleton == NULL )
	{
		singleton = new AssetManager_Impl();
	}

	return singleton;
}
