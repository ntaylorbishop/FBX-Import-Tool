#include "Engine/Renderer/DeviceWindow.hpp"
#include "Game/GameApp/TheApp.hpp"
#include "Game/GameApp/TheGame.hpp"
#include "Engine/General/Logger/TheLogger.hpp"
#include "Engine/Renderer/Renderer/BeirusRenderer.hpp"
#include "Engine/General/Core/WindowHandler.hpp"

STATIC TheApp* TheApp::s_theApp = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INIT SHUTDOWN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheApp::Initialize(HINSTANCE applicationInstanceHandle, int nCmdShow) {

	//DeviceWindow::CreateDeviceWindow(applicationInstanceHandle);

	s_theApp = new TheApp();
	BeirusEngine::Initialize();
	//TheGame::Initialize(applicationInstanceHandle, nCmdShow);
}


//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheApp::Shutdown() {

	delete s_theApp;
	s_theApp = nullptr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOOP
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
STATIC void TheApp::RunFrame() {

	s_theApp->m_timer.m_start = Time::GetCurrentTimeSeconds();

	s_theApp->RunMessagePump();
	s_theApp->Update();
	s_theApp->Render();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//STRUCTORS
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
TheApp::TheApp() 
	: m_numTimesPrinted(0)
{ }


//---------------------------------------------------------------------------------------------------------------------------
TheApp::~TheApp() { }



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//INTERNAL LOOP
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------------------------------
void TheApp::RunMessagePump() {

	MSG queuedMessage;

	for (EVER) {

		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);

		if (!wasMessagePresent) {
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage);
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void TheApp::Update() {

	static double s_timeLastFrameBegan = Time::GetCurrentTimeSeconds();

	double timeThisFrameBegan = Time::GetCurrentTimeSeconds();
	float deltaSeconds = static_cast<float>(timeThisFrameBegan - s_timeLastFrameBegan); //In seconds


	s_timeLastFrameBegan = timeThisFrameBegan;

	BeirusEngine::Update(deltaSeconds);
	InputSystem::Update(deltaSeconds);

	m_age += deltaSeconds;

	if (m_age > 0.25f) {
		m_age = 0.f;
	}
}


//---------------------------------------------------------------------------------------------------------------------------
void TheApp::Render() {

	//TheGame::Get()->Render();
	//BeirusEngine::Render();

	s_theApp->m_timer.m_end = Time::GetCurrentTimeSeconds();

	if (m_age == 0.f) {
		m_frameTimeToPrint = s_theApp->m_timer.GetDifference();
		m_frameTimeToPrint = 1.0 / m_frameTimeToPrint;
	}
	//String timeStr = StringUtils::Stringf("%.0f", m_frameTimeToPrint);
	//Font::DrawText2DWithDefaultFont(Vector2(10.f, 800.f), timeStr, 0.4f, RGBA::WHITE);

	//SwapBuffers(g_displayDeviceContext);
}