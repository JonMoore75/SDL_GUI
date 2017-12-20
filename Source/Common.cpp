#include "Common.h"

#include "Font_TTF.h"
#include "Renderer.h"

Point operator+(const Point& lhs, const Point& rhs)
{
	return Point{ lhs.x + rhs.x, lhs.y + rhs.y };
}

Rect operator+(const Rect& lhs, const Point& rhs)
{
	return Rect{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.w, lhs.h };
}

Point operator-(const Point& lhs, const Point& rhs)
{
	return Point{ lhs.x - rhs.x, lhs.y - rhs.y };
}

bool PointinRect(const Point& point, const Rect& rect)
{
	return (SDL_PointInRect(&point, &rect) == SDL_TRUE);
}

bool PointinRect(int x, int y, const Rect& rect)
{
	Point point{ x,y };
	return PointinRect(point, rect);
}

Point TextBounds(const std::string& fontName, int fontSize, const std::string& text, Uint32 wrapLength/* = 0*/)
{
	Point textSize{ -1, -1 };
	FontTTF fontTTF;
	fontTTF.LoadFont(fontName.c_str(), fontSize, Color{});
	fontTTF.SizeUTF8(text, textSize.x, textSize.y, wrapLength);
	fontTTF.Release();

	return textSize;
}

void StartUnicodeEvents()
{
	SDL_StartTextInput();
}

void StopUnicodeEvents()
{
	SDL_StopTextInput();
}

std::string GetClipboardText()
{
	char* cbstr{ SDL_GetClipboardText() };
	std::string ret{ cbstr };
	SDL_free(cbstr);
	return ret;
}

bool SetClipboardText(const std::string& str)
{
	return SDL_SetClipboardText(str.c_str()) >= 0;
}

