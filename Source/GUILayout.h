#pragma once

#include "GUICommon.h"
class Renderer;

namespace SGUI
{
	class Widget;
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
}

