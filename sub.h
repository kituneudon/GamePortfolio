#pragma once

#include <Windows.h>	// Windows�p�̖��߂�ǉ�
#include <shellapi.h>  // ShellExecute�̂��߂ɒǉ�
#include <stdio.h>
#include <iostream>

// ���C�u�����錾
#pragma comment(lib,"winmm.lib")	// ���y�֌W���C�u����
#pragma comment(lib,"msimg32.lib")	// �摜�\���֌W���C�u����

// �}�N����`
#define APPLI_NAME "Choice_is_Correct"				// �A�v���P�[�V������
#define TITLE_NAME "���̑I���͐�����?"				// �Q�[���^�C�g��
#define WND_W	1280								// �E�C���h�E�̕�(width)
#define WND_H	720									// �E�C���h�E�̍���(height)

// �}�N���֐�
#define ERR_MSG(s,f) MessageBox(NULL, s, f, MB_OK)	// �G���[���b�Z�[�W
#define TRANS_COLOR RGB(0,255,0)					// ���ߐF

//=== �֐��̃v���g�^�C�v�錾 ===
// �摜�ǂݍ��݊֐�
bool ReadBmp(HBITMAP* hBmp, LPCTSTR fileName);
// �摜�\���֐�
void BBlt(HDC hdc, HDC hMemDC, HBITMAP hBmp);
// ���ߕ\���֐�
void TransBlt(HDC hdc, HDC hMemDC, HBITMAP hBmp, int x, int y, int w, int h);

// �t�H���g�̑g�ݍ��݊֐�
void AddFont(LPCTSTR fontPath);
// �t�H���g�쐬�֐�
HFONT MakeFont(int w, int h, LPCTSTR fontName);
// �e�L�X�g�\���֐�
void Text(HDC hdc, int x, int y, const char* text);

// �e�L�X�g�g�`��֐�
void DrawTextFrame(HDC hdc, int left, int top, int right, int bottom);
void DrawTextFrame_2(HDC hdc, int left, int top, int right, int bottom);

// Sound�֐�
void BGM_Opne(HWND hWnd, const char* fileName);		// �I�[�v��
void BGM_Play(HWND hWnd);							// �Đ�
void BGM_Close(HWND hWnd);							// �N���[�Y
void BGM_Loop(HWND hWnd);							// ���[�v
void SE_Play(const char* fileName, bool isLoop);	// ���ʉ�(wav)

// �t���X�N���[���̐؂�ւ�
bool FullScreenOn(HWND hWnd);						// �t���X�N���[��ON
void FullScreenOff(HWND hWnd);						// �t���X�N���[��OFF

// �x�� (���܂�Ӗ��͂Ȃ�����)
void sleep(int speed);