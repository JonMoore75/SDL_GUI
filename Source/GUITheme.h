#pragma once

#include "Common.h"
#include "GUIObject.h"

class Renderer;

namespace SGUI
{
	/**
	* \class Theme theme.h 
	*
	* \brief Storage class for basic theme-related properties.
	*/
	class Theme : public Object
	{
	public:
		Theme();

 		/* Fonts */
 		std::string mFontNormal;
// 		int mFontBold;
		std::string mFontIcons;

		/* Spacing-related parameters */
		int mStandardFontSize;
		int mButtonFontSize;
		int mTextBoxFontSize;
		int mWindowCornerRadius;
		int mWindowHeaderHeight;
		int mWindowDropShadowSize;
		int mButtonCornerRadius;

		/* Tab-related parameters*/
		float mTabBorderWidth;
		int mTabInnerMargin;
		int mTabMinButtonWidth;
		int mTabMaxButtonWidth;
		int mTabControlWidth;
		int mTabButtonHorizontalPadding;
		int mTabButtonVerticalPadding;

		/* Generic colors */
		Color mDropShadow;
		Color mTransparent;
		Color mBorderDark;
		Color mBorderLight;
		Color mBorderMedium;
		Color mTextColor;
		Color mDisabledTextColor;
		Color mTextColorShadow;
		Color mIconColor;

		/* Button colors */
		Color mButtonGradientTopFocused;
		Color mButtonGradientBotFocused;
		Color mButtonGradientTopUnfocused;
		Color mButtonGradientBotUnfocused;
		Color mButtonGradientTopPushed;
		Color mButtonGradientBotPushed;

		/* Window colors */
		Color mWindowFillUnfocused;
		Color mWindowFillFocused;
		Color mWindowTitleUnfocused;
		Color mWindowTitleFocused;

		Color mWindowHeaderGradientTop;
		Color mWindowHeaderGradientBot;
		Color mWindowHeaderSepTop;
		Color mWindowHeaderSepBot;

		Color mWindowPopup;
		Color mWindowPopupTransparent;

		/// Icon to use for CheckBox widgets (default: ``ENTYPO_ICON_CHECK``).
		int mCheckBoxIcon;
		/// Icon to use for informational MessageDialog widgets (default: ``ENTYPO_ICON_INFO_WITH_CIRCLE``).
		int mMessageInformationIcon;
		/// Icon to use for interrogative MessageDialog widgets (default: ``ENTYPO_ICON_HELP_WITH_CIRCLE``).
		int mMessageQuestionIcon;
		/// Icon to use for warning MessageDialog widgets (default: ``ENTYPO_ICON_WARNING``).
		int mMessageWarningIcon;
		/// Icon to use on MessageDialog alt button (default: ``ENTYPO_ICON_CIRCLE_WITH_CROSS``).
		int mMessageAltButtonIcon;
		/// Icon to use on MessageDialog primary button (default: ``ENTYPO_ICON_CHECK``).
		int mMessagePrimaryButtonIcon;
		/// Icon to use for PopupButton widgets opening to the right (default: ``ENTYPO_ICON_CHEVRON_RIGHT``).
		int mPopupChevronRightIcon;
		/// Icon to use for PopupButton widgets opening to the left (default: ``ENTYPO_ICON_CHEVRON_LEFT``).
		int mPopupChevronLeftIcon;
		/// Icon to indicate hidden tabs to the left on a TabHeader (default: ``ENTYPO_ICON_ARROW_BOLD_LEFT``).
		int mTabHeaderLeftIcon;
		/// Icon to indicate hidden tabs to the right on a TabHeader (default: ``ENTYPO_ICON_ARROW_BOLD_RIGHT``).
		int mTabHeaderRightIcon;
		/// Icon to use when a TextBox has an up toggle (e.g. IntBox) (default: ``ENTYPO_ICON_CHEVRON_UP``).
		int mTextBoxUpIcon;
		/// Icon to use when a TextBox has a down toggle (e.g. IntBox) (default: ``ENTYPO_ICON_CHEVRON_DOWN``).
		int mTextBoxDownIcon;
	protected:
		virtual ~Theme() { };
	};
}

