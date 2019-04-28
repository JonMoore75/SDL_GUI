#ifndef _RENDERER_H_
#define _RENDERER_H_

#pragma once

#include <SDL2\SDL.h>
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

	Rect GetViewport() const;

	Point GetOutputSize() const;

	void RenderToTexture(Texture& texture);

	void RenderToFrameBuffer()
	{
		SDL_SetRenderTarget(m_pRenderer, nullptr);
	}
		
	bool Create(Window& window, Uint32 flags = SDL_RENDERER_ACCELERATED, int index = -1);

	void FillRect(const Rect& rect, const Color& color);
	void OutlineRect(const Rect& rect, const Color& color);
	void DrawLine(int x1, int y1, int x2, int y2, const Color& color);
	void DrawPoint(int x, int y, const Color& color);

	SDL_BlendMode SetRenderDrawMode(SDL_BlendMode blendMode);
		
	SDL_Renderer* GetPtr() const { return m_pRenderer; }
		
private:
	SDL_Renderer* m_pRenderer = nullptr;
	Color m_BackgroundColor = Color{ 0x00, 0x00, 0x00, 0xFF };
};

#endif _RENDERER_H_