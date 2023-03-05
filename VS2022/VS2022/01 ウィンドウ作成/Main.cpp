#include <windows.h>

//定数定義
#define NAME L"ウィンドウ"
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ

//グローバル変数
HWND g_hWnd=NULL;
//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void App();

//
//エントリー関数
//
INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR szStr,INT iCmdShow)
{
	// ウィンドウの作成
	WNDCLASSEX  wc={0};
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = NAME;
	if(!RegisterClassEx(&wc)) return 1;
	g_hWnd = CreateWindow(NAME,NAME,WS_OVERLAPPEDWINDOW,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,0,0,hInst,0);
	ShowWindow(g_hWnd,SW_SHOW);
	UpdateWindow(g_hWnd);
	
	// メッセージループ
	MSG msg={0};
	while(msg.message!=WM_QUIT)	PeekMessage(&msg,NULL,0,0,PM_REMOVE) ? DispatchMessage(&msg) : App();

	return 0;
}

//
//ウィンドウプロシージャー関数
//
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch(iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd,iMsg,wParam,lParam);
}

//
//
//
void App()
{
}