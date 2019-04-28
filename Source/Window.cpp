#include "Window.h"


Window::Window()
{

}

Window::~Window()
{
	Release();
}

void Window::Release()
{
	m_Renderer.Release();

	// Destroy the window
	if (m_pWindow != nullptr)
	{
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}
}

bool Window::Create(std::string title, int x, int y, int w, int h, Uint32 flags)
{
	Release();

	m_pWindow = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

	if (m_pWindow == nullptr)
		return false;

	m_Scale = GetDisplayScaling();

	if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
		SDL_GetWindowSize(m_pWindow, &w, &h);
	else if (m_Scale != 1)
	{
		// Scales window size based on dpi scaling
		SetWindowSize(w, h);
	}

	m_Width = w;
	m_Height = h;

	m_bMouseFocus = true;
	m_bKeyboardFocus = true;

	//Grab window identifier 
	m_WindowID = SDL_GetWindowID(m_pWindow);

	//Flag as opened 
	m_bShown = true;

	return true;
}

bool Window::CreateRenderer(Uint32 iFlags /*= SDL_RENDERER_ACCELERATED*/)
{
	return m_Renderer.Create(*this, iFlags);
}

void Window::ClearWindow()
{
	m_Renderer.ClearBackBuffer();
}

void Window::Present()
{
	if (CanRender())
		m_Renderer.Present();
}

void Window::SetWindowSize(int width, int height)
{
	SDL_assert(m_pWindow);
	
	SDL_SetWindowSize(m_pWindow, static_cast<int>(width*m_Scale), static_cast<int>(height*m_Scale));

	m_Width = width;
	m_Height = height;
}

Point Window::GetWindowSize() const
{
	SDL_assert(m_pWindow);

	Point rSize{ 0,0 };
	SDL_GetWindowSize(m_pWindow, &rSize.x, &rSize.y);

	return rSize;
}

