#pragma once

#include "GUIWidget.h"


/**
 * \class VScrollPanel GUI_VScrollPanel.h 
 *
 * \brief Adds a vertical scrollbar around a widget that is too big to fit into
 *        a certain area.
 */
namespace SGUI
{
	class VScrollPanel : public Widget 
	{
	public:
	    VScrollPanel(Widget* parent) : Widget(parent) { }
	
		virtual void performLayout(Renderer& renderer) override;
	    virtual Point preferredSize(Renderer& renderer) const override;
		virtual bool mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers) override;
		virtual bool scrollEvent(const Point& p, const Point& rel) override;
		virtual void Render(Renderer& renderer, Point& offset) override;

	protected:
		int mChildPreferredHeight{ 0 };
		float mScroll{ 0.0f };
		bool mUpdateLayout{ false };
	};
}

