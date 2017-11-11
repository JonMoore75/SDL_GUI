#include "GUIWindow.h"

#include <assert.h>
#include <algorithm>

#include "Renderer.h"
#include "GUIRootWidget.h"

namespace SGUI
{
	Window::Window(Widget* parent, const SGUI::Point& pos, const SGUI::Point& size, const std::string& title /*= "Untitled"*/)
		: Widget{ parent }, mTitle{ title }
	{
		setPosition(pos);
		setSize(size);
	}

	void Window::Render(Renderer& renderer, Point& offset)
	{
		Point shadowOffset{ 2,2 };
		Rect rect{ Point{0,0}, mSize };
// 		Rect shadowRect{ Point{ 0,0 } - shadowOffset, mSize + shadowOffset };
// 		renderer.FillRect(shadowRect, Color{0,30});
		renderer.FillRect(rect, mMouseFocus ?mWindowFillFocused : mWindowFillUnfocused);

		/* Draw header */
		if (!mTitle.GetText().empty())
		{
			int hh = mWindowHeaderHeight;
			Color mWindowHeaderGradientTop{ 74, 255 };
			Color mWindowHeaderSepTop{ 92, 255 };
			Color mWindowHeaderSepBot{ 29, 255 };
			Color mDropShadow{ 0, 128 };
			Color mWindowTitleUnfocused{ 220, 160 };
			Color mWindowTitleFocused{ 255, 190 };

			Rect headerRect{ Point{ 0,0 }, Point{mSize.x, hh} };

			renderer.FillRect(headerRect, mWindowHeaderGradientTop);
			renderer.Line(0, 0, mSize.x, 0, mWindowHeaderSepTop);
			renderer.Line(0, hh-1, mSize.x, hh-1, mWindowHeaderSepBot);

			if (mTitle.NeedsCreation())
			{
				mTitle.Create(renderer, "Boku2-Regular.otf", 18, mWindowTitleUnfocused);
				mTitle.TextAlign(TextObject::CLIPCENTRE, Point{ mSize.x, hh });
			}

			mTitle.Render(renderer);		
		}

		Widget::Render(renderer, offset);
	}

	SGUI::Point Window::preferredSize(Renderer& renderer) const
	{
		Point result = Widget::preferredSize(renderer);
		Point textBounds = TextBounds("Boku2-Regular.otf", 18, mTitle.GetText());

		result.x = std::max(result.x, textBounds.x + 20);
		result.y = std::max(result.y, textBounds.y);

		return result;
	}

	bool Window::mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, SDL_Keymod modifiers)
	{
		if (mDrag && buttons.mLeft )
		{
			mPos += rel;
			mPos.ClampMin(Point{ 0,0 });
			assert( dynamic_cast<RootWidget*>(parent()) );
			assert(parent()->size().notZero());
			mPos.ClampMax(parent()->size() - mSize);
			return true;
		}
		return false;
	}

	bool Window::mouseButtonEvent(const Point& p, MouseBut button, bool down, SDL_Keymod modifiers)
	{
		if (Widget::mouseButtonEvent(p, button, down, modifiers))
			return true;
		if (button == MouseBut::LEFT) 
		{
			mDrag = down && (p.y - mPos.y) < mWindowHeaderHeight;
			return true;
		}
		return false;
	}

	bool Window::scrollEvent(const Point& p, const Point& rel)
	{
		Widget::scrollEvent(p, rel);
		return true;
	}
}
