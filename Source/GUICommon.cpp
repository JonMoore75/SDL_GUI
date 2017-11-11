#include "GUICommon.h"

#include "Font_TTF.h"
#include "Renderer.h"

namespace SGUI
{
	int Decorate(Renderer& renderer, const SGUI::Rect& rect, const SGUI::Color& color, const int edge)
	{
		int d = 2 * edge;

		if (rect.w < d + 6 || rect.h < d + 6)
			return -1;

		for (int i = 0; i < 2; i++)
		{
			SGUI::Rect outlinerect{
				rect.x + edge + i,
				rect.y + edge + i,
				rect.w - d - i - i,
				rect.h - d - i - i };

			renderer.OutlineRect(outlinerect, color);
		}
		return 0;
	}
}

SGUI::Point operator+(const SGUI::Point& lhs, const SGUI::Point& rhs)
{
	return SGUI::Point{ lhs.x + rhs.x, lhs.y + rhs.y };
}

SGUI::Rect operator+(const SGUI::Rect& lhs, const SGUI::Point& rhs)
{
	return SGUI::Rect{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.w, lhs.h };
}

SGUI::Point operator-(const SGUI::Point& lhs, const SGUI::Point& rhs)
{
	return SGUI::Point{ lhs.x - rhs.x, lhs.y - rhs.y };
}

bool PointinRect(const SGUI::Point& point, const SGUI::Rect& rect)
{
	return (SDL_PointInRect(&point, &rect) == SDL_TRUE);
}

bool PointinRect(int x, int y, const SGUI::Rect& rect)
{
	SGUI::Point point{ x,y };
	return PointinRect(point, rect);
}

SGUI::Point TextBounds(const std::string& fontName, int fontSize, const std::string& text)
{
	SGUI::Point textSize{ -1, -1 };
	FontTTF fontTTF;
	fontTTF.LoadFont(fontName.c_str(), fontSize, SGUI::Color{});
	fontTTF.SizeUTF8(text, textSize.x, textSize.y);
	fontTTF.Release();

	return textSize;
}

