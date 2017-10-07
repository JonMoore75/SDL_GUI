#pragma once
#include <functional>

#include "Texture.h"
#include "Font_TTF.h"

#include "GUICommon.h"
#include "GUIWidget.h"

namespace SGUI
{
	enum CLIPALIGN { CLIPLEFT, CLIPCENTRE, CLIPRIGHT };

	class Button : public Widget
	{
	public:
		Button(Renderer& renderer, std::string text, int x = 0, int y = 0, Widget* parent = nullptr) : Widget(parent) { Init(renderer, text, x, y); }
		virtual ~Button() { CleanUp(); }

		void CleanUp()
		{
			//mFont.Release();
			mImageNormal.Release();
			mImagePushed.Release();
			mImageFocus.Release();
			mImageText.Release();
		}

		bool Init(Renderer& renderer, std::string text, int x, int y)
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

			FontTTF mFont;
			mFont.LoadFont("kiss_font.ttf", fontSize(), mTextColor);
			mImageText.CreateFromText(renderer, mCaption, mFont);
			mFont.Release();

			mTextPos.x = mImageNormal.GetWidth() / 2 - mImageText.GetWidth() / 2;
			mTextPos.y = mImageNormal.GetHeight() / 2 - mFont.GetHeight() / 2;

			SDL_Rect clipRect{ 0, 0, mImageText.GetWidth(), mImageText.GetHeight() };

			CLIPALIGN align = CLIPRIGHT;

			if (mTextPos.x < 0)
			{
				clipRect.w = mImageNormal.GetWidth();

				if (align == CLIPCENTRE)
					clipRect.x = -mTextPos.x;
				if (align == CLIPRIGHT)
					clipRect.x = mImageText.GetWidth() - mImageNormal.GetWidth();

				mTextPos.x = 0;
			}

			if (mTextPos.y < 0)
			{
				clipRect.y = -mTextPos.y;
				clipRect.h = mImageNormal.GetHeight();
				mTextPos.y = 0;
			}

			mImageText.SetClipRect(clipRect);

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

			mImageText.Render(renderer, mTextPos.x, mTextPos.y);

			Widget::Render(renderer);
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

		SGUI::Point mTextPos;
		std::string mCaption;

		std::function<void()>		mCallback;
		std::function<void(bool)>	mChangeCallback;

		Texture mImageNormal;
		Texture mImagePushed;
		Texture mImageFocus;
		Texture mImageText;
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

// NVGcolor gradTop = mTheme->mButtonGradientTopUnfocused;
// NVGcolor gradBot = mTheme->mButtonGradientBotUnfocused;
// 
// if (mPushed) {
// 	gradTop = mTheme->mButtonGradientTopPushed;
// 	gradBot = mTheme->mButtonGradientBotPushed;
// }
// else if (mMouseFocus && mEnabled) {
// 	gradTop = mTheme->mButtonGradientTopFocused;
// 	gradBot = mTheme->mButtonGradientBotFocused;
// }
// 
// nvgBeginPath(ctx);
// 
// nvgRoundedRect(ctx, mPos.x() + 1, mPos.y() + 1.0f, mSize.x() - 2,
// 	mSize.y() - 2, mTheme->mButtonCornerRadius - 1);
// 
// if (mBackgroundColor.w() != 0) {
// 	nvgFillColor(ctx, Color(mBackgroundColor.head<3>(), 1.f));
// 	nvgFill(ctx);
// 	if (mPushed) {
// 		gradTop.a = gradBot.a = 0.8f;
// 	}
// 	else {
// 		double v = 1 - mBackgroundColor.w();
// 		gradTop.a = gradBot.a = mEnabled ? v : v * .5f + .5f;
// 	}
// }
// 
// NVGpaint bg = nvgLinearGradient(ctx, mPos.x(), mPos.y(), mPos.x(),
// 	mPos.y() + mSize.y(), gradTop, gradBot);
// 
// nvgFillPaint(ctx, bg);
// nvgFill(ctx);
// 
// nvgBeginPath(ctx);
// nvgStrokeWidth(ctx, 1.0f);
// nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + (mPushed ? 0.5f : 1.5f), mSize.x() - 1,
// 	mSize.y() - 1 - (mPushed ? 0.0f : 1.0f), mTheme->mButtonCornerRadius);
// nvgStrokeColor(ctx, mTheme->mBorderLight);
// nvgStroke(ctx);
// 
// nvgBeginPath(ctx);
// nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + 0.5f, mSize.x() - 1,
// 	mSize.y() - 2, mTheme->mButtonCornerRadius);
// nvgStrokeColor(ctx, mTheme->mBorderDark);
// nvgStroke(ctx);
// 
// int fontSize = mFontSize == -1 ? mTheme->mButtonFontSize : mFontSize;
// nvgFontSize(ctx, fontSize);
// nvgFontFace(ctx, "sans-bold");
// float tw = nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr);
// 
// Vector2f center = mPos.cast<float>() + mSize.cast<float>() * 0.5f;
// Vector2f textPos(center.x() - tw * 0.5f, center.y() - 1);
// NVGcolor textColor =
// mTextColor.w() == 0 ? mTheme->mTextColor : mTextColor;
// if (!mEnabled)
// textColor = mTheme->mDisabledTextColor;
// 
// nvgFontSize(ctx, fontSize);
// nvgFontFace(ctx, "sans-bold");
// nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
// nvgFillColor(ctx, mTheme->mTextColorShadow);
// nvgText(ctx, textPos.x(), textPos.y(), mCaption.c_str(), nullptr);
// nvgFillColor(ctx, textColor);
// nvgText(ctx, textPos.x(), textPos.y() + 1, mCaption.c_str(), nullptr);

