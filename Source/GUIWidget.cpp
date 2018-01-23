#include "GUIWidget.h"

#include <assert.h>
#include <algorithm>

#include "Renderer.h"
#include "GUIRootWidget.h"
#include "GUILayout.h"
#include "GUITheme.h"
#include "GUIWindow.h"

namespace SGUI
{

	Widget::Widget(Widget* parent) : mParent{ nullptr }, mTheme{ nullptr }, mLayout{ nullptr }
	{
		if (parent)
			parent->addChild(this);
	}

	Widget::~Widget() 
	{
		for (auto& child : mChildren) 
		{
			if (child)
			{
				child->setParent(nullptr);
				child->decRef();
			}
		}
	}

	void Widget::setTheme(Theme* theme)
	{
		if (mTheme.get() == theme)
			return;
		mTheme = theme;
		for (auto child : mChildren)
			child->setTheme(theme);
	}

	int Widget::fontSize() const
	{
		return (mFontSize < 0 && mTheme) ? mTheme->mStandardFontSize : mFontSize;
	}

	void Widget::addChild(int index, Widget* widget)
	{
		assert(index <= childCount());
		mChildren.insert(mChildren.begin() + index, std::move(widget));
		widget->incRef();
		widget->setParent(this);
		widget->setTheme(mTheme);
	}

	void Widget::addChild(Widget* widget)
	{
		addChild(childCount(), widget);
	}

	void Widget::removeChild(const Widget* widget) 
	{
		mChildren.erase( std::remove(mChildren.begin(), mChildren.end(), widget), mChildren.end() );
		widget->decRef();
		//mChildren.erase( std::remove_if(mChildren.begin(), mChildren.end(), [widget](const std::unique_ptr<Widget>& pWidget) { return (pWidget.get() == widget); }) );
	}

	void Widget::removeChild(int index) 
	{
		Widget* widget = mChildren[index];
		mChildren.erase(mChildren.begin() + index);
		widget->decRef();
	}

	int Widget::childIndex(Widget* widget) const 
	{
		auto it = std::find_if(mChildren.begin(), mChildren.end(), [widget](const Widget* pWidget) { return (pWidget == widget); });
		if (it == mChildren.end())
			return -1;
		return static_cast<int>(it - mChildren.begin());
	}

	Widget* Widget::findWidget(const Point& p) 
	{
		Point relP = p - mPos;
		for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) 
		{
			Widget* child = *it;
			if (child->visible() && child->contains(relP))
				return child->findWidget(relP);
		}
		return contains(p) ? this : nullptr;
	}

	Point Widget::preferredSize(Renderer& renderer) const
	{
		if (mLayout)
			return mLayout->preferredSize(renderer, this);
		else
			return mSize;
	}

	void Widget::performLayout(Renderer& renderer)
	{
		if (mLayout) 
		{
			mLayout->performLayout(renderer, this);
		}
		else 
		{
			for (auto& c : mChildren) 
			{
				Point pref = c->preferredSize(renderer);
				Point fix = c->fixedSize();
				c->setSize(Point(
					fix.x ? fix.x : pref.x,
					fix.y ? fix.y : pref.y
					));
				c->performLayout(renderer);
			}
		}
	}

	Window* Widget::window() 
	{
		Widget* widget = this;
		while (true) 
		{
			if (!widget)
				throw std::runtime_error(
					"Widget:internal error (could not find parent window)");
			Window* window = dynamic_cast<Window*>(widget);
			if (window)
				return window;
			widget = widget->parent();
		}
	}

// 	Screen* Widget::screen() 
// 	{
// 		Widget* widget = this;
// 		while (true) 
// 		{
// 			if (!widget)
// 				throw std::runtime_error(
// 					"Widget:internal error (could not find parent screen)");
// 			Screen* screen = dynamic_cast<Screen*>(widget);
// 			if (screen)
// 				return screen;
// 			widget = widget->parent();
// 		}
// 	}

	void Widget::requestFocus()
	{
		Widget* widget = this;
		while (widget->parent())
			widget = widget->parent();

		RootWidget* root = dynamic_cast<RootWidget*>(widget);
		if (root)
			root->updateFocus(this);
	}

	void Widget::Render(Renderer& renderer, Point& offset)
	{
		if (mChildren.empty())
			return;
		for (auto& child : mChildren) 
		{
			if (child->visible()) 
			{
				Rect oldViewport = renderer.GetViewport();
				Rect childRect{ child->absolutePosition(), child->size() };

				Rect intersectRect = IntersectRect(oldViewport, childRect);

				renderer.SetViewport(intersectRect);

				Point newOffset = childRect.getPosition() - intersectRect.getPosition();

				child->Render(renderer, newOffset);
				renderer.SetViewport(oldViewport);
			}
		}
	}

	bool Widget::mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers)
	{
		Point relP = p - mPos;
		for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it)
		{
			
			auto& child = *it;
			if (child->visible() && child->contains(relP) &&
				child->mouseButtonEvent(relP, button, down, modifiers))
				return true;
		}
		if (button == SGUI::MouseBut::LEFT && down && !mFocused)
 			requestFocus();
		return false;
	}

	bool Widget::mouseMotionEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers)
	{
		Point relP = p - mPos;
		for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) 
		{
			auto& child = *it;
			
			if (!child->visible())
				continue;

			bool contained = child->contains(relP);
			bool prevContained = child->contains(relP - rel);

			if (contained != prevContained)
				child->mouseEnterEvent(relP, contained);

			if ((contained || prevContained) &&
				child->mouseMotionEvent(relP, rel, buttons, modifiers))
				return true;
		}
		return false;
	}

	bool Widget::scrollEvent(const Point& p, const Point& rel) 
	{
		Point relP = p - mPos;
		for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) 
		{
			auto& child = *it;
			if (!child->visible())
				continue;
			if (child->contains(relP) && child->scrollEvent(relP, rel))
				return true;
		}
		return false;
	}

	bool Widget::mouseEnterEvent(const Point&, bool enter) 
	{
		mMouseFocus = enter;
		return false;
	}

	bool Widget::focusEvent(bool focused) 
	{
		mFocused = focused;
		return false;
	}
}