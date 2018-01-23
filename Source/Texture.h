#ifndef Texture_h__
#define Texture_h__
#pragma once

#include <string>
#include <memory>
#include <vector>

#include <SDL.h>

#include "Common.h"

class Renderer;
class FontTTF;

class Texture
{
public:
	Texture();
	~Texture() noexcept;

	void Release();
	SDL_Texture* GetPtr() { return m_pTexture; }
	bool isNull() const { return m_pTexture == nullptr; }

	void Render(Renderer& renderer, Point pos) const;
	void RenderStretch(Renderer& renderer, Rect* pDestRect = nullptr) const;

	void SetClipRect(const Rect& rect) { m_ClipRect = rect; }
	void SetDefaultClipRect() { m_ClipRect = Rect{0,0,m_Width,m_Height}; }

	bool Create(Renderer& renderer, Uint32 format, int access, int w, int h);

	bool CreateFromFile(Renderer& renderer, std::string filename);

	bool CreateFromText(Renderer& renderer, std::string text, FontTTF& font, int wrapLength=0);
//	bool CreateFromTextWrapped(Renderer& renderer, std::string text, FontTTF& font, int wrapLength);

	bool CreateFromText_Fast(Renderer& renderer, std::string text, FontTTF& font, int wrapLength=0);

	bool CreateFromSurface(SDL_Surface* textSurface, Renderer &renderer);

	void SetAlphaBlend() { SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND); }
	void SetAlpha(Uint8 alpha) { SDL_SetTextureAlphaMod(m_pTexture, alpha); }

	int GetHeight() const { return m_Height; }
	int GetWidth() const { return m_Width; }

private:
	SDL_Texture*	m_pTexture = nullptr;
	Rect			m_ClipRect{0,0,0,0};
	int m_Width = 0;
	int m_Height = 0;
};

#endif // Texture_h__
