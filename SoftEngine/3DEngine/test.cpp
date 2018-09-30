#include <iostream>
#include<math.h>
#include <objidl.h>
#include <gdiplus.h>
#include<windowsx.h>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include"pipeline.h"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

KZEngine::KZRenderList* KZEngine::KZRenderList::r_instance_ = NULL;
KZEngine::KZPipeLine*  KZEngine::KZPipeLine::p_instance_ = NULL;
float g_average_fps = 0.0f;

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT_PTR iTimerID, DWORD dwTime)
{
	KZEngine::KZPipeLine::GetInstance()->SetGameFps();
	return;
}

void GameMain() {
	KZEngine::KZPipeLine::GetInstance()->FrameUpdate();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	switch (iMsg)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		//w
		case 0x57: 
		{
			KZEngine::KZPipeLine::GetInstance()->main_camera_.ProcessKeyboard(KZEngine::CameraMovement::FORWARD, 0.02f);
			break;
		}
		//a
		case 0x41: 
		{
			KZEngine::KZPipeLine::GetInstance()->main_camera_.ProcessKeyboard(KZEngine::CameraMovement::LEFT, 0.02f);
			break;
		}
		//s
		case 0x53: 
		{
			KZEngine::KZPipeLine::GetInstance()->main_camera_.ProcessKeyboard(KZEngine::CameraMovement::BACKWARD, 0.02f);
			break;
		}
		//d
		case 0x44: 
		{
			KZEngine::KZPipeLine::GetInstance()->main_camera_.ProcessKeyboard(KZEngine::CameraMovement::RIGHT, 0.02f);
			break;
		}
		//0
		case 0x60 :
		{
			KZEngine::KZPipeLine::GetInstance()->ChangeLight(0);
			break;
		}
		//1
		case 0x61:
		{
			KZEngine::KZPipeLine::GetInstance()->ChangeLight(1);
			break;
		}
		//2
		case 0x62:
		{
			KZEngine::KZPipeLine::GetInstance()->ChangeLight(2);
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (!KZEngine::KZPipeLine::GetInstance()->first_mouse_) {
			KZEngine::KZPipeLine::GetInstance()->first_mouse_ = true;
			KZEngine::KZPipeLine::GetInstance()->last_pos_x_ = GET_X_LPARAM(lParam);
			KZEngine::KZPipeLine::GetInstance()->last_pos_y_ = GET_Y_LPARAM(lParam);
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (KZEngine::KZPipeLine::GetInstance()->first_mouse_) 
		{
			int x_pos = GET_X_LPARAM(lParam);
			int y_pos = GET_Y_LPARAM(lParam);
			float x_offset = (float)(x_pos - KZEngine::KZPipeLine::GetInstance()->last_pos_x_);
			float y_offset = (float)(y_pos - KZEngine::KZPipeLine::GetInstance()->last_pos_y_);
			KZEngine::KZPipeLine::GetInstance()->main_camera_.ProcessMouseMovement(x_offset, y_offset);
			KZEngine::KZPipeLine::GetInstance()->last_pos_x_ = x_pos;
			KZEngine::KZPipeLine::GetInstance()->last_pos_y_ = y_pos;
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		KZEngine::KZPipeLine::GetInstance()->first_mouse_ = false;
		break;
	}
	case WM_CREATE:
	{
		SetTimer(hwnd, 0, 20, NULL);
		SetTimer(hwnd, 1, 1000, TimerProc);
		return 0;
		break;
	}
	case WM_TIMER:
	{
		if (wParam == 0) {
			InvalidateRect(hwnd, NULL, FALSE);
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT pt_str;
		//获得系统绘图设备  
		HDC hdc = BeginPaint(hwnd, &pt_str);
		EndPaint(hwnd, &pt_str);
		break;
	}
	case WM_CLOSE:
	{
		if (IDYES == MessageBox(hwnd, L"是否退出", L"Hello", MB_YESNO)) {
			DestroyWindow(hwnd);
		}
		break;
	}
	case WM_DESTROY:
	{
		KillTimer(hwnd, 0);
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	}
}

bool IniWindow(HINSTANCE hinst, int nCmdShow) {
	HWND hwnd = CreateWindowEx(NULL, L"Hello", L"Engine", WS_OVERLAPPEDWINDOW, 0, 0, KZEngine::KZPipeLine::GetInstance()->GetWindowWidth() + 16, KZEngine::KZPipeLine::GetInstance()->GetWindowHeight() + 39, NULL, NULL, hinst, NULL);
	KZEngine::KZPipeLine::GetInstance()->SetWindowHwnd(hwnd);
	if (!hwnd) {
		return false;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return true;
}

bool IniWindowClass(HINSTANCE hinst) {
	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(wndClass);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(hinst, IDC_ARROW);
	wndClass.hIconSm = LoadIcon(hinst, IDI_APPLICATION);
	wndClass.hIcon = LoadIcon(hinst, IDI_ERROR);
	wndClass.hInstance = hinst;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = L"Hello";
	wndClass.lpszMenuName = NULL;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	return (bool)RegisterClassEx(&wndClass);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow) {
	MSG msg;
	GdiplusStartupInput gdiplus_startup_input;
	ULONG_PTR           gdi_plus_token;

	//开启gdi+
	GdiplusStartup(&gdi_plus_token,&gdiplus_startup_input, NULL);

	//KZMath::KZQuat quat;
	//quat.SetFromVector3DTheta(KZMath::KZVector3D(0, 0, 1), 90);
	KZEngine::KZPipeLine* p_instance = KZEngine::KZPipeLine::GetInstance();
	//p_instance->CreateCube();
	p_instance->CreateSphere(0.8f, 12, 16, KZEngine::Color(255, 255, 0), false, KZMath::KZVector4D<float>(0, 0, -5));
	//p_instance->CreateCylinder(1.0f, 1.0f, 2.0f, 4, 8, KZEngine::Color(255, 255, 0), false, KZMath::KZVector4D<float>(0, -2, -2));
	p_instance->Create_Terrain(10, 10, 4, "height_map_texture/cs1.bmp", "terrain_map_texture/terrain3.jpg", KZEngine::Color(255, 255, 255), false, KZMath::KZVector4D<float>(0, -4, -10), KZMath::KZQuat::ZERO);
	//p_instance->CreatePyramid();
	if (!IniWindowClass(hInstance)) {
		return false;
	}
	if (!IniWindow(hInstance, cmdShow)) {
		return false;
	}
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		GameMain();
	}

	//关闭gdi+
	GdiplusShutdown(gdi_plus_token);
	_CrtDumpMemoryLeaks();
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	return (int)msg.wParam;
}