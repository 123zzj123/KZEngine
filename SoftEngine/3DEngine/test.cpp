#include <iostream>
#include<math.h>
#include <objidl.h>
#include <gdiplus.h>
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
	case WM_CREATE:
	{
		SetTimer(hwnd, 0, 16.6, NULL);
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
		//创建辅助绘图设备  
		HDC mem_dc = CreateCompatibleDC(hdc);
		uint32_t w = KZEngine::KZPipeLine::GetInstance()->GetWindowWidth();
		uint32_t h = KZEngine::KZPipeLine::GetInstance()->GetWindowHeight();

		Gdiplus::Color color(255, 255, 255, 255);

		//创建GDI掩码位图（画布）
		HBITMAP bmp_back = CreateCompatibleBitmap(hdc, KZEngine::KZPipeLine::GetInstance()->GetWindowWidth(), KZEngine::KZPipeLine::GetInstance()->GetWindowHeight());

		//创建GDI+掩码位图（画布）
		Gdiplus::Bitmap bitmap(w, h, 3 * w, PixelFormat24bppRGB, (BYTE*)KZEngine::KZPipeLine::GetInstance()->GetFrameBuffer());
		bitmap.GetHBITMAP(color,&bmp_back);
		//将画布贴到绘图设备上
		SelectObject(mem_dc, bmp_back);
		//复制到系统设备上显示  
		bool success = BitBlt(hdc, 0, 0, KZEngine::KZPipeLine::GetInstance()->GetWindowWidth(), KZEngine::KZPipeLine::GetInstance()->GetWindowHeight(), mem_dc, 0, 0, SRCCOPY);
		DeleteObject(bmp_back);//释放位图资源  
		DeleteDC(mem_dc);//释放辅助绘图设备  
		ReleaseDC(hwnd, hdc);//归还系统绘图设备

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

	return RegisterClassEx(&wndClass);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow) {
	MSG msg;
	GdiplusStartupInput gdiplus_startup_input;
	ULONG_PTR           gdi_plus_token;

	//开启gdi+
	GdiplusStartup(&gdi_plus_token,&gdiplus_startup_input, NULL);

	KZEngine::KZPipeLine* p_instance = KZEngine::KZPipeLine::GetInstance();
	p_instance->CreateCube();
	p_instance->CreatePyramid();
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
	return msg.wParam;
}