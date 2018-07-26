#include<Windows.h>
#include<math.h>
#include"pipeline.h"

KZEngine::KZRenderList* KZEngine::KZRenderList::r_instance_ = NULL;
KZEngine::KZPipeLine*  KZEngine::KZPipeLine::p_instance_ = NULL;
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	switch (iMsg)
	{
	case WM_CREATE:
	{
		return 0;
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT pt_str;
		//获得系统绘图设备  
		HDC hdc = BeginPaint(hwnd, &pt_str);
		//创建辅助绘图设备  
		HDC mem_dc = CreateCompatibleDC(0);

		int w = KZEngine::KZPipeLine::GetInstance()->view_width;
		int h = KZEngine::KZPipeLine::GetInstance()->view_height;
		//创建掩码位图（画布）
		HBITMAP bmp_back = CreateCompatibleBitmap(hdc, KZEngine::KZPipeLine::GetInstance()->view_width, KZEngine::KZPipeLine::GetInstance()->view_height);
		//将画布贴到绘图设备上
		SelectObject(mem_dc, bmp_back); 

		//擦除背景
		//客户端区域结构  
		RECT rc_client;
		//获得客户区域  
		GetClientRect(hwnd, &rc_client);
		//获得库存物体，白色画刷。
		HBRUSH brush_temp = (HBRUSH)GetStockObject(GRAY_BRUSH);  
		FillRect(mem_dc, &rc_client, brush_temp);//填充客户区域。  

		int rows = KZEngine::KZPipeLine::GetInstance()->view_width;
		int cols = KZEngine::KZPipeLine::GetInstance()->view_height;
		for (int r = 0; r<rows; ++r)
		{
			for (int c = 0; c<cols; ++c)
			{
				if (KZEngine::KZPipeLine::GetInstance()->z_buffer_[r][c] != -2) {
					COLORREF res = SetPixelV(mem_dc, r, c, RGB(255, 0, 0));
					if (res == 0) {
						cout << "fail to draw in " << r << " , " << c << endl;
					}
				}
			}
		}

		//复制到系统设备上显示  
		BitBlt(hdc, 0, 0, KZEngine::KZPipeLine::GetInstance()->view_width, KZEngine::KZPipeLine::GetInstance()->view_height, mem_dc, 0, 0, SRCCOPY);
		//释放位图资源 
		DeleteObject(bmp_back);
		//释放辅助绘图设备 
		DeleteDC(mem_dc); 
		EndPaint(hwnd, &pt_str);
		break;
	}
	case WM_CLOSE:
	{
		if (IDYES == MessageBox(hwnd, "是否退出", "Hello", MB_YESNO)) {
			DestroyWindow(hwnd);
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	}
}

bool IniWindow(HINSTANCE hinst, int nCmdShow) {
	HWND hwnd;
	hwnd = CreateWindowEx(NULL, "Hello", "Engine", WS_OVERLAPPEDWINDOW, 0, 0, KZEngine::KZPipeLine::GetInstance()->view_width, KZEngine::KZPipeLine::GetInstance()->view_height, NULL, NULL, hinst, NULL);
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
	wndClass.lpszClassName = "Hello";
	wndClass.lpszMenuName = NULL;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClassEx(&wndClass);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow) {
	MSG msg;
	KZEngine::KZPipeLine* p_instance = KZEngine::KZPipeLine::GetInstance();
	p_instance->CreateCube();
	p_instance->FrameUpdate();
	if (!IniWindowClass(hInstance)) {
		return false;
	}
	if (!IniWindow(hInstance, cmdShow)) {
		return false;
	}
	while (GetMessage(&msg, 0, 0 ,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}