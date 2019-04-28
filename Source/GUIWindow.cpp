#include "GUIWindow.h"

#include <assert.h>
#include <algorithm>

#include "Renderer.h"
#include "GUIRootWidget.h"
#include "GUITheme.h"

namespace SGUI
{
	Window::Window(Widget* parent, const Point& pos, const Point& size, const std::string& title /*= "Untitled"*/)
		: Widget{ parent }, mTitle{ title }
	{
		setPosition(pos);
		setSize(size);
	}

	void Window::Render(Renderer& renderer, Point& offset)
	{
		Point shadowOffset{ 2,2 };
		Rect rect{ offset, mSize };
// 		Rect shadowRect{ Point{ 0,0 } - shadowOffset, mSize + shadowOffset };
// 		renderer.FillRect(shadowRect, Color{0,30});
		renderer.FillRect(rect, mMouseFocus ? mTheme->mWindowFillFocused : mTheme->mWindowFillUnfocused);

		/* Draw header */
		if (!mTitle.GetText().empty())
		{
			int hh = mTheme->mWindowHeaderHeight;

			Rect headerRect{ offset, Point{mSize.x, hh} };

			renderer.FillRect(headerRect, mTheme->mWindowHeaderGradientTop);
			renderer.DrawLine(	offset.x,			offset.y, 
							offset.x + mSize.x, offset.y, 
							mTheme->mWindowHeaderSepTop);
			renderer.DrawLine(	offset.x,			offset.y + hh-1, 
							offset.x + mSize.x, offset.y + hh-1, 
							mTheme->mWindowHeaderSepBot);

			if (mTitle.NeedsCreation())
			{
				mTitle.Create(renderer, mTheme->mFontNormal, 18, mTheme->mWindowTitleUnfocused);
				mTitle.TextAlign(TextObject::CLIPCENTRE, Point{ mSize.x, hh });
			}

			mTitle.Render(renderer, offset);		
		}

		Widget::Render(renderer, offset);
	}

	Point Window::preferredSize(Renderer& renderer) const
	{
		Point result = Widget::preferredSize(renderer);
		Point textBounds = TextBounds(mTheme->mFontNormal, 18, mTitle.GetText());

		result.x = std::max(result.x, textBounds.x + 20);
		result.y = std::max(result.y, textBounds.y);

		return result;
	}

	bool Window::mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers)
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

	bool Window::mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers)
	{
		if (Widget::mouseButtonEvent(p, button, down, modifiers))
			return true;
		if (button == MouseBut::LEFT) 
		{
			mDrag = down && (p.y - mPos.y) < mTheme->mWindowHeaderHeight;
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
