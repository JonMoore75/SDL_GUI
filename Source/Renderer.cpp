#include "Renderer.h"

#include <SDL2_gfxPrimitives.h>

#include "Texture.h"
#include "Window.h"

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

Rect Renderer::GetViewport() const
{
	Rect retRect{ 0,0,0,0 };
	SDL_RenderGetViewport(m_pRenderer, &retRect);
	return retRect;
}

void Renderer::RenderToTexture(Texture& texture)
{
	SDL_SetRenderTarget(m_pRenderer, texture.GetPtr());
}

bool Renderer::Create(Window& window, Uint32 flags /*= SDL_RENDERER_ACCELERATED*/, int index /*= -1*/)
{
	if (window.IsNull())
		return false;

	Release();

	m_pRenderer = SDL_CreateRenderer(window.GetPtr(), index, flags);
	if (m_pRenderer == nullptr)
		return false;

	ClearBackBuffer();

	return true;
}

void Renderer::FillRect(const Rect& rect, const Color& color)
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(m_pRenderer, &rect);
}

void Renderer::OutlineRect(const Rect& rect, const Color& color)
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(m_pRenderer, &rect);
}

void Renderer::Line(int x1, int y1, int x2, int y2, const Color& color)
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void Renderer::Point(int x, int y, const Color& color)
{
	SDL_SetRenderDrawColor(m_pRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(m_pRenderer, x, y);
}

SDL_BlendMode Renderer::SetRenderDrawMode(SDL_BlendMode blendMode)
{
	SDL_BlendMode oldBlendMode;
	SDL_GetRenderDrawBlendMode(m_pRenderer, &oldBlendMode);
	SDL_SetRenderDrawBlendMode(m_pRenderer, blendMode);

	return oldBlendMode;
}
