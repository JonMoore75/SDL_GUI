#include "GUIWidget.h"

#include <assert.h>
#include <algorithm>

#include "Renderer.h"
#include "GUIRootWidget.h"

namespace SGUI
{

	Widget::Widget(Widget* parent) : mParent(nullptr)
	{
		if (parent)
			parent->addChild(this);
	}

	Widget::~Widget() 
	{
		for (auto& child : mChildren) 
		{
			if (child)
				child->setParent(nullptr);
		}
	}

	void Widget::addChild(int index, Widget* widget) 
	{
		assert(index <= childCount());
		mChildren.insert(mChildren.begin() + index, std::unique_ptr<Widget>(widget));
		widget->setParent(this);
	}

	void Widget::addChild(Widget* widget) 
	{
		addChild(childCount(), widget);
	}

	void Widget::removeChild(const Widget* widget) 
	{
		//mChildren.erase( std::remove(mChildren.begin(), mChildren.end(), widget), mChildren.end() );
		mChildren.erase( std::remove_if(mChildren.begin(), mChildren.end(), [widget](const std::unique_ptr<Widget>& pWidget) { return (pWidget.get() == widget); }) );
	}

	void Widget::removeChild(int index) 
	{
		Widget* widget = mChildren[index].get();
		mChildren.erase(mChildren.begin() + index);
	}

	int Widget::childIndex(Widget* widget) const 
	{
		auto it = std::find_if(mChildren.begin(), mChildren.end(), [widget](const std::unique_ptr<Widget>& pWidget) { return (pWidget.get() == widget); });
		if (it == mChildren.end())
			return -1;
		return static_cast<int>(it - mChildren.begin());
	}

	Widget* Widget::findWidget(const Point& p) 
	{
		for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) 
		{
			Widget* child = it->get();
			if (child->visible() && child->contains(p))
				return child->findWidget(p);
		}
		return contains(p) ? this : nullptr;
	}

	void Widget::requestFocus() 
	{
		Widget* widget = this;
		while (widget->parent())
			widget = widget->parent();

		RootWidget* root = dynamic_cast<RootWidget*>(widget);
		if (root)
			root->updateFocus(this);
	}

	void Widget::Render(Renderer& renderer) 
	{
		if (mChildren.empty())
			return;
		for (auto& child : mChildren) 
		{
			if (child->visible()) 
			{
				SDL_Rect oldViewport = renderer.GetViewport();
 				renderer.SetViewport(SGUI::Rect{ child->mPos, child->mSize });
				child->Render(renderer);
				renderer.SetViewport(oldViewport);
			}
		}
	}

	bool Widget::mouseButtonEvent(const Point& p, MouseBut button, bool down, SDL_Keymod modifiers)
	{
		for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it)
		{
			auto& child = *it;
			if (child->visible() && child->contains(p) &&
				child->mouseButtonEvent(p, button, down, modifiers))
				return true;
		}
		if (button == SGUI::MouseBut::LEFT && down && !mFocused)
 			requestFocus();
		return false;
	}

	bool Widget::mouseMotionEvent(const Point& p, const Point& rel, MouseButStatus buttons, SDL_Keymod modifiers)
	{
		for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) 
		{
			auto& child = *it;

			if (!child->visible())
				continue;

			bool contained = child->contains(p); 
			bool prevContained = child->contains(p - rel);

			if (contained != prevContained)
				child->mouseEnterEvent(p, contained);

			if ((contained || prevContained) &&
				child->mouseMotionEvent(p, rel, buttons, modifiers))
				return true;
		}
		return false;
	}

	bool Widget::scrollEvent(const Point& p, const Point& rel) 
	{
		for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) 
		{
			auto& child = *it;
			if (!child->visible())
				continue;
			if (child->contains(p) && child->scrollEvent(p, rel))
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