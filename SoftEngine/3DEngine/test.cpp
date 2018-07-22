#include<Windows.h>
#include<math.h>

int g_nWidth = 800;
int g_nHeight = 800;
int dw = 30;
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
		PAINTSTRUCT ptStr;
		HDC hDC = BeginPaint(hwnd, &ptStr);    //获得系统绘图设备  

		HDC memDC = CreateCompatibleDC(0);  //创建辅助绘图设备  

		HBITMAP bmpBack = CreateCompatibleBitmap(hDC, g_nWidth, g_nHeight);//创建掩码位图（画布）  
		SelectObject(memDC, bmpBack);    //将画布贴到绘图设备上  

		HPEN penBack = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));//创建画笔  
		SelectObject(memDC, penBack);    //将画笔选到绘图设备上  

		HBRUSH brushBack = CreateSolidBrush(RGB(255, 255, 255));//创建画刷  
		SelectObject(memDC, brushBack);  //将画刷选到绘图设备上  

										 //擦除背景  
		RECT rcClient;//区域结构  
		GetClientRect(hwnd, &rcClient);//获得客户区域  
		HBRUSH brushTemp = (HBRUSH)GetStockObject(BLACK_BRUSH);//获得库存物体，白色画刷。  
		FillRect(memDC, &rcClient, brushTemp);//填充客户区域。  
		HBRUSH brushObj = CreateSolidBrush(RGB(0, 255, 0));

		int rows = g_nHeight / dw;
		int cols = g_nWidth / dw;
		for (int r = 0; r<rows; ++r)
		{
			for (int c = 0; c<cols; ++c)
			{
				if (r == c)
				{
					SelectObject(memDC, brushObj);
				}
				else
				{
					SelectObject(memDC, brushBack);
				}
				Rectangle(memDC, c*dw, r*dw, (c + 1)*dw, (r + 1)*dw);
			}
		}

		DeleteObject(brushObj);
		//////////////////////////////////////////////////////////////////////////  
		BitBlt(hDC, 0, 0, g_nWidth, g_nHeight, memDC, 0, 0, SRCCOPY);//复制到系统设备上显示  
		DeleteObject(penBack);  //释放画笔资源  
		DeleteObject(brushBack);//释放画刷资源  
		DeleteObject(bmpBack);  //释放位图资源  
		DeleteDC(memDC);        //释放辅助绘图设备  
		EndPaint(hwnd, &ptStr);
		dw ++;
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
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, iMsg, wParam, lParam);
	}
}

bool IniWindow(HINSTANCE hinst, int nCmdShow) {
	HWND hwnd;
	hwnd = CreateWindowEx(NULL, L"Hello", L"Engine", WS_OVERLAPPEDWINDOW, 0, 0, g_nWidth, g_nHeight, NULL, NULL, hinst, NULL);
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