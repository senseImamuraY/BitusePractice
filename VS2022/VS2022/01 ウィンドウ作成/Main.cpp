#include <windows.h>

//�萔��`
#define NAME L"�E�B���h�E"
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����

//�O���[�o���ϐ�
HWND g_hWnd=NULL;
//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void App();

//
//�G���g���[�֐�
//
INT WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR szStr,INT iCmdShow)
{
	// �E�B���h�E�̍쐬
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
	
	// ���b�Z�[�W���[�v
	MSG msg={0};
	while(msg.message!=WM_QUIT)	PeekMessage(&msg,NULL,0,0,PM_REMOVE) ? DispatchMessage(&msg) : App();

	return 0;
}

//
//�E�B���h�E�v���V�[�W���[�֐�
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