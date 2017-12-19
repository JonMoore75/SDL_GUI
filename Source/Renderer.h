#ifndef _RENDERER_H_
#define _RENDERER_H_

#pragma once

#include <SDL.h>
#include "Common.h"

class Texture;
class Window;

class Renderer
{
public:
	Renderer();
	~Renderer() noexcept;
		
	void Release();
		
	void ClearBackBuffer();
		
	void Present();

	void SetViewport(const Rect& rect) { SDL_RenderSetViewport(m_pRenderer, &rect); }
	void SetViewportWholeScreen() { SDL_RenderSetViewport(m_pRenderer, nullptr); }

	Rect GetViewport() const
	{ 
		Rect retRect{0,0,0,0};
		SDL_RenderGetViewport(m_pRenderer, &retRect); 
		return retRect;
	}

	void SetRenderTexture(Texture& texture);

	void SetRenderFrameBuffer()
	{
		SDL_SetRenderTarget(m_pRenderer, nullptr);
	}
		
	bool Create(Window& window, Uint32 flags = SDL_RENDERER_ACCELERATED, int index = -1);

	void FillRect(const Rect& rect, const Color& color);
	void OutlineRect(const Rect& rect, const Color& color);
	void Line(int x1, int y1, int x2, int y2, const Color& color);
	void Point(int x, int y, const Color& color);

	SDL_BlendMode SetRenderDrawMode(SDL_BlendMode blendMode)
	{
		SDL_BlendMode oldBlendMode;
		SDL_GetRenderDrawBlendMode(m_pRenderer, &oldBlendMode);
		SDL_SetRenderDrawBlendMode(m_pRenderer, blendMode);

		return oldBlendMode;
	}
		
	SDL_Renderer* GetPtr() const { return m_pRenderer; }
		
private:
	SDL_Renderer* m_pRenderer = nullptr;
	Color m_BackgroundColor = Color{ 0x00, 0x00, 0x00, 0xFF };
};

#endif _RENDERER_H_