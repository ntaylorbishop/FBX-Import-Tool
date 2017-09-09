/*
#pragma once

#include "Engine/General/Core/BeirusEngine.hpp"
#include "Engine/General/Core/EngineCommon.hpp"
#include "Engine/Renderer/SpriteRenderer/TheSpriteRenderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/Camera3D.hpp"
#include <windows.h>

class TheGame {
public:
	//INIT DESTROY
	static void Initialize(HINSTANCE applicationInstanceHandle, int nCmdShow);
	static void Shutdown();

	static TheGame* Get();

	//UPDATE
	void Update(float deltaSeconds);

	//RENDER
	void Render();

private:
	//STRUCTORS INIT
	TheGame(HINSTANCE applicationInstanceHandle, int nCmdShow);
	~TheGame();

	void CreateShaderProgram();
	void CreateMesh();

	Camera3D m_playerCamera;

	static TheGame* s_theGame;
};*/