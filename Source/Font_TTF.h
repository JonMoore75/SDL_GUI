#ifndef Font_TTF_h__
#define Font_TTF_h__
#pragma once

#include <string>
#include <SDL2\SDL_ttf.h>

#include "SDL_TTF_utf8wrapsize.h"
#include "Common.h"

class FontTTF
{
public:
	FontTTF();
	~FontTTF() noexcept;

	bool LoadFont(const char* file, int ptsize, Color textColor = { 0x00, 0x00, 0x00, 0xFF });

	void Release();

	void ChangeFontSize(int ptsize) { m_ptsize = ptsize; }
	void ChangeColor(Color color) { m_textColor = color; }

	int GetHeight() const { return m_height; }
	int GetAscent() const { return m_ascent; }
	int GetLineSkip() const { return m_lineskip; }

	int SizeUTF8(const std::string& text, int& w, int& h, Uint32 wrapLength=0) const;

	TTF_Font* GetFontPtr() const noexcept { return m_pFont; }
	const Color& GetColor() const noexcept { return m_textColor; }

private:
	TTF_Font* m_pFont = nullptr;
	int m_ptsize = 0;
	int m_height = 0;
	int m_ascent = 0;
	int m_lineskip = 0;
	Color m_textColor = { 0x00, 0x00, 0x00, 0x00 };
};

#endif // Font_TTF_h__
