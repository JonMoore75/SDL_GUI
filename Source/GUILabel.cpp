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


	SGUI::Point Label::preferredSize(Renderer& renderer) const
	{
		if (mCaption.GetText() == "")
			return Point{ 0,0 };

		Point textBounds = TextBounds(mTheme->mFontNormal, mFontSize, mCaption.GetText());

		// If we need to split label over multiple lines
		if (mFixedSize.x > 0) 
			return Point{ mFixedSize.x, textBounds.y }; // TODO split text

		textBounds.x += 2;
		return textBounds;
	}

	void Label::Render(Renderer& renderer, Point& offset)
	{
		if (mCaption.NeedsCreation())
		{
			mCaption.Create(renderer, mTheme->mFontNormal, mFontSize, mColor);
			mCaption.TextAlign(TextObject::CLIPALIGN::CLIPLEFT);
			// TODO split text if mFixedSize.x > 0
		}
		mCaption.Render(renderer);
	}
}
