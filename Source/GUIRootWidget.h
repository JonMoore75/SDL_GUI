#pragma once

#include <functional>

#include "Texture.h"
#include "Font_TTF.h"

#include "GUICommon.h"
#include "GUIWidget.h"
#include "GUIWindow.h"
#include "TimeKeeper.h"

namespace SGUI
{
	class RootWidget : public Widget
	{
	public:
		RootWidget() { mTimer.Initialize(); SDL_StartTextInput();}
		~RootWidget() { SDL_StopTextInput();}

		virtual void Initialise(Renderer& renderer)
		{
			Widget::performLayout(renderer);
		}

		virtual void Render(Renderer& renderer) override
		{
			SDL_BlendMode oldMode = renderer.SetRenderDrawMode(SDL_BLENDMODE_BLEND);
			Widget::Render(renderer);
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

			if (mDragActive) 
				ret = mDragWidget->mouseDragEvent(p, rel, buttons, modifiers);

			if (!ret)
				ret = Widget::mouseMotionEvent(p, rel, buttons, modifiers);

			return ret;
		}


	private:
		Widget* mDragWidget{ nullptr };
		bool	mDragActive{ false };
		std::vector<Widget*> mFocusPath;
		double mLastInteraction{ 0.0 };
		TimeKeeper mTimer;
	};

}

