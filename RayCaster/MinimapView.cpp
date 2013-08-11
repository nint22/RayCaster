/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
***************************************************************/

#include "SDL.h"
#include "MinimapView.h"

MinimapView::MinimapView( Vector2i pos, int tileSize, const World* gameWorld, const Player* player )
	: m_gameWorld( gameWorld )
	, m_player( player )
	, m_minimapPos( pos )
	, m_minimapTileSize( tileSize )
{
}

MinimapView::~MinimapView()
{
}

void MinimapView::Render( SDL_Renderer* renderer )
{
	SDL_Rect rect;

	// For each tile in the game world
	Vector2i worldSize = m_gameWorld->GetWorldSize();
	for(int y = 0; y < worldSize.y; y++)
	for(int x = 0; x < worldSize.x; x++)
	{
		int tileId = m_gameWorld->GetWorldTile(x, y)->m_tileId;
		
		if(tileId == ' ')
			SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
		else
			SDL_SetRenderDrawColor( renderer, 32, 32, 32, 255 );
		
		rect.x = m_minimapPos.x + x * m_minimapTileSize;
		rect.y = m_minimapPos.y + y * m_minimapTileSize;
		rect.h = rect.w = m_minimapTileSize;

		SDL_RenderFillRect( renderer, &rect );
	}

	// Draw wherever the player is as a circle, then draw the facing vector
	Vector3f ppos = m_player->GetPosition();
	rect.x = (int)((float)m_minimapPos.x + ppos.x * (float)m_minimapTileSize - 1.0f);
	rect.y = (int)((float)m_minimapPos.y + ppos.y * (float)m_minimapTileSize - 1.0f);
	rect.h = rect.w = 3;

	SDL_SetRenderDrawColor( renderer, 0, 0, 128, 255 );
	SDL_RenderFillRect( renderer, &rect );

	// Draw a line for the facing-direction
	float facing = m_player->GetFacing();
	int dx = (int)(cos( facing ) * 8.0f);
	int dy = (int)(-sin( facing ) * 8.0f);
	
	SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
	SDL_RenderDrawLine( renderer, rect.x + 1, rect.y + 1, rect.x + dx + 1, rect.y + dy + 1 );
}
