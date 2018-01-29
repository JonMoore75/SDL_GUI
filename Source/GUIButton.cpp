#include "GUIButton.h"

#include "Renderer.h"
#include "GUITheme.h"
#include "entypo.h"

namespace SGUI
{
	void Button::CleanUp()
	{
		mImageNormal.Release();
		mImagePushed.Release();
		mImageFocus.Release();
//		mImageText.Release();
	}

	void Button::PreRender(Renderer& renderer, TextObject::CLIPALIGN align /*= TextObject::CLIPCENTRE*/)
	{
		if (mSize.isZero())
			setSize(preferredSize(renderer));

		SDL_BlendMode oldMode = renderer.SetRenderDrawMode(SDL_BLENDMODE_BLEND);

		TextObject imageText{ mCaption };
		Point textOffset;
		imageText.Create(renderer, mTheme->mFontNormal, fontSize(), mTheme->mTextColor);
		imageText.TextAlign(align, Point{ width(), height() });

		Texture iconTexture;
		Point iconOffset;
		if (mIcon)
		{
			/// Create check mark texture from icon
			FontTTF fontTTF;
			fontTTF.LoadFont(mTheme->mFontIcons.c_str(), fontSize(), mTheme->mTextColor);
			iconTexture.CreateFromText(renderer, int_to_utf8(mIcon), fontTTF);
			fontTTF.Release();

			int iconWidth = iconTexture.GetWidth();
			int iconPad = 8;
			int textWidth = imageText.getWidth();
			
			iconOffset.y = iconTexture.GetHeight() / 2;

			switch (mIconPosition)
			{
			case IconPosition::LeftCentered:
				iconOffset.x = (iconTexture.GetWidth() - textWidth - iconWidth) / 2;
				textOffset.x += iconWidth / 2;
				break;
			case IconPosition::RightCentered:
				iconOffset.x = (iconTexture.GetWidth() + textWidth) / 2;
				textOffset.x -= iconWidth / 2;
				break;
			default:
			case IconPosition::Left:
				iconOffset.x = iconPad;
				break;
			case IconPosition::Right:
				iconOffset.x = width() - iconWidth - iconPad;
				break;
			}
		}

		mImageNormal.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
		renderer.RenderToTexture(mImageNormal);
		renderer.FillRect(Rect{ Point{0,0}, size() }, mTheme->mButtonGradientTopFocused);
		Rect lightRect{ 0, 1, mSize.x, mSize.y - 1 };
		Rect darkRect{ 0, 0, mSize.x, mSize.y - 1 };
 		renderer.OutlineRect(lightRect, mTheme->mBorderLight);
 		renderer.OutlineRect(darkRect, mTheme->mBorderDark);
		imageText.Render(renderer, textOffset);
		iconTexture.Render(renderer, iconOffset);
		renderer.RenderToFrameBuffer();

		// Pushed
		mImagePushed.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
		renderer.RenderToTexture(mImagePushed);
		renderer.FillRect(Rect{ Point{ 0,0 }, size() }, mTheme->mButtonGradientTopPushed);
		lightRect = Rect{ 0, 0, mSize.x, mSize.y };
		darkRect = Rect{ 0, 0, mSize.x, mSize.y - 1 };
		renderer.OutlineRect(lightRect, mTheme->mBorderLight);
		renderer.OutlineRect(darkRect, mTheme->mBorderDark);
		imageText.Render(renderer, textOffset);
		iconTexture.Render(renderer, iconOffset);
		renderer.RenderToFrameBuffer();

		// Focused
		mImageFocus.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
		renderer.RenderToTexture(mImageFocus);
		renderer.FillRect(Rect{ Point{ 0,0 }, size() }, mTheme->mButtonGradientTopFocused);
		lightRect = Rect{ 0, 0, mSize.x, mSize.y };
		darkRect = Rect{ 0, 0, mSize.x, mSize.y - 1 };
		renderer.OutlineRect(lightRect, mTheme->mBorderLight);
		renderer.OutlineRect(darkRect, mTheme->mBorderDark);
		imageText.Render(renderer, textOffset);
		iconTexture.Render(renderer, iconOffset);
		renderer.RenderToFrameBuffer();

		renderer.SetRenderDrawMode(oldMode);


// 		mImageText.Create(renderer, mTheme->mFontNormal, fontSize(), mTheme->mTextColor);
// 		mImageText.TextAlign(align, Point{ mImageNormal.GetWidth(), mImageNormal.GetHeight() });
	}

	bool Button::mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers)
	{
		Widget::mouseButtonEvent(p, button, down, modifiers);

		if (!mEnabled)
			return false;

		if (button == SGUI::MouseBut::LEFT)
		{
			bool pushedBackup = mPushed;
			if (down)
			{
				if (mFlags & RadioButton) 
					RadioButtonDown();

				if (mFlags & PopupButton) 
					PopupButtonDown();

				if (mFlags & ToggleButton)
					mPushed = !mPushed;
				else
					mPushed = true;
			}
			else if (mPushed)  // Mouse button released but GUIButton state is pushed
			{
				if (mFlags & NormalButton)
					mPushed = false;
				if (contains(p) && mCallback)
					mCallback();
			}
			if (pushedBackup != mPushed && mChangeCallback)
				mChangeCallback(mPushed);

			return true;
		}
		return false;
	}

	void Button::PopupButtonDown()
	{
		for (auto widget : parent()->children())
		{
			Button *b = dynamic_cast<Button *>(widget);
			if (b != this && b && (b->flags() & PopupButton) && b->mPushed)
			{
				b->mPushed = false;
				if (b->mChangeCallback)
					b->mChangeCallback(false);
			}
		}
	}

	void Button::RadioButtonDown()
	{
		if (mButtonGroup.empty())
		{
			for (auto widget : parent()->children())
			{
				Button* b = dynamic_cast<Button*>(widget);
				if (b != this && b && (b->flags() & RadioButton) && b->mPushed)
				{
					b->mPushed = false;
					if (b->mChangeCallback)
						b->mChangeCallback(false);
				}
			}
		}
		else
		{
			for (auto b : mButtonGroup)
			{
				if (b != this && (b->flags() & RadioButton) && b->mPushed)
				{
					b->mPushed = false;
					if (b->mChangeCallback)
						b->mChangeCallback(false);
				}
			}
		}
	}

	void Button::Render(Renderer& renderer, Point& offset)
	{
//		if (mImageText.NeedsCreation())
		if (mImageNormal.isNull())
			PreRender(renderer);

		if (mPushed)
			mImagePushed.Render(renderer, offset);
		else if (mMouseFocus && mEnabled)
			mImageFocus.Render(renderer, offset);
		else
			mImageNormal.Render(renderer, offset);

//		mImageText.Render(renderer, offset);

		Widget::Render(renderer, offset);
	}

	Point Button::preferredSize(Renderer& renderer) const
	{
		int fontSize = (mFontSize == -1) ? mTheme->mButtonFontSize : mFontSize;
		Point textBounds = TextBounds(mTheme->mFontNormal, fontSize, mCaption);// mImageText.GetText());

		return Point{ textBounds.x + 20, textBounds.y + 10 };
	}


}
