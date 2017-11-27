#include "GUITheme.h"

#include "Renderer.h"

namespace SGUI
{
	Theme::Theme()
	{
		mFontNormal = "Boku2-Regular.otf";

		mStandardFontSize = 16;

		mButtonFontSize = 20;

		mTextBoxFontSize = 20;

		mWindowCornerRadius = 2;
		mWindowHeaderHeight = 30;
		mWindowDropShadowSize = 10;

		mButtonCornerRadius = 2;

		mTabBorderWidth = 0.75f;
		mTabInnerMargin = 5;
		mTabMinButtonWidth = 20;
		mTabMaxButtonWidth = 160;
		mTabControlWidth = 20;
		mTabButtonHorizontalPadding = 10;
		mTabButtonVerticalPadding = 2;

		mDropShadow = Color(0, 128);
		mTransparent = Color(0, 0);
		mBorderDark = Color(29, 255);
		mBorderLight = Color(92, 255);
		mBorderMedium = Color(35, 255);
		mTextColor = Color(255, 160);
		mDisabledTextColor = Color(255, 80);
		mTextColorShadow = Color(0, 160);
		mIconColor = mTextColor;

		mButtonGradientTopFocused = Color(64, 255);
		mButtonGradientBotFocused = Color(48, 255);
		mButtonGradientTopUnfocused = Color(74, 255);
		mButtonGradientBotUnfocused = Color(58, 255);
		mButtonGradientTopPushed = Color(41, 255);
		mButtonGradientBotPushed = Color(29, 255);

		/* Window-related */
		mWindowFillUnfocused = Color(43, 230);
		mWindowFillFocused = Color(45, 230);
		mWindowTitleUnfocused = Color(220, 160);
		mWindowTitleFocused = Color(255, 190);

		mWindowHeaderGradientTop = mButtonGradientTopUnfocused;
		mWindowHeaderGradientBot = mButtonGradientBotUnfocused;
		mWindowHeaderSepTop = mBorderLight;
		mWindowHeaderSepBot = mBorderDark;

		mWindowPopup = Color(50, 255);
		mWindowPopupTransparent = Color(50, 0);
	}
}
