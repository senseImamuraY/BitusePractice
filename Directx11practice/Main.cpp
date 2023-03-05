#include <windows.h>
#include <d3d11.h>
#include "/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/D3DX10.h"
#include "/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/D3DX11.h"
#include <d3dCompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//定数定義
#define NAME L"クアッド描画"
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ

//グローバル変数
HWND g_hWnd = NULL;
ID3D11Device* g_pDevice;
ID3D11DeviceContext* g_pDeviceContext;
IDXGISwapChain* g_pSwapChain;
ID3D11RenderTargetView* g_pRTV;
ID3D11Texture2D* g_pDS;
ID3D11DepthStencilView* g_pDSV;
ID3D11InputLayout* g_pVertexLayout;
ID3D11VertexShader* g_pVertexShader;
ID3D11PixelShader* g_pPixelShader;
ID3D11Buffer* g_pConstantBuffer;
ID3D11Buffer* g_pVertexBuffer;

//頂点の構造体
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置
};

//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体
struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
};

//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void App();
HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);
HRESULT InitDirect3D();

//
//エントリー関数
//
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	// ウィンドウの作成
	WNDCLASSEX  wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = NAME;
	if (!RegisterClassEx(&wc)) return 1;
	g_hWnd = CreateWindow(NAME, NAME, WS_OVERLAPPEDWINDOW, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInst, 0);
	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);
	//Direct3Dの作成
	if (FAILED(InitDirect3D())) return 1;

	// メッセージループ
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
		}
		else
		{
			App();
		}
	}
	return 0;
}

//
//ウィンドウプロシージャー関数
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
//
//HRESULT MAIN::MakeShader(LPSTR szFileName,LPSTR szFuncName,LPSTR szProfileName,void** ppShader,ID3DBlob** ppBlob)
//hlslファイルを読み込みシェーダーを作成する
HRESULT MakeShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob)
{
	ID3DBlob* pErrors = NULL;
	if (FAILED(D3DX11CompileFromFileA(szFileName, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
	{
		char* p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);
	if (strcmp(szProfile, "vs") == 0)//Vertex Shader
	{
		g_pDevice->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader);
	}
	if (strcmp(szProfile, "ps") == 0)//Pixel Shader
	{
		g_pDevice->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader);
	}
	if (strcmp(szProfile, "gs") == 0)//Geometry Shader
	{
		g_pDevice->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader);
	}
	if (strcmp(szProfile, "hs") == 0)//Hull Shader
	{
		g_pDevice->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader);
	}
	if (strcmp(szProfile, "ds") == 0)//Domain Shader
	{
		g_pDevice->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader);
	}
	if (strcmp(szProfile, "cs") == 0)//Compute Shader
	{
		g_pDevice->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader);
	}
	return S_OK;
}
//
//HRESULT InitDirect3D()
//
HRESULT InitDirect3D()
{
	//デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &pFeatureLevels
		, 1, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pDevice, pFeatureLevel, &g_pDeviceContext);

	//バックバッファーのレンダーターゲットビュー(RTV)を作成
	ID3D11Texture2D* pBack;
	g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);

	g_pDevice->CreateRenderTargetView(pBack, NULL, &g_pRTV);
	pBack->Release();

	//デプスステンシルビュー(DSV)を作成
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
	g_pDevice->CreateTexture2D(&descDepth, NULL, &g_pDS);

	g_pDevice->CreateDepthStencilView(g_pDS, NULL, &g_pDSV);

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pDeviceContext->RSSetViewports(1, &vp);

	ID3DBlob* pCompiledShader = NULL;


	// 仕様回避
	CHAR Y[] = "Shader.hlsl";
	LPSTR ShaderHlsl = Y;
	CHAR A[] = "VSMain";
	LPSTR VS = A;
	CHAR B[] = "vs_5_0";
	LPSTR vs_5_0 = B;
	CHAR C[] = "PSMain";
	LPSTR PS = C;
	CHAR D[] = "ps_5_0";
	LPSTR ps_5_0 = D;

	//バーテックスシェーダー作成
	MakeShader(ShaderHlsl, VS, vs_5_0, (void**)&g_pVertexShader, &pCompiledShader);;
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	g_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &g_pVertexLayout);

	//ピクセルシェーダー作成
	MakeShader(ShaderHlsl, PS, ps_5_0, (void**)&g_pPixelShader, &pCompiledShader);

	//コンスタントバッファー作成
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	g_pDevice->CreateBuffer(&cb, NULL, &g_pConstantBuffer);

	//トライアングル作成
	//バーテックスバッファー作成
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-0.5,-0.5,0),//頂点1	
		D3DXVECTOR3(-0.5,0.5,0), //頂点2
		D3DXVECTOR3(0.5,-0.5,0),  //頂点3
		D3DXVECTOR3(0.5,0.5,0), //頂点4	
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	g_pDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);

	return S_OK;
}

//
//void App()
//
void App()
{
	//レンダーターゲットビューとデプスステンシルビューをセット
	g_pDeviceContext->OMSetRenderTargets(1, &g_pRTV, g_pDSV);
	//画面クリア
	float ClearColor[4] = { 0,0,0.5,1 };// クリア色作成　RGBAの順
	g_pDeviceContext->ClearRenderTargetView(g_pRTV, ClearColor);//カラーバッファクリア
	g_pDeviceContext->ClearDepthStencilView(g_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);//デプスステンシルバッファクリア

	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	//ワールドトランスフォーム
	D3DXMatrixIdentity(&World);
	// ビュートランスフォーム
	D3DXVECTOR3 vEyePt(0.0f, 1.0f, -2.0f); //視点位置
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMatrixLookAtLH(&View, &vEyePt, &vLookatPt, &vUpVec);
	// プロジェクショントランスフォーム
	D3DXMatrixPerspectiveFovLH(&Proj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f);
	//使用するシェーダーのセット
	g_pDeviceContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pDeviceContext->PSSetShader(g_pPixelShader, NULL, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(g_pDeviceContext->Map(g_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド、カメラ、射影行列を渡す
		cb.mWVP = World * View * Proj;
		D3DXMatrixTranspose(&cb.mWVP, &cb.mWVP);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		g_pDeviceContext->Unmap(g_pConstantBuffer, 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	g_pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pDeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット
	g_pDeviceContext->IASetInputLayout(g_pVertexLayout);
	//プリミティブ・トポロジーをセット
	g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//プリミティブをレンダリング
	g_pDeviceContext->Draw(4, 0);

	g_pSwapChain->Present(0, 0);//画面更新
}