//#include "DxLib.h"
//
//void Kao(int mouth)
//{
//	// 顔を描く処理
//	DrawTriangle(320 - 200, 400, 320 + 200, 400, 320, 600, GetColor(0, 122, 102), TRUE);	// 身体
//	DrawCircle(320, 240, 200, GetColor(255, 216, 101), TRUE);	// 顔
//	DrawOval(320, 140, 200, 100, GetColor(153, 102, 0), TRUE);	// 髪の毛
//	DrawCircle(320 - 70, 300, 10, GetColor(20, 10, 0), TRUE);	// 目
//	DrawCircle(320 + 70, 300, 10, GetColor(20, 10, 0), TRUE);	// 目
//	DrawBox(320 - 40, 360, 320 + 40 + 1, 370 + mouth, GetColor(224, 51, 51), TRUE);
//}
//
//// プログラムは WinMain から始まります
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//	ChangeWindowMode(TRUE);		// TRUEにするとウインドウモードで起動
//
//	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
//	{
//		return -1;			// エラーが起きたら直ちに終了
//	}
//
//
//	// 一枚目の絵
//	Kao(0);		// 顔を描画。口（閉じてる）
//	WaitKey();	// 何か押すまで停止
//
//	// 二枚目の絵
//	ClearDrawScreen();	// 画面クリア
//	Kao(30);	// 顔を描画。口（開ける）
//	WaitKey();	// 何か押すまで停止
//
//	// 二枚目の絵
//	ClearDrawScreen();	// 画面クリア
//	Kao(0);		// 顔を描画。口（閉じてる）
//	WaitKey();	// 何か押すまで停止
//
//	DxLib_End();				// ＤＸライブラリ使用の終了処理
//
//	return 0;				// ソフトの終了 
//}
