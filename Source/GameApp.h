#ifndef GameApp_h__
#define GameApp_h__
#pragma once

#include <memory>
#include <map>
#include <functional>

#include "Window.h"
#include "TimeKeeper.h"

struct WindowCreationParams
{
								// Resolution, only applies for windowed not fullscreen
	int iWidth = 1024;			//< Required width of the window.
	int iHeight = 768;			//< Required height of the window.

								// Window Params
	bool bFullscreen = false;	//< Run full-screen.
	bool Resizeable = false;	//< Whether window can be resized
	bool bOpenGL = false;		//< Create OpenGL context
	bool bBorderless = false;	//< Create window with no border
	bool bMouseGrab = false;	//< Limits mouse within bounds of window
	bool bMouseCapture = false;	//< Captures the mouse input

									// Renderer Params
	bool bSoftwareRender = false;	//< Whether use software rendering instead of GPU
	bool bVSync = false;			//< Whether to wait for Vsync or present immediately to the user.
	bool bTextureRender = false;	//< Renderer can render to a texture as well as screen

	Uint32 SetRendererCreateFlags();

	Uint32 SetWindowCreateFlags();
};

class GameState;


template<typename ClassType> 
std::unique_ptr<GameState> CreateObject()
{                                                                                                        
	return std::make_unique<ClassType>();
}


class StateFactory
{
public:
	typedef std::function<std::unique_ptr<GameState>()> CreateObjectFunc;
	typedef std::map<std::string, CreateObjectFunc>::const_iterator ConstIterator;
	typedef std::map<std::string, CreateObjectFunc>::iterator Iterator;

	template<typename ClassType>                                                                          
	bool Register(std::string unique_id)
	{                                                                                                     
		if (m_object_creator.find(unique_id) != m_object_creator.end())                                   
			return false;                                                                                   
			
			m_object_creator[unique_id] = &CreateObject<ClassType>; 
			
			return true;                                                                                       
	}


	std::unique_ptr<GameState> Create(std::string unique_id)
	{                                                                                                    
		Iterator iter = m_object_creator.find(unique_id);                                                  
		
		SDL_assert(iter != m_object_creator.end()); 

		std::function<std::unique_ptr<GameState>()> createFunc = iter->second;
			
		return createFunc();
	}
private:
	std::map<std::string, CreateObjectFunc> m_object_creator;
};

class GameApp : public StateFactory
{
public:
	GameApp(std::string appname);
	virtual ~GameApp() noexcept;

	int Execute(WindowCreationParams& createParam, std::string initial_state);

	Window& GetWindow() { return m_Window; }
	bool IsRunning() const noexcept { return m_Running; }
	void AppQuit() noexcept { m_Running = false; }

	bool ChangeState(std::unique_ptr<GameState> new_state);

protected:

	void Cleanup();

	bool Init(WindowCreationParams& createParam, std::string initial_state);

	void HandleEvents();

	void MainLoop();

	void Render();

	void DrawFramesPerSecond();

protected:
	std::string m_AppName;
	bool	m_Running = false;

	bool	m_ShowFPS = true;

	Window	m_Window;

	TimeKeeper m_Timer;

	std::unique_ptr<GameState>		m_pState;
};



#endif // GameApp_h__
