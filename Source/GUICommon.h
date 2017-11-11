#ifndef GUICommon_h__
#define GUICommon_h__
#pragma once

#include <string>
#include <SDL.h>

class Renderer;

namespace SGUI
{
	class Point : public SDL_Point
	{
	public:
		Point(int _x = 0, int _y = 0) { x = _x; y = _y; }
		~Point() {}

		Point& operator+=(const Point& right)
		{
			x += right.x;
			y += right.y;

			return *this;
		}

		bool isZero() const
		{
			return (x == 0 && y == 0);
		}

		bool notZero() const
		{
			return !isZero();
		}

		void ClampMin(const Point& minLimit)
		{
			if (x < minLimit.x)
				x = minLimit.x;

			if (y < minLimit.y)
				y = minLimit.y;
		}

		void ClampMax(const Point& maxLimit)
		{
			if (x > maxLimit.x)
				x = maxLimit.x;

			if (y > maxLimit.y)
				y = maxLimit.y;
		}
	};

	class Rect : public SDL_Rect
	{
	public:
		Rect(int _x = 0, int _y = 0, int _w = 0, int  _h = 0) { x = _x; y = _y; w = _w; h = _h;  }
		Rect(const Point& pos, const Point& size) { x = pos.x; y = pos.y; w = size.x; h = size.y; }

		Point getPosition() const { return Point{x,y}; }
		Point getSize() const { return Point{w,h}; }
		~Rect() {}
	};

	class Color : public SDL_Color
	{
	public:
		Color() { r = 0; g = 0; b = 0; a = 1; }
		Color(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a ) { r = _r; g = _g; b = _b; a = _a; }
		Color(Uint8 l, Uint8 _a) { r = l; g = l; b = l; a = _a; }
		~Color() {}
	};

	int Decorate(Renderer& renderer, const SGUI::Rect& rect, const SGUI::Color& color, const int edge);
}

SGUI::Point operator+(const SGUI::Point& lhs, const SGUI::Point& rhs);
SGUI::Point operator-(const SGUI::Point& lhs, const SGUI::Point& rhs);

SGUI::Rect operator+(const SGUI::Rect& lhs, const SGUI::Point& rhs);

bool PointinRect(const SGUI::Point& point, const SGUI::Rect& rect);
bool PointinRect(int x, int y, const SGUI::Rect& rect);

SGUI::Point TextBounds(const std::string& fontName, int fontSize, const std::string& text);

#endif // GUICommon_h__



