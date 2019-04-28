#include <memory>
#include <string>
#include <regex>
#include <assert.h>

#include "GameApp.h"
#include "GameState.h"

#include "SDLErrorReport.h"
#include "Common.h"

#include "GUIWidget.h"
#include "GUIWindow.h"
#include "GUIRootWidget.h"
#include "GUIButton.h"
#include "GUIPopupButton.h"
#include "GUITextBox.h"
#include "GUILayout.h"
#include "GUILabel.h"
#include "GUICheckBox.h"
#include "GUIslider.h"
#include "GUI_VScrollPanel.h"


bool InitialiseGUI(SGUI::RootWidget& mGUI, Window& wnd)
{
	/// Tell root Widget the screen / physical window size
	mGUI.setSize(Point{ wnd.GetWidth(), wnd.GetHeight() });

	Point winPos{ wnd.GetWidth() / 3, wnd.GetHeight() / 3 };
	Point winSize{ wnd.GetWidth() / 3, wnd.GetHeight() / 3 };
	auto pWin = new SGUI::Window(&mGUI, winPos, winSize);
	assert(pWin);
	pWin->setLayout(new SGUI::GroupLayout());

	new SGUI::Label(pWin, "Push buttons");

	SGUI::Button* pButton{ new SGUI::Button(pWin, "World") };
	assert(pButton);
	pButton->setTooltip("Super-duper extra looooooooooooong, gigantic, enormous, extra special Tooltip");

	SGUI::Button* pButton2{ new SGUI::Button(pWin, "HelloHelloHelloHelloEnd") };
	assert(pButton2);

	SGUI::PopupButton* pPopupBtn{ new SGUI::PopupButton(pWin, "Popup") };
	assert(pPopupBtn);

	pPopupBtn->setSide(SGUI::Popup::Left);

	SGUI::Popup* pPopup = pPopupBtn->popup();
	assert(pPopup);
	pPopup->setSize(Point{320,50});
//	pPopup->setLayout(new SGUI::GroupLayout());

	SGUI::VScrollPanel* vscroll = new SGUI::VScrollPanel(pPopup);
	SGUI::Widget* panel = new SGUI::Widget(vscroll); // new SGUI::Widget(pPopup); // pPopup; //  

	panel->setLayout(new SGUI::GroupLayout());
	new SGUI::Label(panel, "Arbitrary widgets can be placed here");
	SGUI::Button* pButton3{ new SGUI::Button(panel, "Inside Popup") };
	SGUI::CheckBox* pCheck{ new SGUI::CheckBox(panel, "Check Me!")};
	SGUI::Slider* pSlider{ new SGUI::Slider(panel) };

	SGUI::TextBox* textBox = new SGUI::TextBox(panel, "Text");
	textBox->setAlignment(SGUI::TextBox::Alignment::Center);
	textBox->setEditable(true);


//	std::function<void()> f = std::bind(&GUIState::ButtonCallback, this);
//	pButton2->setCallback(f);

	textBox= new SGUI::TextBox(pWin, u8"みんなのにほんご");
	textBox->setAlignment(SGUI::TextBox::Alignment::Left);
	textBox->setEditable(true);
//	std::function<bool(const std::string& str)> fedit = [this](const std::string& str) { return EditCallBack(str); };
//	textBox->setCallback(fedit);

	textBox = new SGUI::IntBox<int>(pWin, 50);
	textBox->setAlignment(SGUI::TextBox::Alignment::Right);
	textBox->setEditable(true);
	textBox->setSpinnable(true);

	textBox = new SGUI::TextBox(pWin, "VeryVeryVeryVeryVeryLongText");
	textBox->setAlignment(SGUI::TextBox::Alignment::Right);
	textBox->setEditable(true);

	/// Sets default theme and performs the layout
	mGUI.performLayout(wnd.GetRenderer());

	return true;
}

class GUIState : public GameState
{
public:
	GUIState() {}
	~GUIState() { }

	bool Initialise() override
	{
		assert(m_pWnd);
		return InitialiseGUI(mGUI, *m_pWnd);
	};
	void Update(double dt)			override {}

	QUITRESPONSE QuitDialog() override { return TwoOptionQuitDialog(); }

	void Render(Renderer& renderer)	override
	{
		mGUI.Render(renderer, Point{ 0,0 });
	}

	void CleanUp() override {}

	bool OnEvent(SDL_Event& Event) override
	{
		if (mGUI.GUIEventHandler(Event))
			return true;
			
		return EventHandler::OnEvent(Event);
	}

private:
	SGUI::RootWidget mGUI; /// Root widget that stores the whole of this scenes GUI
};

bool CreateSDLWindow(Window& window, WindowCreationParams& createParam)
{
	/// Create a window, report error if window not created
	if (!window.Create("Test Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		createParam.iWidth, createParam.iHeight,
		createParam.SetWindowCreateFlags()))
	{
		MsgBoxErrorReport("Window Creation Failed.\n", SDL_GetError());
		return false;
	}

	/// Creates a renderer and clears the window
	if (!window.CreateRenderer(createParam.SetRendererCreateFlags()))
	{
		MsgBoxErrorReport("Renderer Creation Failed.\n", SDL_GetError());
		return false;
	}

	return true;
}


int main(int argc, char *argv[])
{
	InitSDL();

	WindowCreationParams createParam;
	Window window;

	if (!CreateSDLWindow(window, createParam))
		return -1;

	{ /// Scope to make sure mGUI is destroyed before Window 
		SGUI::RootWidget mGUI;

		InitialiseGUI(mGUI, window);

		/// Setup event handling and rendering lambda functions
		std::function<bool(SDL_Event& Event)> f_eventHandle = [&mGUI](SDL_Event& Event) { return mGUI.GUIEventHandler(Event); };
		std::function<void()> f_render = [&mGUI, &window]() 
		{ 
			/// If we have valid window & renderer then render the frame
			if (window.CanRender())
			{
				mGUI.Render(window.GetRenderer(), Point{ 0,0 });
				window.Present();
			}
		};
		MainLoop(f_eventHandle, f_render);
	}

	window.Release();
	ShutdownSDL();
	return 0;

// 	GameApp app("GUI Dev App");
// 	app.Register<GUIState>("GUISTATE");
// 
// 	return app.Execute(createParam, "GUISTATE");
}
