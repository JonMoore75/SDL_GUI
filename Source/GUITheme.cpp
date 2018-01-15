#include "GUITheme.h"

#include "Renderer.h"
#include "entypo.h"

namespace SGUI
{
	Theme::Theme()
	{
		mFontNormal = "Boku2-Regular.otf";
		mFontIcons = "entypo.ttf";

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

		mCheckBoxIcon = ENTYPO_ICON_CHECK;
		mMessageInformationIcon = ENTYPO_ICON_INFO_WITH_CIRCLE;
		mMessageQuestionIcon = ENTYPO_ICON_HELP_WITH_CIRCLE;
		mMessageWarningIcon = ENTYPO_ICON_WARNING;
		mMessageAltButtonIcon = ENTYPO_ICON_CIRCLE_WITH_CROSS;
		mMessagePrimaryButtonIcon = ENTYPO_ICON_CHECK;
		mPopupChevronRightIcon = ENTYPO_ICON_CHEVRON_RIGHT;
		mPopupChevronLeftIcon = ENTYPO_ICON_CHEVRON_LEFT;
		mTabHeaderLeftIcon = ENTYPO_ICON_ARROW_BOLD_LEFT;
		mTabHeaderRightIcon = ENTYPO_ICON_ARROW_BOLD_RIGHT;
		mTextBoxUpIcon = ENTYPO_ICON_CHEVRON_UP;
		mTextBoxDownIcon = ENTYPO_ICON_CHEVRON_DOWN;
	}
}
