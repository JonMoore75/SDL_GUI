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
		Button(Renderer& renderer, std::string text, int x = 0, int y = 0, Widget* parent = nullptr) : Widget(parent) { Init(renderer, text, x, y); }
		virtual ~Button() { CleanUp(); }

		void CleanUp()
		{
			mImageNormal.Release();
			mImagePushed.Release();
			mImageFocus.Release();
			mImageText.Release();
		}

		bool Init(Renderer& renderer, std::string text, int x, int y, TextObject::CLIPALIGN align = TextObject::CLIPCENTRE)
		{
			mCaption = text;

 			mImageNormal.CreateFromFile(renderer, "kiss_normal.png");
			mImagePushed.CreateFromFile(renderer, "kiss_active.png");
			mImageFocus.CreateFromFile(renderer, "kiss_prelight.png");

			setPosition(Point{ x, y });
			setSize(Point{ mImageNormal.GetWidth(), mImageNormal.GetHeight() });

			mImageNormal.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
			renderer.SetRenderTexture(mImageNormal);
			renderer.FillRect(Rect{ Point{0,0}, size() }, mNormColor);
			Rect lightRect{0, 1, mSize.x, mSize.y - 1 };
			Rect darkRect{0, 0, mSize.x, mSize.y - 1 };
			renderer.OutlineRect(lightRect, mBorderLight);
			renderer.OutlineRect(darkRect, mBorderDark);
			renderer.SetRenderFrameBuffer();

			// Pushed
			mImagePushed.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
			renderer.SetRenderTexture(mImagePushed);
			renderer.FillRect(Rect{ Point{ 0,0 }, size() }, mPushedColor);
			lightRect = Rect{ 0, 0, mSize.x, mSize.y };
			darkRect = Rect{ 0, 0, mSize.x, mSize.y - 1 };
			renderer.OutlineRect(lightRect, mBorderLight);
			renderer.OutlineRect(darkRect, mBorderDark);
			renderer.SetRenderFrameBuffer();

			// Focused
			mImageFocus.Create(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width(), height());
			renderer.SetRenderTexture(mImageFocus);
			renderer.FillRect(Rect{ Point{ 0,0 }, size() }, mFocusColor);
			lightRect = Rect{ 0, 0, mSize.x, mSize.y };
			darkRect = Rect{ 0, 0, mSize.x, mSize.y - 1 };
			renderer.OutlineRect(lightRect, mBorderLight);
			renderer.OutlineRect(darkRect, mBorderDark);
			renderer.SetRenderFrameBuffer();

			mImageText.SetText(text);
			mImageText.Create(renderer, "Boku2-Regular.otf", fontSize(), mTextColor);
			mImageText.TextAlign(align, Point{ mImageNormal.GetWidth(), mImageNormal.GetHeight() });

			return false;
		}

		void SetTextColor(const SGUI::Color& col) { mTextColor = col; }

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
					if (contains(p) && mCallback)
						mCallback();
					mPushed = false;
				}
				if (pushedBackup != mPushed && mChangeCallback)
					mChangeCallback(mPushed);

				return true;
			}
			return false;
		}

		void Render(Renderer& renderer) override
		{
			if (mPushed)
				mImagePushed.Render(renderer, 0, 0);
			else if (mMouseFocus && mEnabled)
				mImageFocus.Render(renderer, 0, 0);
			else
				mImageNormal.Render(renderer, 0, 0);

			mImageText.Render(renderer);

			Widget::Render(renderer);
		}

		/// Compute the preferred size of the widget
		virtual Point preferredSize(Renderer& renderer) const override
		{
			Point textBounds = TextBounds("Boku2-Regular.otf", mFontSize, mCaption);

			return Point{ textBounds.x + 20, textBounds.y + 10 };
		}


	private:
		Color mTextColor{ 255, 160 };
		Color mTextColorShadow{ 0, 160 };
		Color mDisabledTextColor{ 255, 80 };

		Color mNormColor{ 74, 255 };
		Color mPushedColor{ 41, 255 };
		Color mFocusColor{ 64, 255 };

		Color mBorderDark{ 29, 255 };
		Color mBorderLight{ 92, 255 };

		bool mPushed{ false };

		std::string mCaption;

		std::function<void()>		mCallback;
		std::function<void(bool)>	mChangeCallback;

		Texture mImageNormal;
		Texture mImagePushed;
		Texture mImageFocus;
		TextObject mImageText;
	};
}



// mStandardFontSize = 16;
// mButtonFontSize = 20;
// mTextBoxFontSize = 20;
// mWindowCornerRadius = 2;
// mWindowHeaderHeight = 30;
// mWindowDropShadowSize = 10;
// mButtonCornerRadius = 2;
// mTabBorderWidth = 0.75f;
// mTabInnerMargin = 5;
// mTabMinButtonWidth = 20;
// mTabMaxButtonWidth = 160;
// mTabControlWidth = 20;
// mTabButtonHorizontalPadding = 10;
// mTabButtonVerticalPadding = 2;
// 
// mDropShadow = Color(0, 128);
// mTransparent = Color(0, 0);
// mBorderDark = Color(29, 255);
// mBorderLight = Color(92, 255);
// mBorderMedium = Color(35, 255);
// mTextColor = Color(255, 160);
// mDisabledTextColor = Color(255, 80);
// mTextColorShadow = Color(0, 160);
// mIconColor = mTextColor;
// 
// mButtonGradientTopFocused = Color(64, 255);
// mButtonGradientBotFocused = Color(48, 255);
// mButtonGradientTopUnfocused = Color(74, 255);
// mButtonGradientBotUnfocused = Color(58, 255);
// mButtonGradientTopPushed = Color(41, 255);
// mButtonGradientBotPushed = Color(29, 255);
// 
// /* Window-related */
// mWindowFillUnfocused = Color(43, 230);
// mWindowFillFocused = Color(45, 230);
// mWindowTitleUnfocused = Color(220, 160);
// mWindowTitleFocused = Color(255, 190);
// 
// mWindowHeaderGradientTop = mButtonGradientTopUnfocused;
// mWindowHeaderGradientBot = mButtonGradientBotUnfocused;
// mWindowHeaderSepTop = mBorderLight;
// mWindowHeaderSepBot = mBorderDark;
// 
// mWindowPopup = Color(50, 255);
// mWindowPopupTransparent = Color(50, 0);



