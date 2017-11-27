#pragma once
#include <functional>

#include "Texture.h"
#include "Font_TTF.h"

#include "GUICommon.h"
#include "GUIWidget.h"
#include "TextObject.h"


namespace SGUI
{
	class Button : public Widget
	{
	public:
		Button(Widget* parent, std::string text) : Widget{parent}, mImageText{ text } { }
		virtual ~Button() { CleanUp(); }

		void CleanUp()
		{
			mImageNormal.Release();
			mImagePushed.Release();
			mImageFocus.Release();
			mImageText.Release();
		}

		void PreRender(Renderer& renderer, TextObject::CLIPALIGN align = TextObject::CLIPCENTRE)
		{
			if (mSize.isZero())
				setSize( preferredSize(renderer) );

			mImageNormal.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
			renderer.SetRenderTexture(mImageNormal);
			renderer.FillRect(Rect{ Point{0,0}, size() }, mTheme->mButtonGradientTopUnfocused);
			Rect lightRect{0, 1, mSize.x, mSize.y - 1 };
			Rect darkRect{0, 0, mSize.x, mSize.y - 1 };
			renderer.OutlineRect(lightRect, mTheme->mBorderLight);
			renderer.OutlineRect(darkRect, mTheme->mBorderDark);
			renderer.SetRenderFrameBuffer();

			// Pushed
			mImagePushed.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
			renderer.SetRenderTexture(mImagePushed);
			renderer.FillRect(Rect{ Point{ 0,0 }, size() }, mTheme->mButtonGradientTopPushed);
			lightRect = Rect{ 0, 0, mSize.x, mSize.y };
			darkRect = Rect{ 0, 0, mSize.x, mSize.y - 1 };
			renderer.OutlineRect(lightRect, mTheme->mBorderLight);
			renderer.OutlineRect(darkRect, mTheme->mBorderDark);
			renderer.SetRenderFrameBuffer();

			// Focused
			mImageFocus.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
			renderer.SetRenderTexture(mImageFocus);
			renderer.FillRect(Rect{ Point{ 0,0 }, size() }, mTheme->mButtonGradientTopFocused);
			lightRect = Rect{ 0, 0, mSize.x, mSize.y };
			darkRect = Rect{ 0, 0, mSize.x, mSize.y - 1 };
			renderer.OutlineRect(lightRect, mTheme->mBorderLight);
			renderer.OutlineRect(darkRect, mTheme->mBorderDark);
			renderer.SetRenderFrameBuffer();

			
			mImageText.Create(renderer, mTheme->mFontNormal, fontSize(), mTheme->mTextColor);
			mImageText.TextAlign(align, Point{ mImageNormal.GetWidth(), mImageNormal.GetHeight() });
		}

		/// Set the push callback (for any type of button)
		std::function<void()> callback() const { return mCallback; }
		void setCallback(const std::function<void()> &callback) { mCallback = callback; }

		/// Set the change callback (for toggle buttons)
		std::function<void(bool)> changeCallback() const { return mChangeCallback; }
		void setChangeCallback(const std::function<void(bool)> &callback) { mChangeCallback = callback; }


		bool mouseButtonEvent(const Point& p, MouseBut button, bool down, SDL_Keymod modifiers) override
		{
			Widget::mouseButtonEvent(p, button, down, modifiers);

			if (button == SGUI::MouseBut::LEFT && mEnabled)
			{
				bool pushedBackup = mPushed;
				if (down)
				{
					mPushed = true;
				}
				else if (mPushed)
				{
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

		void Render(Renderer& renderer, Point& offset) override
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

		/// Compute the preferred size of the widget
		virtual Point preferredSize(Renderer& renderer) const override
		{
			int fontSize = (mFontSize == -1) ? mTheme->mButtonFontSize : mFontSize;
			Point textBounds = TextBounds(mTheme->mFontNormal, fontSize, mImageText.GetText());

			return Point{ textBounds.x + 20, textBounds.y + 10 };
		}


	private:
		bool mPushed{ false };

		std::function<void()>		mCallback;
		std::function<void(bool)>	mChangeCallback;

		Texture mImageNormal;
		Texture mImagePushed;
		Texture mImageFocus;
		TextObject mImageText;
	};
}



