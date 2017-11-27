#pragma once

#include <functional>

#include "Texture.h"
#include "Font_TTF.h"

#include "GUICommon.h"
#include "GUIWidget.h"
#include "GUIWindow.h"
#include "GUITheme.h"
#include "TimeKeeper.h"

namespace SGUI
{
	class RootWidget : public Widget
	{
	public:
		RootWidget() 
		{ 
			mTimer.Initialize(); 
			SDL_StartTextInput();
			setTheme(new Theme());
		}
		~RootWidget() { SDL_StopTextInput();}

		virtual void Render(Renderer& renderer, Point& offset) override
		{
			SDL_BlendMode oldMode = renderer.SetRenderDrawMode(SDL_BLENDMODE_BLEND);

			// Draw the widget tree
			Widget::Render(renderer, offset);

			// If there is a widget under the mouse with a tooltip, draw tootip after 0.5 secs
 			double elapsed =  mTimer.GetCurrentTime() - mLastInteraction;
 			if (elapsed > 0.5f && mFocusWidget && !mFocusWidget->tooltip().empty())
 				DrawFocusedWidgetTooltip(renderer, elapsed);

			renderer.SetRenderDrawMode(oldMode);
		}

		virtual bool mouseButtonEvent(const Point& p, MouseBut button, bool down, SDL_Keymod modifiers) override
		{
			auto dropWidget = findWidget(p);
			if (mDragActive && !down && dropWidget != mDragWidget)
				mDragWidget->mouseButtonEvent( p, button, down, modifiers);

			if (down && (button == LEFT || button == RIGHT)) 
			{
				mDragWidget = findWidget(p);
				if (mDragWidget == this)
					mDragWidget = nullptr;
				mDragActive = (mDragWidget != nullptr);
				if (!mDragActive)
					updateFocus(nullptr);
			}
			else 
			{
				mDragActive = false;
				mDragWidget = nullptr;
			}

			return Widget::mouseButtonEvent(p, button, down, modifiers);
		}

		void updateFocus(Widget* widget) 
		{
			for (auto w : mFocusPath) 
			{
				if (!w->focused())
					continue;
				w->focusEvent(false);
			}
			mFocusPath.clear();
			Window* window = nullptr;
			while (widget) 
			{
				mFocusPath.push_back(widget);
				window = dynamic_cast<Window*>(widget);
				widget->focusEvent(true);
				widget = widget->parent();
			}

// 			if (window)
// 				moveWindowToFront(window);
		}

		bool keyboardEvent(SDL_Scancode scan, SDL_Keycode key, bool down, SDL_Keymod modifiers) override
		{
			if (mFocusPath.size() > 0) 
			{
				for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
					if ((*it)->focused() && (*it)->keyboardEvent(scan, key, down, modifiers))
						return true;
			}

			return false;
		}

		bool keyboardCharacterEvent(const std::string& codepoint) override
		{
			if (mFocusPath.size() > 0) {
				for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
					if ((*it)->focused() && (*it)->keyboardCharacterEvent(codepoint))
						return true;
			}
			return false;
		}


		bool mouseMotionEvent(const Point& p, const Point& rel, MouseButStatus buttons, SDL_Keymod modifiers) override
		{
			bool ret = false;
			mLastInteraction = mTimer.GetCurrentTime();

			Widget* newFocusWidget = findWidget(p);
			if (newFocusWidget != mFocusWidget)
			{
				mTooltipText.Release();
			}
			mFocusWidget = newFocusWidget;

			if (mDragActive) 
				ret = mDragWidget->mouseDragEvent(p - mDragWidget->parent()->absolutePosition(), rel, buttons, modifiers);

			if (!ret)
				ret = Widget::mouseMotionEvent(p, rel, buttons, modifiers);

			return ret;
		}

		void DrawFocusedWidgetTooltip(Renderer& renderer, double elapsed)
		{
			static int tipFontSize = 15;
			static int tooltipWidth = 150;

			// Increase alpha with time so tooltip fades in
			Uint8 alpha = static_cast<Uint8>(255*std::min(1.0, 2 * (elapsed - 0.5f)) * 0.8);

			mTooltipText.SetText(mFocusWidget->tooltip());

			if (mTooltipText.NeedsCreation())
			{
				// Create texture, TODO make it wrap to toolTipwidth
				mTooltipText.Create(renderer, mTheme->mFontNormal, tipFontSize, Color(255, 255));
			}

			// TODO Draw triangle pointing to widget
// 			int px = (int)((bounds[2] + bounds[0]) / 2) - h;
// 			nvgMoveTo(mNVGContext, px, bounds[1] - 10);
// 			nvgLineTo(mNVGContext, px + 7, bounds[1] + 1);
// 			nvgLineTo(mNVGContext, px - 7, bounds[1] + 1);
// 			nvgFill(mNVGContext);

			int halfwidth = mTooltipText.getWidth() / 2;
			Point pos = mFocusWidget->absolutePosition() +
				Point(mFocusWidget->width() / 2 - halfwidth, mFocusWidget->height() + 10);

			Rect tooltipRect{ pos.x - 4 , pos.y - 4, mTooltipText.getWidth() + 8, mTooltipText.getHeight() + 8 };

			renderer.FillRect(tooltipRect, Color(0, alpha));
			renderer.OutlineRect(tooltipRect, Color(5, alpha));
			mTooltipText.SetAlpha(alpha);
			mTooltipText.Render(renderer, pos);
		}


	private:
		Widget* mDragWidget{ nullptr };
		Widget* mFocusWidget{ nullptr };
		bool	mDragActive{ false };

		TextObject mTooltipText;

		std::vector<Widget*> mFocusPath;

		double mLastInteraction{ 0.0 };
		TimeKeeper mTimer;
	};

}

