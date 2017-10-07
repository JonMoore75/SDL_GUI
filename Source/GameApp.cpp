#include "GameApp.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>

#include "SDLErrorReport.h"
#include "GameState.h"

Uint32 WindowCreationParams::SetRendererCreateFlags()
{
	Uint32 iFlags = bSoftwareRender ? SDL_RENDERER_SOFTWARE : SDL_RENDERER_ACCELERATED;

	if (bVSync)
		iFlags |= SDL_RENDERER_PRESENTVSYNC;
	if (bTextureRender)
		iFlags |= SDL_RENDERER_TARGETTEXTURE;

	return iFlags;
}

Uint32 WindowCreationParams::SetWindowCreateFlags()
{
	Uint32 iFlags = SDL_WINDOW_SHOWN;
	if (Resizeable)
		iFlags |= SDL_WINDOW_RESIZABLE;
	if (bFullscreen)
		iFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	if (bOpenGL)
		iFlags |= SDL_WINDOW_OPENGL;
	if (bBorderless)
		iFlags |= SDL_WINDOW_BORDERLESS;
	if (bMouseGrab)
		iFlags |= SDL_WINDOW_INPUT_GRABBED;
	if (bMouseCapture)
		iFlags |= SDL_WINDOW_MOUSE_CAPTURE;
	return iFlags;
}

GameApp::GameApp(std::string appname) : m_AppName(appname)
{
	
}

GameApp::~GameApp()
{
	Cleanup();
}

void GameApp::Cleanup()
{
	if (m_pState)
		m_pState->CleanUp();

	m_Window.Release();

	// Shutdown SDL
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool GameApp::ChangeState(std::unique_ptr<GameState> new_state)
{
	m_pState = std::move(new_state);
	SDL_assert(m_pState);

	m_pState->SetWindow(&m_Window);

	return (m_pState.get() != nullptr);
}

bool GameApp::Init(WindowCreationParams& createParam, std::string initial_state)
{
	// Initialise SDL, report error if it fails
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		Error2MsgBox("SDL Initialisation Failed!\n");
		return false;
	}

	//Initialize PNG/JPG loading 
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		std::string err_msg = "SDL_image could not initialize!\n";
		err_msg += IMG_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", err_msg.c_str(), nullptr);
		return false;
	}

	//Initialize SDL_ttf 
	if (TTF_Init() == -1)
	{
		std::string err_msg = "SDL_ttf could not initialize!\n";
		err_msg += TTF_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", err_msg.c_str(), nullptr);
		return false;
	}

	//Initialize SDL_mixer 
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) 
	{ 
		std::string err_msg = "SDL_mixer could not initialize!\n";
		err_msg += Mix_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", err_msg.c_str(), nullptr);
		return false; 
	}

	// Create a window, report error if window not created
	if (!m_Window.Create("Test Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		createParam.iWidth, createParam.iHeight,
		createParam.SetWindowCreateFlags()))
	{
		Error2MsgBox("Window Creation Failed.\n");
		return false;
	}

	// Creates a renderer and clears the window
	if (!m_Window.CreateRenderer( createParam.SetRendererCreateFlags() ))
	{
		Error2MsgBox("Renderer Creation Failed.\n");
		return false;
	}

	m_Timer.Initialize();

	ChangeState( Create(initial_state) );
	SDL_assert(m_pState);

	return m_pState->Initialise();
}



void GameApp::HandleEvents()
{
	SDL_Event Event;

	while (SDL_PollEvent(&Event))
	{
		// If quit has been requested (eg clicking close button) then query user for confirmation
		// via dialog box.  Then react to users choice
		if (Event.type == SDL_QUIT)
		{
			GameState::QUITRESPONSE response = m_pState->QuitDialog();

			switch (response)
			{
			case GameState::QUIT:
				AppQuit();
				return;
			case GameState::NEWSTATE:
				return;
			}

		}

		// If the window handled the event the return else pas to the state
		if (Event.type == SDL_WINDOWEVENT && Event.window.windowID == m_Window.GetID())
			if (m_Window.OnEvent(Event))
				return;

		m_pState->OnEvent(Event);
	}
}

void GameApp::MainLoop()
{
	m_Running = true;

	// Application will indicate quit by setting m_Running to false
	// until then we loop thro event handling and rendering each frame
	while (m_Running)
	{
		HandleEvents();

		if (m_Timer.Update())
		{
			// Gets time since last frame
			double deltaTime = m_Timer.GetDeltaTime();

			std::string new_state = m_pState->ExtractNextState();

			if (!new_state.empty())
			{
				ChangeState( Create(new_state) );
				m_pState->Initialise();
			}

			// Update the state
			m_pState->Update(deltaTime);

			if (m_ShowFPS)
				DrawFramesPerSecond();

		}

		// Draw our frame
		Render();
	}
}

void GameApp::Render()
{
	// If we have valid window & renderer then render the frame
	if (m_Window.CanRender())
	{
		m_pState->Render(m_Window.GetRenderer());
		m_Window.Present();
	}
}

int GameApp::Execute(WindowCreationParams& createParam, std::string initial_state)
{
	// Initialise SDL and create window
	if ( !Init(createParam, initial_state) )
		return -1;

	MainLoop();

	Cleanup();

	return 0;
}

void GameApp::DrawFramesPerSecond()
{
	std::stringstream strm;
	strm << m_AppName << "--Frames Per Second = " << round(m_Timer.GetFrameRate());
	strm << ", missed " << m_Timer.GetMissedFrames() << " frames";

	// Now set the new caption to the main window.
	m_Window.SetTitle(strm.str());
}