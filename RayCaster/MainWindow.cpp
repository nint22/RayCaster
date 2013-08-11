/***************************************************************
 
 RayCaster - A Simple C++ Ray Caster Game
 Copyright 2013 Core S2 - See License.txt for details
 
***************************************************************/

#include "MainWindow.h"

#include "Utilities.h"
#include <math.h>

MainWindow::MainWindow()
	: m_window( NULL )
	, m_renderer( NULL )
	, m_windowSize( 800, 600 )
	, m_gameWorld( NULL )
	, m_player( NULL )
	, m_worldView( NULL )
	, m_minimapView( NULL )
{
	/*** 1. Graphics ***/

	// Initialize SDL itself
	UtilAssert( SDL_Init(SDL_INIT_VIDEO) >= 0, "Failed to initialize SDL" );

	// Initialize window
	m_window = SDL_CreateWindow("RayCaster Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowSize.x, m_windowSize.y, 0);
	UtilAssert( m_window != NULL, "Failed to create window" );

	// Initialize the renderer within the window
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	/*** 2. Game Logic ***/

	// Load the game's controller, and the two main rendering views
	m_gameWorld = new World( "DemoWorld.txt" );
	m_player = new Player( Vector3f(6.5f, 6.5f, 0.5f), 0.0f );

	m_worldView = new WorldView( m_gameWorld, m_player );
	m_minimapView = new MinimapView( Vector2i(10, 10), 8, m_gameWorld, m_player );
}

MainWindow::~MainWindow()
{
	if( m_minimapView != NULL )
		delete m_minimapView;

	if( m_worldView != NULL )
		delete m_worldView;

	if( m_player != NULL )
		delete m_player;

	if( m_gameWorld != NULL )
		delete m_gameWorld;

	if( m_renderer != NULL )
		SDL_DestroyRenderer( m_renderer );

	if( m_window != NULL )
		SDL_DestroyWindow( m_window );

	SDL_Quit();
}

void MainWindow::Run()
{
	// Target FPS
	static const float frameRate = 30.0f;
	static const float timePerFrame = 1.0f / frameRate;

	UtilHighresClock highresClock;
	float dTime = 0.0f;
	bool validUpdate = true;
	
	// Keep looping until update fails
	while( validUpdate )
	{
		highresClock.Start();
		{
			// 1. Update
			validUpdate = Update( dTime );
			
			// 2. Clear our back buffer and draw to it
			SDL_SetRenderDrawColor(m_renderer, 227, 243, 255, 255);
			SDL_RenderClear( m_renderer );
			Render( dTime );

			// 3. Present back-buffer that we've just drawn onto (flip)
			SDL_RenderPresent( m_renderer );

		}
		highresClock.Stop();

		dTime = highresClock.GetTime();

		// Sleep off any remaining time
		float timeLeft = max( 0.0f, timePerFrame - dTime );
		SDL_Delay( Uint32(timeLeft * 1000.0f) );
	}
}

bool MainWindow::Update(float dTime)
{
	// Check for exit conditions
	SDL_Event e;
	while( SDL_PollEvent(&e) > 0 )
	{
		// Pass the key event to any observers
		if( e.type == SDL_QUIT || (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE) )
			return false;
		else
			m_player->UpdateKeys( e );
	} 
	
	// Full player update
	m_player->Update( dTime );

	// All good!
	return true;
}

void MainWindow::Render(float dTime)
{
	// Render world
	m_worldView->Render( m_renderer, m_windowSize );

	// Render the minimap
	m_minimapView->Render( m_renderer );
}
