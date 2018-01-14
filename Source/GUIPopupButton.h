#pragma once

#include <string>
#include "GUIButton.h"
#include "GUIPopup.h"


/**
* \class PopupButton popupbutton.h 
*
* \brief Button which launches a popup widget.
*
* \remark
*     This class overrides nanogui::Widget::mIconExtraScale to be ``0.8f``,
*     which affects all subclasses of this Widget.  Subclasses must explicitly
*     set a different value if needed (e.g., in their constructor).
*/
namespace SGUI
{
	class PopupButton : public Button
	{
	public:
		PopupButton(Widget* parent, const std::string& caption = "Untitled",
			int buttonIcon = 0);

// 		void setChevronIcon(int icon) { mChevronIcon = icon; }
// 		int chevronIcon() const { return mChevronIcon; }

		void setSide(Popup::Side popupSide);
		Popup::Side side() const { return mPopup->side(); }

		Popup* popup() { return mPopup; }
		const Popup* popup() const { return mPopup; }

		void Render(Renderer& renderer, Point& offset) override;
		Point preferredSize(Renderer& renderer) const override;

		virtual void performLayout(Renderer& renderer);

	protected:
		Popup* mPopup{ nullptr };
	//	int mChevronIcon{ 0 };
	};
}