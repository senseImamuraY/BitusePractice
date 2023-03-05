#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>
#include "CD3DXSKINMESH.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//�萔��`
#define NAME L"�X�L���A�j���[�V�������b�V�� X"
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����

//�O���[�o���ϐ�
HWND g_hWnd=NULL;
ID3D11Device* g_pDevice;
ID3D11DeviceContext* g_pDeviceContext;
IDXGISwapChain* g_pSwapChain;
ID3D11RenderTargetView* g_pRTV;
ID3D11Texture2D* g_pDS;
ID3D11DepthStencilView* g_pDSV;

CD3DXSKINMESH* g_pMesh;

//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void App();
HRESULT MakeShader(LPSTR ,LPSTR ,LPSTR ,void** ,ID3DBlob** );
HRESULT InitDirect3D();

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
	//Direct3D�̍쐬
	if(FAILED(InitDirect3D())) return 1;
	
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
//HRESULT InitDirect3D()
//
HRESULT InitDirect3D()
{
	//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd,sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width=WINDOW_WIDTH;
	sd.BufferDesc.Height=WINDOW_HEIGHT;
	sd.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator=60;
	sd.BufferDesc.RefreshRate.Denominator=1;
	sd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow=g_hWnd;
	sd.SampleDesc.Count=1;
	sd.Windowed=TRUE;
	
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;
	
	D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,0,&pFeatureLevels
		,1,D3D11_SDK_VERSION,&sd,&g_pSwapChain,&g_pDevice,pFeatureLevel,&g_pDeviceContext);

	//�o�b�N�o�b�t�@�[�̃����_�[�^�[�Q�b�g�r���[(RTV)���쐬
	ID3D11Texture2D *pBack;
	g_pSwapChain->GetBuffer(0,__uuidof( ID3D11Texture2D ),(LPVOID*)&pBack);

	g_pDevice->CreateRenderTargetView( pBack, NULL, &g_pRTV );
	pBack->Release();	

	//�f�v�X�X�e���V���r���[(DSV)���쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	g_pDevice->CreateTexture2D( &descDepth, NULL, &g_pDS);

	g_pDevice->CreateDepthStencilView(g_pDS, NULL, &g_pDSV);

	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pDeviceContext->RSSetViewports(1,&vp);

	//���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc,sizeof(rdc));
	rdc.CullMode=D3D11_CULL_NONE;
	rdc.FillMode=D3D11_FILL_SOLID;
	ID3D11RasterizerState* pIr=NULL;
	g_pDevice->CreateRasterizerState(&rdc,&pIr);
	g_pDeviceContext->RSSetState(pIr);

	//�X�L�����b�V���N���X�ɕK�v�ȏ���n��������
	CD3DXSKINMESH_INIT si;
	si.hWnd=g_hWnd;
	si.pDevice=g_pDevice;
	si.pDeviceContext=g_pDeviceContext;

	g_pMesh=new CD3DXSKINMESH;
	g_pMesh->Init(&si);
	g_pMesh->CreateFromX("hand_tex.x");

	return S_OK;
}

//
//void App()
//
void App()
{
	//�����_�[�^�[�Q�b�g�r���[�ƃf�v�X�X�e���V���r���[���Z�b�g
	g_pDeviceContext->OMSetRenderTargets(1,&g_pRTV,g_pDSV);
	//��ʃN���A
	float ClearColor[4] = {0,0,0.5,1};// �N���A�F�쐬�@RGBA�̏�
	g_pDeviceContext->ClearRenderTargetView(g_pRTV,ClearColor);//�J���[�o�b�t�@�N���A
	g_pDeviceContext->ClearDepthStencilView(g_pDSV,D3D11_CLEAR_DEPTH,1.0f,0);//�f�v�X�X�e���V���o�b�t�@�N���A

	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	//���[���h�g�����X�t�H�[��
	D3DXMatrixIdentity(&World);
	// �r���[�g�����X�t�H�[��
	D3DXVECTOR3 vEyePt( 0.0f, 7.0f,-7.0f ); //���_�ʒu
	D3DXVECTOR3 vLookatPt( 0.0f, 1.0f, 0.0f );//�����ʒu
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );//����ʒu
	D3DXMatrixLookAtLH( &View, &vEyePt, &vLookatPt, &vUpVec );
	// �v���W�F�N�V�����g�����X�t�H�[��
	D3DXMatrixPerspectiveFovLH( &Proj, D3DX_PI/4, (FLOAT)WINDOW_WIDTH/(FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f );
		
	//�����_�����O
	g_pMesh->m_View=View;
	g_pMesh->m_Eye=vEyePt;
	g_pMesh->m_Proj=Proj;
	
	g_pMesh->Render();

	g_pSwapChain->Present(0,0);//��ʍX�V
}