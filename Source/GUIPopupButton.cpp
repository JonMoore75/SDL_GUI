#include "GUIPopupButton.h"

#include "GUITheme.h"
#include <assert.h>

namespace SGUI
{

	PopupButton::PopupButton(Widget* parent, const std::string& caption /*= "Untitled"*/, int buttonIcon /*= 0*/)
		: Button(parent, caption, buttonIcon) 
	{
		setFlags(Flags::ToggleButton | Flags::PopupButton);

		Window* parentWindow = window();
		mPopup = new Popup(parentWindow->parent(), window());
		assert(mPopup);
		mPopup->setSize(Point{ 320, 250 });
		mPopup->setVisible(false);

		setSide(Popup::Right);

//		mIconExtraScale = 0.8f;// widget override
	}

	void PopupButton::setSide(Popup::Side popupSide)
	{
		if (popupSide == Popup::Right)
		{
			mIcon = mTheme->mPopupChevronRightIcon;
			mIconPosition = Button::IconPosition::Right;
			
		}
		else if (popupSide == Popup::Left)
		{
			mIcon = mTheme->mPopupChevronLeftIcon;
			mIconPosition = Button::IconPosition::Left;
		}
		mPopup->setSide(popupSide);
	}

	void PopupButton::Render(Renderer& renderer, Point& offset)
	{
		if (!mEnabled && mPushed)
			mPushed = false;

		mPopup->setVisible(mPushed);
		Button::Render(renderer, offset);
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