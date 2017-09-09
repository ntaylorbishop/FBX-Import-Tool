#pragma once

#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/SpriteRenderer/TheSpriteRenderer.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Input/InputSystem.hpp"

#define EVER ;;

class Timer {
public:
	Timer() : m_start(0.0), m_end(0.0) { }

	double GetDifference() { return m_end - m_start; }

	double m_start = 0.0;
	double m_end = 0.0;
};

class TheApp {
public:
	//INIT SHUTDOWN
	static void Initialize(HINSTANCE applicationInstanceHandle, int nCmdShow);
	static void Shutdown();

	//LOOP
	static void RunFrame();


private:
	//STRUCTORS
	TheApp();
	~TheApp();

	//LOOP
	void RunMessagePump();
	void Update();
	void Render();

	uint32_t m_numTimesPrinted = 0;
	Timer m_timer;

	float m_age = 0.f;
	double m_frameTimeToPrint = 0.0;

	static TheApp* s_theApp;
};