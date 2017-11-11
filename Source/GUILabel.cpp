#include "GUILabel.h"

namespace SGUI
{
	Label::Label(Widget* parent, const std::string& caption, int fontSize /*= -1*/)
		: Widget{ parent }, mCaption{ caption }, mColor{ 255, 160 }
	{
		if (fontSize >= 0)
			mFontSize = fontSize;
		else
			mFontSize = 16;
	}


	SGUI::Point Label::preferredSize(Renderer& renderer) const
	{
		if (mCaption.GetText() == "")
			return Point{ 0,0 };

		Point textBounds = TextBounds("Boku2-Regular.otf", mFontSize, mCaption.GetText());

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
			mCaption.Create(renderer, "Boku2-Regular.otf", mFontSize, mColor);
			mCaption.TextAlign(TextObject::CLIPALIGN::CLIPLEFT);
			// TODO split text if mFixedSize.x > 0
		}
		mCaption.Render(renderer);
	}
}
