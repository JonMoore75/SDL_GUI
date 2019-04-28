#ifndef GUIWidget_h__
#define GUIWidget_h__
#pragma once

#include <string>
#include <memory>
#include <vector>
#include "Common.h"
#include "GUIObject.h"

namespace SGUI
{
	class Layout;
	class Theme;
	class Window;

	enum MouseBut {NONE, LEFT, MIDDLE, RIGHT};

	class MouseButStatus
	{
	public:
		MouseButStatus() {}
		MouseButStatus(bool left, bool middle, bool right) 
			: mLeft(left), mRight(right), mMiddle(middle) {}

		bool mLeft{ false };
		bool mMiddle{ false };
		bool mRight{ false };
	};
	/**
	* \class Widget widget.h
	*
	* \brief Base class of all widgets.
	*
	* \ref Widget is the base class of all widgets in \c SGUI.
	*/
	class Widget : public Object
	{
	public:
		/// Construct a new widget with the given parent widget
		Widget(Widget* parent = nullptr);
		/// Free all resources used by the widget and any children
		virtual ~Widget();

		/// Return the parent widget
		Widget* parent() { return mParent; }
		/// Return the parent widget
		const Widget* parent() const { return mParent; }
		/// Set the parent widget
		void setParent(Widget* parent) { mParent = parent; }

		/// Return the used \ref Layout generator
		Layout* layout() { return mLayout.get(); }
		/// Return the used \ref Layout generator
		const Layout* layout() const { return mLayout.get(); }
		/// Set the used \ref Layout generator
		void setLayout(Layout* layout) { mLayout = layout; }
		
		/// Return the \ref Theme used to draw this widget
		Theme* theme() { return mTheme; }
		/// Return the \ref Theme used to draw this widget
		const Theme* theme() const { return mTheme.get(); }
		/// Set the \ref Theme used to draw this widget
		virtual void setTheme(Theme* theme);

		/// Return the position relative to the parent widget
		const Point& position() const { return mPos; }
		/// Set the position relative to the parent widget
		void setPosition(const Point& pos) { mPos = pos; }

		/// Return the absolute position on screen
		Point absolutePosition() const {
			return mParent ?
				(parent()->absolutePosition() + mPos) : mPos;
		}

		void translate(const Point& translation) { mPos += translation; }


		/// Return the size of the widget
		const Point& size() const { return mSize; }
		/// set the size of the widget
		virtual void setSize(const Point& size) { mSize = size; }

		/// Return the width of the widget
		int width() const { return mSize.x; }
		/// Set the width of the widget
		void setWidth(int width) { mSize.x = width; }

		/// Return the height of the widget
		int height() const { return mSize.y; }
		/// Set the height of the widget
		void setHeight(int height) { mSize.y = height; }

		/**
		* \brief Set the fixed size of this widget
		*
		* If nonzero, components of the fixed size attribute override any values
		* computed by a layout generator associated with this widget. Note that
		* just setting the fixed size alone is not enough to actually change its
		* size; this is done with a call to \ref setSize or a call to \ref performLayout()
		* in the parent widget.
		*/
		void setFixedSize(const Point& fixedSize) { mFixedSize = fixedSize; }

		/// Return the fixed size (see \ref setFixedSize())
		const Point& fixedSize() const { return mFixedSize; }

		/// Return the fixed width (see \ref setFixedSize())
		int fixedWidth() const { return mFixedSize.x; }
		/// Return the fixed height (see \ref setFixedSize())
		int fixedHeight() const { return mFixedSize.y; }
		/// Set the fixed width (see \ref setFixedSize())
		void setFixedWidth(int width) { mFixedSize.x = width; }
		/// Set the fixed height (see \ref setFixedSize())
		void setFixedHeight(int height) { mFixedSize.y = height; }

		Rect GetRect() const { return Rect{mPos, mSize}; }


		/// Return whether or not the widget is currently visible (assuming all parents are visible)
		bool visible() const { return mVisible; }
		/// Set whether or not the widget is currently visible (assuming all parents are visible)
		void setVisible(bool visible) { mVisible = visible; }

		/// Check if this widget is currently visible, taking parent widgets into account
		bool visibleRecursive() const 
		{
			bool visible = true;
			const Widget* widget = this;
			while (widget) 
			{
				visible &= widget->visible();
				widget = widget->parent();
			}
			return visible;
		}

		/// Return the number of child widgets
		int childCount() const { return static_cast<int>(mChildren.size()); }

		/// Return the list of child widgets of the current widget
		const std::vector<Widget*>& children() const { return mChildren; }

		/**
		* \brief Add a child widget to the current widget at
		* the specified index.
		*
		* This function almost never needs to be called by hand,
		* since the constructor of \ref Widget automatically
		* adds the current widget to its parent
		*/
		virtual void addChild(int index, Widget* widget);

		/// Convenience function which appends a widget at the end
		void addChild(Widget* widget);

		/// Remove a child widget by index
		void removeChild(int index);

		/// Remove a child widget by value
		void removeChild(const Widget* widget);

		/// Retrieves the child at the specific position
		const Widget* childAt(int index) const { return mChildren[index]; }

		/// Retrieves the child at the specific position
		Widget* childAt(int index) { return mChildren[index]; }

		/// Returns the index of a specific child or -1 if not found
		int childIndex(Widget* widget) const;

		/// Variadic shorthand notation to construct and add a child widget
		template<typename WidgetClass, typename... Args>
		WidgetClass* add(const Args&... args) 
		{
			return new WidgetClass(this, args...);
		}

		/// Walk up the hierarchy and return the parent window
		Window* window();

// 		/// Walk up the hierarchy and return the parent screen
// 		Screen *screen();

		/// Associate this widget with an ID value (optional)
		void setId(const std::string& id) { mId = id; }
		/// Return the ID value associated with this widget, if any
		const std::string& id() const { return mId; }

		/// Return whether or not this widget is currently enabled
		bool enabled() const { return mEnabled; }
		/// Set whether or not this widget is currently enabled
		virtual void setEnabled(bool enabled) { mEnabled = enabled; }

		/// Return whether or not this widget is currently focused
		bool focused() const { return mFocused; }
		/// Set whether or not this widget is currently focused
		void setFocused(bool focused) { mFocused = focused; }
		/// Request the focus to be moved to this widget
		void requestFocus();

		const std::string& tooltip() const { return mTooltip; }
		void setTooltip(const std::string& tooltip) { mTooltip = tooltip; }

		/// Return current font size. If not set the default of the current theme will be returned
		int fontSize() const;

		/// Set the font size of this widget
		void setFontSize(int fontSize) { mFontSize = fontSize; }
		/// Return whether the font size is explicitly specified for this widget
		bool hasFontSize() const { return mFontSize > 0; }

// 		/// Return a pointer to the cursor of the widget
// 		Cursor cursor() const { return mCursor; }
// 		/// Set the cursor of the widget
// 		void setCursor(Cursor cursor) { mCursor = cursor; }

		/// Check if the widget contains a certain position
		bool contains(const Point& p) const 
		{
			return PointinRect(p, GetRect());
		}

		/// Determine the widget located at the given position value (recursive)
		Widget* findWidget(const Point& p);

 		/// Compute the preferred size of the widget
 		virtual Point preferredSize(Renderer& renderer) const;

		/// Invoke the associated layout generator to properly place child widgets, if any
		virtual void performLayout(Renderer& renderer);

		/// Handle a mouse button event (default implementation: propagate to children)
		virtual bool mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers);

		/// Handle a mouse motion event (default implementation: propagate to children)
		virtual bool mouseMotionEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers);

		/// Handle a mouse drag event (default implementation: do nothing)
		virtual bool mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers) { return false; }

		/// Handle a mouse enter/leave event (default implementation: record this fact, but do nothing)
		virtual bool mouseEnterEvent(const Point& p, bool enter);

		/// Handle a mouse scroll event (default implementation: propagate to children)
		virtual bool scrollEvent(const Point& p, const Point& rel);

		/// Handle a focus change event (default implementation: record the focus status, but do nothing)
		virtual bool focusEvent(bool focused);

		/// Handle a keyboard event (default implementation: do nothing)
		virtual bool keyboardEvent(Scancode scan, Keycode key, bool down, Keymod modifiers) { return false; }

		/// Handle text input (UTF-32 format) (default implementation: do nothing)
		virtual bool keyboardCharacterEvent(const std::string& codepoint) { return false; }

		/// Draw the widget (and all child widgets)
		virtual void Render(Renderer& renderer, Point& offset);

	protected:

	protected:
		std::vector<Widget*> mChildren;
		ref<Theme> mTheme;
		ref<Layout> mLayout;
		Widget*		mParent{ nullptr };
		std::string	mId;
		std::string	mTooltip;
		Point		mPos; 
		Point		mSize;
		Point		mFixedSize;
		int			mFontSize{ 15 };
		bool mVisible{ true }; 
		bool mEnabled{ true };
		bool mFocused{ false }; 
		bool mMouseFocus{ false };
	};
}

#endif // GUIWidget_h__
