/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
***************************************************************/

#include "WorldView.h"

WorldView::WorldView( const World* world, const Player* player )
	: m_gameWorld( world )
	, m_player( player )
{
}

WorldView::~WorldView()
{
}

void WorldView::Render( SDL_Renderer* renderer, Vector2i windowSize )
{
	SDL_Rect rect;

	// Important properties for how we render our ray-casted world
	static float columnCount = 300.0f;
	static float fieldOfView = 1.2f; // In radians (130 degrees)

	// For each column ray we want to cast..
	for(int x = 0; x < columnCount; x++)
	{
		// What is the theta offset?
		float normx = (float)x / columnCount;
		float thetaOffset = (0.5f - normx) * fieldOfView;

		// Compute our casting ray
		float rayTheta = m_player->GetFacing() + thetaOffset;
		Vector3f sourcePos = m_player->GetPosition();
		Vector3f collisionPos = CollisionCheck( sourcePos, rayTheta );
		float dist = (float)(collisionPos - sourcePos).GetLength() * cos(thetaOffset); // Correction for fish-eye lense effect
		float wallHeight = (3.0f / dist) * (windowSize.y / 2.0f);

		// What is the x-axis pixel range?
		float px0 = normx * (float)windowSize.x;
		float px1 = px0 + windowSize.x / columnCount;

		// Render the pixels column
		rect.w = (int)(px1 - px0 + 1.0f);
		rect.h = (int)wallHeight;
		rect.x = (int)px0;
		rect.y = (int)(windowSize.y / 2.0f - rect.h / 2.0f);
		
		SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
}

Vector3f WorldView::CollisionCheck( Vector3f origin, float radians )
{
	// Starting position
	float x = origin.x;
	float y = origin.y;
	int worldWidth = m_gameWorld->GetWorldSize().x;
	int worldHeight = m_gameWorld->GetWorldSize().y;

    // A ray will always move towards one or two surfaces, like if
    // a ray is defined with R of zero, then it always moves to the right,
    // and thus we should only search right of us
    bool goingRight = cos(radians) >= 0.0f; // X+
    bool goingDown = (-sin(radians)) >= 0.0f; // Y+
    float slope = (-sin(radians)) / cos(radians);
    float b = y - slope * x;
	
    // Delta-fudge value; used to bump us off for the next test
    // Specificly it is used as a "look ahead" to see what we're colliding into
    float df = 0.00001f;
    float dfx = cos(radians) * df;
    float dfy = -sin(radians) * df;
	
    // Keep moving one unit-length ahead until we collide with something
    bool collisionFound = false;
    while( x > 0.0f && y > 0.0f && x <= worldWidth && y <= worldHeight && !collisionFound )
    {
        // What is the Y position if we check the block right/left of us
        float tx = goingRight ? ceil(x + df) : floor(x - df);
        float nextY = slope * tx + b;
		
        // What is the X position if we check the block above/below us
        float ty = goingDown ? ceil(y + df) : floor(y - df);
        float nextX = (ty - b) / slope;
		
        // What's closest? (Overkill, we can just use street/hamming-dist)
        float dx = sqrt( pow(tx - x, 2.0f) + pow(nextY - y, 2.0f) );
        float dy = sqrt( pow(nextX - x, 2.0f) + pow(ty - y, 2.0f) );
		
        if( dx < dy )
        {
            x = tx;
            y = nextY;
        }
        else
        {
            x = nextX;
            y = ty;
        }
		
        // Collision check
		int tileId = m_gameWorld->GetWorldTile( (int)(x + dfx), (int)(y + dfy) )->m_tileId;
		collisionFound = (tileId != ' ');
    }
	
    // Echo off results
    if(collisionFound)
    {
		return Vector3f( x, y, origin.z );
    }
    else
    {
		return Vector3f(0.0f, 0.0f, 0.0f);
    }
}
