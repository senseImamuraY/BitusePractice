//#include "DxLib.h"
//
//void Kao(int mouth)
//{
//	// ���`������
//	DrawTriangle(320 - 200, 400, 320 + 200, 400, 320, 600, GetColor(0, 122, 102), TRUE);	// �g��
//	DrawCircle(320, 240, 200, GetColor(255, 216, 101), TRUE);	// ��
//	DrawOval(320, 140, 200, 100, GetColor(153, 102, 0), TRUE);	// ���̖�
//	DrawCircle(320 - 70, 300, 10, GetColor(20, 10, 0), TRUE);	// ��
//	DrawCircle(320 + 70, 300, 10, GetColor(20, 10, 0), TRUE);	// ��
//	DrawBox(320 - 40, 360, 320 + 40 + 1, 370 + mouth, GetColor(224, 51, 51), TRUE);
//}
//
//// �v���O������ WinMain ����n�܂�܂�
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//	ChangeWindowMode(TRUE);		// TRUE�ɂ���ƃE�C���h�E���[�h�ŋN��
//
//	if (DxLib_Init() == -1)		// �c�w���C�u��������������
//	{
//		return -1;			// �G���[���N�����璼���ɏI��
//	}
//
//
//	// �ꖇ�ڂ̊G
//	Kao(0);		// ���`��B���i���Ă�j
//	WaitKey();	// ���������܂Œ�~
//
//	// �񖇖ڂ̊G
//	ClearDrawScreen();	// ��ʃN���A
//	Kao(30);	// ���`��B���i�J����j
//	WaitKey();	// ���������܂Œ�~
//
//	// �񖇖ڂ̊G
//	ClearDrawScreen();	// ��ʃN���A
//	Kao(0);		// ���`��B���i���Ă�j
//	WaitKey();	// ���������܂Œ�~
//
//	DxLib_End();				// �c�w���C�u�����g�p�̏I������
//
//	return 0;				// �\�t�g�̏I�� 
//}
