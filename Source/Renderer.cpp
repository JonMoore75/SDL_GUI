#include "Renderer.h"

#include "Texture.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	Release();
}

void Renderer::Release()
{
	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = nullptr;
}

void Renderer::ClearBackBuffer()
{
	SDL_SetRenderDrawColor(m_pRenderer, m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
	SDL_RenderClear(m_pRenderer);
}

void Renderer::Present()
{
	if (m_pRenderer)
	{
		//Update screen 
		SDL_RenderPresent(m_pRenderer);

		//Clear screen ready for next frame 
		ClearBackBuffer();
	}
}

void Renderer::SetRenderTexture(Texture& texture)
{
	SDL_SetRenderTarget(m_pRenderer, texture.GetPtr());
}

bool Renderer::Create(SDL_Window* pWindow, Uint32 flags /*= SDL_RENDERER_ACCELERATED*/, int index /*= -1*/)
{
	if (!pWindow)
		return false;

	Release();

	m_pRenderer = SDL_CreateRenderer(pWindow, index, flags);
	if (m_pRenderer == nullptr)
		return false;

	ClearBackBuffer();

	return true;
}

void Renderer::FillRect(const SDL_Rect& rect, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(m_pRenderer, &rect);
}

void Renderer::OutlineRect(const SDL_Rect& rect, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(m_pRenderer, &rect);
}

void Renderer::Line(int x1, int y1, int x2, int y2, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void Renderer::Point(int x, int y, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(m_pRenderer, x, y);
}
