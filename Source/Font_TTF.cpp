#include "Font_TTF.h"

#include <SDL_ttf.h>


FontTTF::FontTTF()
{
}

FontTTF::~FontTTF()
{
	Release();
}

bool FontTTF::LoadFont(const char* file, int ptsize, SDL_Color textColor)
{
	m_pFont = TTF_OpenFont(file, ptsize);

	if (m_pFont == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", "Font not loaded", nullptr);
		m_ptsize = 0;
		return false;
	}

	m_ptsize = ptsize;
	m_textColor = textColor;
	m_height = TTF_FontHeight(m_pFont);
	m_ascent = TTF_FontAscent(m_pFont);
	m_lineskip = TTF_FontLineSkip(m_pFont);
	return true;
}

void FontTTF::Release()
{
	TTF_CloseFont(m_pFont);
	m_pFont = nullptr;
	m_ptsize = 0;
}

int FontTTF::SizeUTF8(const std::string& text, int& w, int& h, Uint32 wrapLength/* = 0*/) const
{
	// Returns: 0 on success with the ints pointed to by w and h set as appropriate, 
	// if they are not NULL. -1 is returned on errors, such as a glyph in the string not being found.
	if (wrapLength > 0)
		return TTF_SizeUTF8_Wrapped(m_pFont, text.c_str(), &w, &h, wrapLength);
	else
		return TTF_SizeUTF8(m_pFont, text.c_str(), &w, &h);
}
