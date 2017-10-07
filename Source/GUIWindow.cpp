#include "GUIWindow.h"

#include <assert.h>

#include "Renderer.h"
#include "GUIRootWidget.h"

namespace SGUI
{
	Window::Window(const SGUI::Point& pos, const SGUI::Point& size, Widget* pParent /*= nullptr*/, const std::string& title /*= "Untitled"*/)
		: Widget(pParent),
		mTitle(title)
	{
		setPosition(pos);
		setSize(size);
	}

	void Window::Render(Renderer& renderer)
	{
		Point shadowOffset{ 2,2 };
		Rect rect{ Point{0,0}, mSize };
// 		Rect shadowRect{ Point{ 0,0 } - shadowOffset, mSize + shadowOffset };
// 		renderer.FillRect(shadowRect, Color{0,30});
		renderer.FillRect(rect, mMouseFocus ?mWindowFillFocused : mWindowFillUnfocused);

		/* Draw header */
		if (!mTitle.empty())
		{
			int hh = mWindowHeaderHeight;
			Rect headerRect{ Point{ 0,0 }, Point{mSize.x, hh} };

			renderer.FillRect(headerRect, Color{ 74, 255 });
		}

		Widget::Render(renderer);
	}

// 		nvgBeginPath(ctx);
// 		nvgRoundedRect(ctx, mPos.x(), mPos.y(), mSize.x(), hh, cr);
// 		nvgStrokeColor(ctx, mTheme->mWindowHeaderSepTop);
// 
// 		nvgSave(ctx);
// 		nvgIntersectScissor(ctx, mPos.x(), mPos.y(), mSize.x(), 0.5f);
// 		nvgStroke(ctx);
// 		nvgRestore(ctx);
// 
// 		nvgBeginPath(ctx);
// 		nvgMoveTo(ctx, mPos.x() + 0.5f, mPos.y() + hh - 1.5f);
// 		nvgLineTo(ctx, mPos.x() + mSize.x() - 0.5f, mPos.y() + hh - 1.5);
// 		nvgStrokeColor(ctx, mTheme->mWindowHeaderSepBot);
// 		nvgStroke(ctx);
// 
// 		nvgFontSize(ctx, 18.0f);
// 		nvgFontFace(ctx, "sans-bold");
// 		nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
// 
// 		nvgFontBlur(ctx, 2);
// 		nvgFillColor(ctx, mTheme->mDropShadow);
// 		nvgText(ctx, mPos.x() + mSize.x() / 2,
// 			mPos.y() + hh / 2, mTitle.c_str(), nullptr);
// 
// 		nvgFontBlur(ctx, 0);
// 		nvgFillColor(ctx, mFocused ? mTheme->mWindowTitleFocused
// 			: mTheme->mWindowTitleUnfocused);
// 		nvgText(ctx, mPos.x() + mSize.x() / 2, mPos.y() + hh / 2 - 1,
// 			mTitle.c_str(), nullptr);
// 	}
// 
// 	nvgRestore(ctx);

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
