#include "Texture.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "SDLErrorReport.h"
#include "Renderer.h"
#include "Font_TTF.h"

Texture::Texture()
{

}

Texture::~Texture()
{
	Release();
}

void Texture::Release()
{
	SDL_DestroyTexture(m_pTexture);
	m_pTexture = nullptr;
	m_Width = 0;
	m_Height = 0;
}

void Texture::Render(Renderer& renderer, int x, int y) const
{
	SDL_Rect image_rect = { x,y, m_ClipRect.w, m_ClipRect.h };
	RenderStretch(renderer, &image_rect);
}

void Texture::RenderStretch(Renderer& renderer, SDL_Rect* pDestRect /*= nullptr*/) const
{
	if (renderer.GetPtr() && m_pTexture)
		SDL_RenderCopy(renderer.GetPtr(), m_pTexture, &m_ClipRect, pDestRect);
}

bool Texture::Create(Renderer& renderer, Uint32 format, int access, int w, int h)
{
	Release();

	m_pTexture = SDL_CreateTexture(renderer.GetPtr(), format, access, w, h);

	if (m_pTexture == nullptr)
		MsgBoxErrorReport("Unable to create texture from surface!\n", SDL_GetError());
	else
	{
		//Get image dimensions 
		m_Width = w;
		m_Height = h;
		SetDefaultClipRect();
	}

	return m_pTexture != nullptr;
}

bool Texture::CreateFromFile(Renderer& renderer, std::string filename)
{
	SDL_Surface* pSurface = IMG_Load(filename.c_str());

	if (!pSurface)
	{
		// Failed to load from file
		std::string err_msg = "Could not load texture from file.\n";
		err_msg += filename + "\n";
		MsgBoxErrorReport(err_msg, SDL_GetError());
		return false;
	}
	else
		return CreateFromSurface(pSurface, renderer);

	return true;
}


bool Texture::CreateFromText(Renderer& renderer, std::string text, FontTTF& font, int wrapLength/* = 0*/)
{
	if (!font.GetFontPtr())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", "Font not loaded", nullptr);
		return false;
	}

	SDL_Surface* textSurface = (wrapLength > 0) ? 
		TTF_RenderUTF8_Blended_Wrapped(font.GetFontPtr(), text.c_str(), font.GetColor(), wrapLength)
		: TTF_RenderUTF8_Blended(font.GetFontPtr(), text.c_str(), font.GetColor());

	return CreateFromSurface(textSurface, renderer);
}

bool Texture::CreateFromText_Fast(Renderer& renderer, std::string text, FontTTF& font, int wrapLength/* = 0*/)
{
	if (!font.GetFontPtr())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", "Font not loaded", nullptr);
		return false;
	}

	SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font.GetFontPtr(), text.c_str(), font.GetColor());

	return CreateFromSurface(textSurface, renderer);
}

bool Texture::CreateFromSurface(SDL_Surface* textSurface, Renderer &renderer)
{
	Release();

	if (textSurface == nullptr)
	{
		MsgBoxErrorReport("Tried to create texture from invalid surface.\n", SDL_GetError());
		return false;
	}

	//Create texture from surface pixels 
	m_pTexture = SDL_CreateTextureFromSurface(renderer.GetPtr(), textSurface);
	if (m_pTexture == nullptr)
		MsgBoxErrorReport("Unable to create texture from surface!\n", SDL_GetError());
	else
	{
		//Get image dimensions 
		m_Width = textSurface->w;
		m_Height = textSurface->h;
		SetDefaultClipRect();
	}
	//Get rid of old surface 
	SDL_FreeSurface(textSurface);

	return m_pTexture != nullptr;
}