#include "GUILabel.h"

#include "GUITheme.h"

namespace SGUI
{
	Label::Label(Widget* parent, const std::string& caption, int fontSize /*= -1*/)
		: Widget{ parent }, mCaption{ caption }, mColor{ 255, 160 }
	{
		if (mTheme) 
		{
			mFontSize = mTheme->mStandardFontSize;
			mColor = mTheme->mTextColor;
		}
		if (fontSize >= 0) 
			mFontSize = fontSize;
	}


	void Label::setTheme(Theme* theme)
	{
		Widget::setTheme(theme);
		if (mTheme) 
		{
			mFontSize = mTheme->mStandardFontSize;
			mColor = mTheme->mTextColor;
		}
	}


	Point Label::preferredSize(Renderer& renderer) const
	{
		if (mCaption.GetText() == "")
			return Point{ 0,0 };

		// Find text bounds, wrapping text over multiple lines if width would otherwise > mFixedSize.x
		Point textBounds = TextBounds(mTheme->mFontNormal, mFontSize, mCaption.GetText(), mFixedSize.x);

		textBounds.x += 2;
		return textBounds;
	}

	void Label::Render(Renderer& renderer, Point& offset)
	{
		if (mCaption.NeedsCreation())
		{
			// Text will be wrapped to mFixedSize.x if it is > 0
			mCaption.Create(renderer, mTheme->mFontNormal, mFontSize, mColor, mFixedSize.x);

			mCaption.TextAlign(TextObject::CLIPLEFT);
		}
		mCaption.Render(renderer);
	}
}
