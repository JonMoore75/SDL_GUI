#include "GUIPopupButton.h"

#include "GUITheme.h"

namespace SGUI
{

	PopupButton::PopupButton(Widget* parent, const std::string& caption /*= "Untitled"*/, int buttonIcon /*= 0*/)
		: Button(parent, caption, buttonIcon) 
	{

//		mChevronIcon = mTheme->mPopupChevronRightIcon;

		setFlags(Flags::ToggleButton | Flags::PopupButton);

		Window* parentWindow = window();
		mPopup = new Popup(parentWindow->parent(), window());
		mPopup->setSize(Point{ 320, 250 });
		mPopup->setVisible(false);

//		mIconExtraScale = 0.8f;// widget override
	}

	void PopupButton::setSide(Popup::Side popupSide)
	{
// 		if (mPopup->side() == Popup::Right &&
// 			mChevronIcon == mTheme->mPopupChevronRightIcon)
// 			setChevronIcon(mTheme->mPopupChevronLeftIcon);
// 		else if (mPopup->side() == Popup::Left &&
// 			mChevronIcon == mTheme->mPopupChevronLeftIcon)
// 			setChevronIcon(mTheme->mPopupChevronRightIcon);
		mPopup->setSide(popupSide);
	}

	void PopupButton::Render(Renderer& renderer, Point& offset)
	{
		if (!mEnabled && mPushed)
			mPushed = false;

		mPopup->setVisible(mPushed);
		Button::Render(renderer, offset);

// 		if (mChevronIcon) 
// 		{
// 		// TODO icon
// 		}
	}

	Point PopupButton::preferredSize(Renderer& renderer) const
	{
		return Button::preferredSize(renderer) + Point{ 15, 0 };
	}

	void PopupButton::performLayout(Renderer& renderer)
	{
		Widget::performLayout(renderer);

		const Window* parentWindow = window();

		int posY = absolutePosition().y - parentWindow->position().y + mSize.y / 2;
		if (mPopup->side() == Popup::Right)
			mPopup->setAnchorPos(Point{ parentWindow->width() + 15, posY });
		else
			mPopup->setAnchorPos(Point{ 0 - 15, posY });
	}

}