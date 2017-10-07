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
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
}

bool Window::Create(std::string title, int x, int y, int w, int h, Uint32 flags)
{
	Release();

	m_pWindow = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);

	if (m_pWindow == nullptr)
		return false;

	if (flags | SDL_WINDOW_FULLSCREEN_DESKTOP)
		SDL_GetWindowSize(m_pWindow, &w, &h);

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
	return m_Renderer.Create(m_pWindow, iFlags);
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

