#include "GUIButton.h"

#include "Renderer.h"
#include "GUITheme.h"

namespace SGUI
{
	void Button::CleanUp()
	{
		mImageNormal.Release();
		mImagePushed.Release();
		mImageFocus.Release();
		mImageText.Release();
	}

	void Button::PreRender(Renderer& renderer, TextObject::CLIPALIGN align /*= TextObject::CLIPCENTRE*/)
	{
		if (mSize.isZero())
			setSize(preferredSize(renderer));

		SDL_BlendMode oldMode = renderer.SetRenderDrawMode(SDL_BLENDMODE_BLEND);

		mImageNormal.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
		renderer.RenderToTexture(mImageNormal);
		renderer.FillRect(Rect{ Point{0,0}, size() }, mTheme->mButtonGradientTopFocused);
		Rect lightRect{ 0, 1, mSize.x, mSize.y - 1 };
		Rect darkRect{ 0, 0, mSize.x, mSize.y - 1 };
 		renderer.OutlineRect(lightRect, mTheme->mBorderLight);
 		renderer.OutlineRect(darkRect, mTheme->mBorderDark);
		renderer.RenderToFrameBuffer();

		// Pushed
		mImagePushed.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
		renderer.RenderToTexture(mImagePushed);
		renderer.FillRect(Rect{ Point{ 0,0 }, size() }, mTheme->mButtonGradientTopPushed);
		lightRect = Rect{ 0, 0, mSize.x, mSize.y };
		darkRect = Rect{ 0, 0, mSize.x, mSize.y - 1 };
		renderer.OutlineRect(lightRect, mTheme->mBorderLight);
		renderer.OutlineRect(darkRect, mTheme->mBorderDark);
		renderer.RenderToFrameBuffer();

		// Focused
		mImageFocus.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
		renderer.RenderToTexture(mImageFocus);
		renderer.FillRect(Rect{ Point{ 0,0 }, size() }, mTheme->mButtonGradientTopFocused);
		lightRect = Rect{ 0, 0, mSize.x, mSize.y };
		darkRect = Rect{ 0, 0, mSize.x, mSize.y - 1 };
		renderer.OutlineRect(lightRect, mTheme->mBorderLight);
		renderer.OutlineRect(darkRect, mTheme->mBorderDark);
		renderer.RenderToFrameBuffer();

		renderer.SetRenderDrawMode(oldMode);


		mImageText.Create(renderer, mTheme->mFontNormal, fontSize(), mTheme->mTextColor);
		mImageText.TextAlign(align, Point{ mImageNormal.GetWidth(), mImageNormal.GetHeight() });
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
		if (mImageText.NeedsCreation())
			PreRender(renderer);

		if (mPushed)
			mImagePushed.Render(renderer, 0, 0);
		else if (mMouseFocus && mEnabled)
			mImageFocus.Render(renderer, 0, 0);
		else
			mImageNormal.Render(renderer, 0, 0);

		mImageText.Render(renderer);

		Widget::Render(renderer, offset);
	}

	Point Button::preferredSize(Renderer& renderer) const
	{
		int fontSize = (mFontSize == -1) ? mTheme->mButtonFontSize : mFontSize;
		Point textBounds = TextBounds(mTheme->mFontNormal, fontSize, mImageText.GetText());

		return Point{ textBounds.x + 20, textBounds.y + 10 };
	}


}
