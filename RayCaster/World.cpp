/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
***************************************************************/

#include "World.h"
#include "Utilities.h"

World::World( const std::string& worldFileName )
	: m_worldMap( NULL )
	, m_worldSize(0, 0)
{
	// Open up the file
	FILE* file = fopen(worldFileName.c_str(), "r");
	UtilAssert(file != NULL, "Unable to load world file \"%s\"", worldFileName.c_str());

    // World size
    fscanf(file, "%d %d", &m_worldSize.x, &m_worldSize.y);
	
    // World buffer (N: characters in row, M: number of rows)
    m_worldMap = new WorldTile[ m_worldSize.x * m_worldSize.y ];
    for(int m = 0; m < m_worldSize.y; m++)
    for(int n = 0; n < m_worldSize.x; n++)
    {
        char temp = ' ';
        while( (temp = getc(file)) == '\n' );
        m_worldMap[m * m_worldSize.x + n].m_tileId = temp;
    }

	fclose(file);
}

World::~World()
{
	if( m_worldMap != NULL )
		delete[] m_worldMap;
}

const WorldTile* World::GetWorldTile( int x, int y ) const
{
	UtilAssert( x >= 0 && x < m_worldSize.x && y >= 0 && y < m_worldSize.y, "Out of bounds world-tile access" );
	return &(m_worldMap[y * m_worldSize.x + x]);
}
