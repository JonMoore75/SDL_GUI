#ifndef Font_TTF_h__
#define Font_TTF_h__
#pragma once

#include <string>
#include <SDL_ttf.h>

class FontTTF
{
public:
	FontTTF();
	~FontTTF() noexcept;

	bool LoadFont(const char* file, int ptsize, SDL_Color textColor = { 0x00, 0x00, 0x00, 0xFF });

	void Release();

	void ChangeFontSize(int ptsize) { m_ptsize = ptsize; }
	void ChangeColor(SDL_Color color) { m_textColor = color; }

	int GetHeight() const { return m_height; }
	int GetAscent() const { return m_ascent; }

	int SizeUTF8(const std::string& text, int& w, int& h) const;

	TTF_Font* GetFontPtr() const noexcept { return m_pFont; }
	const SDL_Color& GetColor() const noexcept { return m_textColor; }

private:
	TTF_Font* m_pFont = nullptr;
	int m_ptsize = 0;
	int m_height = 0;
	int m_ascent = 0;
	SDL_Color m_textColor = { 0x00, 0x00, 0x00, 0x00 };
};

#endif // Font_TTF_h__
