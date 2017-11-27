#include "GUILayout.h"

#include <algorithm>
#include <assert.h>
#include "GUIWindow.h"
#include "GUITheme.h"
#include "GUILabel.h"

namespace SGUI
{
	SGUI::Point GroupLayout::preferredSize(Renderer& renderer, const Widget* widget) const
	{
		int height = mMargin;
		int width = 2 * mMargin;

		const Window* window = dynamic_cast<const Window*>(widget);
		assert( window->theme() );
		if (window && !window->title().empty())
			height += window->theme()->mWindowHeaderHeight - mMargin / 2;

		bool first = true, indent = false;
		for (auto& c : widget->children()) 
		{
			if (!c->visible())
				continue;
			const Label* label = dynamic_cast<const Label*>(c);
			if (!first)
				height += (label == nullptr) ? mSpacing : mGroupSpacing;
			first = false;

			Point ps = c->preferredSize(renderer), fs = c->fixedSize();
			Point targetSize(
				fs.x ? fs.x : ps.x,
				fs.y ? fs.y : ps.y
			);

			bool indentCur = indent && label == nullptr;
			height += targetSize.y;
			width = std::max(width, targetSize.x + 2 * mMargin + (indentCur ? mGroupIndent : 0));

			if (label)
				indent = !label->caption().empty();
		}
		height += mMargin;
		return Point(width, height);
	}

	void GroupLayout::performLayout(Renderer& renderer, Widget* widget) const
	{
		int height = mMargin; 
		int availableWidth =
			(widget->fixedWidth() ? widget->fixedWidth() : widget->width()) - 2 * mMargin;

		const Window* window = dynamic_cast<const Window*>(widget);
		if (window && !window->title().empty())
			height += window->theme()->mWindowHeaderHeight - mMargin / 2;

		bool first = true, indent = false;
		for (auto& c : widget->children()) 
		{
			if (!c->visible())
				continue;
			const Label* label = dynamic_cast<const Label*>(c);
			if (!first)
				height += (label == nullptr) ? mSpacing : mGroupSpacing;
			first = false;

			bool indentCur = indent && label == nullptr;
			Point ps = Point(availableWidth - (indentCur ? mGroupIndent : 0),
				c->preferredSize(renderer).y);
			Point fs = c->fixedSize();

			Point targetSize(
				fs.x ? fs.x : ps.x,
				fs.y ? fs.y : ps.y
			);

			c->setPosition(Point(mMargin + (indentCur ? mGroupIndent : 0), height));
			c->setSize(targetSize);
			c->performLayout(renderer);

			height += targetSize.y;

			if (label)
				indent = !label->caption().empty();
		}
	}
}
