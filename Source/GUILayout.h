#pragma once

#include "Common.h"
class Renderer;

namespace SGUI
{
	class Widget;

	/// The different kinds of alignments a layout can perform.
	enum class Alignment : uint8_t {
		Minimum = 0,
		Middle,
		Maximum,
		Fill
	};

	/// The direction of data flow for a layout.
	enum class Orientation {
		Horizontal = 0,
		Vertical
	};

	/**
	* \class Layout
	*
	* \brief Basic interface of a layout engine.
	*/
	class Layout
	{
	public:
		virtual ~Layout() { }
		virtual void performLayout(Renderer& renderer, Widget* widget) const = 0;
		virtual Point preferredSize(Renderer& renderer, const Widget* widget) const = 0;
	protected:
	};

	/**
	* \class GroupLayout layout.h nanogui/layout.h
	*
	* \brief Special layout for widgets grouped by labels.
	*
	* This widget resembles a box layout in that it arranges a set of widgets
	* vertically. All widgets are indented on the horizontal axis except for
	* \ref Label widgets, which are not indented.
	*
	* This creates a pleasing layout where a number of widgets are grouped
	* under some high-level heading.
	*/
	class GroupLayout : public Layout 
	{
	public:
		GroupLayout(int margin = 15, int spacing = 6, int groupSpacing = 14,
			int groupIndent = 20)
			: mMargin(margin), mSpacing(spacing), mGroupSpacing(groupSpacing),
			mGroupIndent(groupIndent) {}

		int margin() const { return mMargin; }
		void setMargin(int margin) { mMargin = margin; }

		int spacing() const { return mSpacing; }
		void setSpacing(int spacing) { mSpacing = spacing; }

		int groupIndent() const { return mGroupIndent; }
		void setGroupIndent(int groupIndent) { mGroupIndent = groupIndent; }

		int groupSpacing() const { return mGroupSpacing; }
		void setGroupSpacing(int groupSpacing) { mGroupSpacing = groupSpacing; }

		/* Implementation of the layout interface */
		virtual Point preferredSize(Renderer& renderer, const Widget *widget) const override;
		virtual void performLayout(Renderer& renderer, Widget *widget) const override;

	protected:
		int mMargin;
		int mSpacing;
		int mGroupSpacing;
		int mGroupIndent;
	};
}

