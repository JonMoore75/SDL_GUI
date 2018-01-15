#include "GUICheckBox.h"

#include <string>
#include <algorithm>

#include "Renderer.h"
#include "GUITheme.h"
#include "entypo.h"

namespace SGUI
{
	CheckBox::CheckBox(Widget* parent, const std::string& caption /*= "Untitled"*/, const std::function<void(bool)>& callback /*= std::function<void(bool)>()*/)
		: Widget(parent),  mCallback(callback), mImageText(caption)
	{
//		mIconExtraScale = 1.2f;// widget override
	}

	void CheckBox::PreRender(Renderer& renderer, TextObject::CLIPALIGN align /*= TextObject::CLIPCENTRE*/)
	{
		Point textSize = SizeText();
		Point checkMarkSize = SizeCheck();
		Point tickBoxSize = Point{ checkMarkSize.x + tickPad, checkMarkSize.y + tickPad };
		Point tickBoxAreaSize = Point{ checkMarkSize.x + 2*tickPad, checkMarkSize.y + 2*tickPad };

		int textWidth = textSize.x;
		int textHeight = std::max(textSize.y, tickBoxAreaSize.y);

		if (mFixedSize.notZero() && mFixedSize.x > tickBoxAreaSize.x + tickPad)
		{
			textWidth = mFixedSize.x - tickBoxAreaSize.x;
			mSize = Point{ mFixedSize.x,  textHeight };
		}
		else
			mSize = Point{ tickBoxAreaSize.x + textWidth, textHeight };

		mTextPos.x = tickBoxAreaSize.x + tickPad;

		mTickBoxPos.x = tickPad / 2;
		mTickBoxPos.y = (mSize.y - tickBoxSize.y) / 2;

		mCheckPos.x = tickPad;
		mCheckPos.y = (mSize.y - checkMarkSize.y) / 2;

		/// Create check mark texture from icon
		FontTTF fontTTF;
		fontTTF.LoadFont(mTheme->mFontIcons.c_str(), fontSize(), mTheme->mTextColor);
		mCheckMark.CreateFromText(renderer, int_to_utf8(ENTYPO_ICON_CHECK), fontTTF);
		fontTTF.Release();

		/// Create text label
		mImageText.Create(renderer, mTheme->mFontNormal, fontSize(), mTheme->mTextColor);
		mImageText.TextAlign(TextObject::CLIPLEFT, Point{ textWidth, textHeight });

		/// Create tickbox textures
		SDL_BlendMode oldMode = renderer.SetRenderDrawMode(SDL_BLENDMODE_BLEND);

		Rect tickBoxRect{ Point{ 0, 0 }, tickBoxSize };

		mImageNormal.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tickBoxSize.x, tickBoxSize.y);
		mImageNormal.SetAlphaBlend();
		renderer.RenderToTexture(mImageNormal);
		renderer.FillRect(tickBoxRect, Color{0,32});
		renderer.OutlineRect(tickBoxRect, mTheme->mBorderDark);
		renderer.RenderToFrameBuffer();

		mImagePushed.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tickBoxSize.x, tickBoxSize.y);
		mImagePushed.SetAlphaBlend();
		renderer.RenderToTexture(mImagePushed);
		renderer.FillRect(tickBoxRect, Color{ 0,100 });
		renderer.OutlineRect(tickBoxRect, mTheme->mBorderDark);
		renderer.RenderToFrameBuffer();

		renderer.SetRenderDrawMode(oldMode);
	}

	bool CheckBox::mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers)
	{
		Widget::mouseButtonEvent(p, button, down, modifiers);

		if (!mEnabled)
			return false;

		if (button == SGUI::MouseBut::LEFT)
		{
			if (down) 
				mPushed = true;
			else if (mPushed) 
			{
				if (contains(p)) 
				{
					mChecked = !mChecked;
					if (mCallback)
						mCallback(mChecked);
				}
				mPushed = false;
			}
			return true;
		}
		return false;
	}

	Point CheckBox::preferredSize(Renderer& renderer) const
	{
		Point textSize = SizeText();
		Point checkMarkSize = SizeCheck();
		Point tickBoxSize = Point{ checkMarkSize.x + tickPad, checkMarkSize.y + tickPad };
		Point tickBoxAreaSize = Point{ checkMarkSize.x + 2 * tickPad, checkMarkSize.y + 2 * tickPad };

		int textHeight = std::max(textSize.y, tickBoxAreaSize.y);

		if (mFixedSize.notZero() && mFixedSize.x > tickBoxAreaSize.x + tickPad)
			return Point{ mFixedSize.x,  textHeight };

		return Point{ tickBoxAreaSize.x + textSize.x, textHeight };
	}

	Point CheckBox::SizeCheck() const
	{
		int fontSize = (mFontSize == -1) ? mTheme->mButtonFontSize : mFontSize;

		Point checkBounds = TextBounds(mTheme->mFontIcons, fontSize, int_to_utf8(ENTYPO_ICON_CHECK));

		return checkBounds;
	}

	Point CheckBox::SizeText() const
	{
		int fontSize = (mFontSize == -1) ? mTheme->mButtonFontSize : mFontSize;

		Point textBounds = TextBounds(mTheme->mFontNormal, fontSize, mImageText.GetText());
		textBounds.x += 2 * textYPad;

		return textBounds;
	}

	void CheckBox::Render(Renderer& renderer, Point& offset)
	{
		if (mImageText.NeedsCreation())
			PreRender(renderer);

		if (mPushed)
			mImagePushed.Render(renderer, mTickBoxPos.x, mTickBoxPos.y);
		else
			mImageNormal.Render(renderer, mTickBoxPos.x, mTickBoxPos.y);

		mImageText.Render(renderer, mTextPos);

		if (mChecked)
			mCheckMark.Render(renderer, mCheckPos.x, mCheckPos.y);

		Widget::Render(renderer, offset);
	}
} 

