#include "GUIPopup.h"

#include "GUITheme.h"
#include "Renderer.h"

namespace SGUI
{
	void Popup::performLayout(Renderer& renderer)
	{
		if (mLayout || mChildren.size() != 1) 
		{
			Widget::performLayout(renderer);
		}
		else 
		{
			mChildren[0]->setPosition(Point());
			mChildren[0]->setSize(mSize);
			mChildren[0]->performLayout(renderer);
		}
		if (mSide == Side::Left)
			mAnchorPos.x -= size().x;
	}

	void Popup::Render(Renderer& renderer, Point& offset)
	{
		refreshRelativePlacement();

		if (!mVisible)
			return;

		int ds = mTheme->mWindowDropShadowSize;
//		int cr = mTheme->mWindowCornerRadius;

		Rect popRect{ Point{ 0,0 }, size() };
		renderer.FillRect(popRect, mTheme->mWindowPopup);

		// TODO Draw anchor

		Widget::Render(renderer, offset);
	}

	void Popup::refreshRelativePlacement()
	{
		mParentWindow->refreshRelativePlacement();
		mVisible &= mParentWindow->visibleRecursive();
		mPos = mParentWindow->position() + mAnchorPos - Point{ 0, mAnchorHeight };
	}

}