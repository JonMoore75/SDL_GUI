#include "GUI_VScrollPanel.h"

#include <algorithm>

#include "Renderer.h"
#include "Common.h"

namespace SGUI
{
	void VScrollPanel::performLayout(Renderer& renderer)
	{
		Widget::performLayout(renderer);

		if (mChildren.empty())
			return;
		if (mChildren.size() > 1)
			throw std::runtime_error("VScrollPanel should have one child.");

		Widget* child = mChildren[0];
		mChildPreferredHeight = child->preferredSize(renderer).y;

		if (mChildPreferredHeight > mSize.y)
		{
			child->setPosition( Point{ 0, static_cast<int>(-mScroll * (mChildPreferredHeight - mSize.y ))} );
			child->setSize( Point{ mSize.x - 12, mChildPreferredHeight } );
		}
		else 
		{
			child->setPosition(Point{0,0});
			child->setSize(mSize);
			mScroll = 0;
		}
		child->performLayout(renderer);

		mUpdateLayout = false;
	}

	Point VScrollPanel::preferredSize(Renderer& renderer) const
	{
		if (mChildren.empty())
			return Point{ 0,0 };
		return mChildren[0]->preferredSize(renderer) + Point{ 12,0 };
	}
	
	bool VScrollPanel::mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers)
	{
		if (!mChildren.empty() && mChildPreferredHeight > mSize.y) 
		{
			float scrollh = height() *
				std::min(1.0f, height() / (float)mChildPreferredHeight);

			mScroll = std::max((float) 0.0f, std::min((float) 1.0f,
				mScroll + rel.y / (float)(mSize.y - 8 - scrollh)));
			mUpdateLayout = true;
			return true;
		}
		else 
			return Widget::mouseDragEvent(p, rel, buttons, modifiers);
	}
	
	bool VScrollPanel::scrollEvent(const Point& p, const Point& rel)
	{
		if (!mChildren.empty() && mChildPreferredHeight > mSize.y) 
		{
			float scrollAmount = rel.y * (mSize.y / 20.0f);
			float scrollh = height() *
				std::min(1.0f, height() / (float)mChildPreferredHeight);

			mScroll = std::max((float) 0.0f, std::min((float) 1.0f,
				mScroll - scrollAmount / (float)(mSize.y - 8 - scrollh)));
			mUpdateLayout = true;
			return true;
		}
		else 
			return Widget::scrollEvent(p, rel);
	}

	void VScrollPanel::Render(Renderer& renderer, Point& offset)
	{
		if (mChildren.empty())
			return;
		if (mChildren.size() > 1)
			throw std::runtime_error("VScrollPanel should have one child.");

		Widget* child = mChildren[0];
		child->setPosition( Point{ 0, static_cast<int>(-mScroll * (mChildPreferredHeight - mSize.y)) } );
		mChildPreferredHeight = child->preferredSize(renderer).y;
		float scrollh = height() * std::min(1.0f, height() / (float)mChildPreferredHeight);

		if (mUpdateLayout)
			performLayout(renderer);

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
			
		if (mChildPreferredHeight <= mSize.y)
			return;

		renderer.FillRect( Rect{ offset.x + mSize.x - 12, offset.y + 4, 8, mSize.y - 8 }, Color{0, 92} );

		Rect caretRect{ 
			offset.x + mSize.x - 12 + 1,
			offset.y + static_cast<int>(4 + 1 + (mSize.y - 8 - scrollh) * mScroll),
			8 - 2, 
			static_cast<int>(scrollh - 2) };
		renderer.FillRect(caretRect, Color(128, 100));
	}

// 	void VScrollPanel::draw(NVGcontext *ctx) {
// 	    if (mChildren.empty())
// 	        return;
// 	    Widget *child = mChildren[0];
// 	    child->setPosition(Vector2i(0, -mScroll*(mChildPreferredHeight - mSize.y())));
// 	    mChildPreferredHeight = child->preferredSize(ctx).y();
// 	    float scrollh = height() *
// 	        std::min(1.0f, height() / (float) mChildPreferredHeight);
// 	
// 	    if (mUpdateLayout)
// 	        child->performLayout(ctx);
// 	
// 	    nvgSave(ctx);
// 	    nvgTranslate(ctx, mPos.x(), mPos.y());
// 	    nvgIntersectScissor(ctx, 0, 0, mSize.x(), mSize.y());
// 	    if (child->visible())
// 	        child->draw(ctx);
// 	    nvgRestore(ctx);
// 	
// 	    if (mChildPreferredHeight <= mSize.y())
// 	        return;
// 	
// 
// // 	        mSize.y() - 8, 3, 4, Color(0, 32), Color(0, 92));
// 	    nvgBeginPath(ctx);
// 	    nvgRoundedRect(ctx, 
// 			mPos.x() + mSize.x() - 12, 
// 			mPos.y() + 4, 
// 			8,
// 	        mSize.y() - 8, 
// 			3);
// 	    nvgFillPaint(ctx, paint);
// 	    nvgFill(ctx);
// 	
// 
// // 	        3, 4, Color(220, 100), Color(128, 100));
// 	
// 	    nvgRoundedRect(ctx, 
// 			mPos.x() + mSize.x() - 12 + 1,
// 	        mPos.y() + 4 + 1 + (mSize.y() - 8 - scrollh) * mScroll, 
// 			8 - 2,
// 	        scrollh - 2, 
// 			2);
// 	}
}
