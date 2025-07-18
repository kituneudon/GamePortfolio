#include "sub.h"

// �摜�ǂݍ��݊֐�
bool ReadBmp(HBITMAP* hBmp, LPCTSTR fileName)
{
	*hBmp = (HBITMAP)LoadImage(
		NULL,					// �C���X�^���X�ԍ�(�ȗ�)
		fileName,	// �摜�t�@�C����
		IMAGE_BITMAP,			// �r�b�g�}�b�v�摜��ݒ�
		0, 0,					// �摜���̓ǂݍ��݊J�n���W(�ʏ��0,0)
		LR_LOADFROMFILE);		// �t�@�C������Ǎ��ގw��
	if (*hBmp == NULL)
	{
		MessageBox(NULL, "�摜�t�@�C���̓Ǎ��ݎ��s", fileName, MB_OK);
		return false;		// �ǂݍ��ݎ��s
	}
	return true;			// �Ǎ��ݐ���
}

// �摜�\���֐�
void BBlt(HDC hdc, HDC hMemDC, HBITMAP hBmp)
{
	// �\������摜��������DC�ɓW�J
	SelectObject(hMemDC, hBmp);

	// �摜�̕\��
	BitBlt(hdc,			// ���
		0, 0,			// �\����̍��Wx,y
		WND_W, WND_H,	// �摜�̕��A����
		hMemDC,			// ������DC
		0, 0,			// �摜��̍��Wx,y
		SRCCOPY);		// �u������DC�����ʂɃR�s�[�v�Ƃ����w��

}

// �摜�̓��ߏ����֐�
void TransBlt(HDC hdc, HDC hMemDC, HBITMAP hBmp, int x, int y, int w, int h)
{
	// �\������摜��������DC�ɓW�J
	SelectObject(hMemDC, hBmp);

	TransparentBlt(hdc,	// ���
		x, y,			// �\������W
		w, h,			// �\������摜�̕��A����
		hMemDC,			// ������DC
		0, 0,			// �摜��̍��W
		w, h,			// �摜�̕��A����
		TRANS_COLOR);	// ���ߐF
}

// �t�H���g�̑g�ݍ��݊֐�
void AddFont(LPCTSTR fontPath)
{
	if (AddFontResourceEx(
		fontPath,		// �t�H���g�p�X
		FR_PRIVATE,		// �I�����ɍ폜
		NULL) == NULL)
	{
		MessageBox(NULL, "�����t�H���g�̓ǂݍ��݂Ɏ��s", fontPath, MB_OK);
	}
}

// �t�H���g�쐬�֐�
HFONT MakeFont(int w, int h, LPCTSTR fontName)
{
	// �t�H���g�̍쐬
	HFONT hFont = CreateFont(
		h,							// �����̑傫��
		w,							// ������
		0,							// �p�x
		0,							// �x�[�X���C���p�x (����:�΂ߏ�ɂȂ�Ƃ�)
		FW_REGULAR,					// ����(����:FW_BOLD)
		FALSE,						// �Α�
		FALSE,						// ���� (����:�A���_�[���C��)
		FALSE,						// �ŏ�����
		SHIFTJIS_CHARSET,			// �����Z�b�g 
		OUT_DEFAULT_PRECIS,			// �o�͐��x
		CLIP_DEFAULT_PRECIS,		// �N���b�s���O���x
		DEFAULT_QUALITY,			// �o�͕i��
		VARIABLE_PITCH | FF_ROMAN,	// �σs�b�`�ƃt�H���g�t�@�~��
		fontName);					// ����(NULL:���ݎg�p���̏���)

	if (hFont == NULL)
	{
		MessageBox(NULL, "�t�H���g�̍쐬�Ɏ��s", fontName, MB_OK);
	}
	return hFont;
}

// �e�L�X�g�\���֐�
void Text(HDC hdc, int x, int y, const char* text)
{
	TextOut(hdc,					// ���
		x, y,						// �\����̍��Wxy
		text,						// �\�����镶����
		lstrlen(text));				// �\�����钷��
	SetBkMode(hdc, TRANSPARENT);	//�����ɂ���
}

// �e�L�X�g�E�B���h�E���̘g��`�悷��֐�
void DrawTextFrame(HDC hdc, int left, int top, int right, int bottom)
{
	RECT textRect = { left, top, right, bottom };

	// �w�i�h��i�����O���[�j
	HBRUSH hBrush = CreateSolidBrush(RGB(190, 190, 200));
	FillRect(hdc, &textRect, hBrush);
	DeleteObject(hBrush);

	// �g����`���i�ԁj
	HPEN hPen = CreatePen(PS_SOLID, 7, RGB(100, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	MoveToEx(hdc, left, top, NULL);
	LineTo(hdc, right, top);
	LineTo(hdc, right, bottom);
	LineTo(hdc, left, bottom);
	LineTo(hdc, left, top);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

// �e�L�X�g�E�B���h�E���̘g��`�悷��֐�
void DrawTextFrame_2(HDC hdc, int left, int top, int right, int bottom)
{
	RECT textRect = { left, top, right, bottom };

	// �w�i�h��i�����O���[�j
	HBRUSH hBrush = CreateSolidBrush(RGB(190, 190, 200));
	FillRect(hdc, &textRect, hBrush);
	DeleteObject(hBrush);

	// �g����`���i���j
	HPEN hPen = CreatePen(PS_SOLID, 7, RGB(0, 0, 50));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	MoveToEx(hdc, left, top, NULL);
	LineTo(hdc, right, top);
	LineTo(hdc, right, bottom);
	LineTo(hdc, left, bottom);
	LineTo(hdc, left, top);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}


// �I�[�v��
void BGM_Opne(HWND hWnd, const char* fileName)
{
	char str[128];

	strcpy_s(str, "open sound/");
	strcat_s(str, fileName);
	strcat_s(str, " alias BGM");

	mciSendString(str, NULL, 0, hWnd);
}
// �Đ�
void BGM_Play(HWND hWnd)
{
	mciSendString("play BGM notify", NULL, 0, hWnd);
}
// �N���[�Y
void BGM_Close(HWND hWnd)
{
	mciSendString("close BGM", NULL, 0, hWnd);
}
// ���[�v
void BGM_Loop(HWND hWnd)
{

	char statusMsg[256];	// �X�e�[�^�X���b�Z�[�W�̎擾�p

	// BGM�̏�Ԃ��擾����
	mciSendString("status BGM mode", statusMsg, 255, hWnd);

	// BGM����~���Ă��邩�H
	if (strcmp(statusMsg, "stopped") == 0)
	{
		// BGM�̐擪�ɖ߂�
		mciSendString("seek BGM to start", NULL, 0, hWnd);
		// BGM�̍Đ�
		BGM_Play(hWnd);
	}

}

// ���ʉ�(wav)
void SE_Play(const char* fileName, bool isLoop)
{
	char str[128];

	if (strlen(fileName) > 0)
	{
		strcpy_s(str, "sound/");
		strcat_s(str, fileName);
	}
	else
	{
		// �t�@�C�����������Ȃ̂Ń��[�v�̒�~
		PlaySound(NULL, NULL, 0);
		return;
	}

	if (isLoop == true)
	{
		// ���[�v����
		PlaySound(str, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	else
	{
		// ���[�v�Ȃ�
		PlaySound(str, NULL, SND_ASYNC | SND_FILENAME);
	}
}

// ---�t���X�N���[���̐؂�ւ�---

// �t���X�N���[��ON
bool FullScreenOn(HWND hWnd)
{
	// �E�B���h�E��ʂ�g�����ɕύX
	SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);

	// �f�o�C�X���[�h��ύX
	DEVMODE devMode;
	devMode.dmSize = sizeof(DEVMODE);					// �\���̂̃T�C�Y
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;	// ���A�����̐ݒ�
	devMode.dmPelsWidth = WND_W;						// �E�C���h�E�̕�
	devMode.dmPelsHeight = WND_H;						// �E�C���h�E�̍���

	// �𑜓x�̕ύX
	LONG lResult = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	if (lResult != DISP_CHANGE_SUCCESSFUL)
	{
		ERR_MSG("�f�B�X�v���C�̐ݒ��ύX�ł��܂���", "ERROR", MB_OK);
		return false;
	}

	// ��ʍX�V
	ShowWindow(hWnd, SW_SHOW);

	SetWindowPos(hWnd,
		HWND_TOPMOST,		// �őO�ʂ̃E�C���h�E�ɂ���
		0, 0,				// �n�_
		WND_W, WND_H,		// �I�_
		SWP_NOSIZE);		// �I�v�V����:�E�C���h�E�̌��݂̃T�C�Y���ێ�

	return true;
}

// �t���X�N���[��OFF
void FullScreenOff(HWND hWnd)
{
	// �E�C���h�E��ʂ�߂�
	SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

	// �𑜓x��߂�
	LONG lResult = ChangeDisplaySettings(NULL, 0);

	// ��ʍX�V
	ShowWindow(hWnd, SW_SHOW);

	SetWindowPos(hWnd,
		HWND_TOPMOST,		// �őO�ʂ̃E�C���h�E�ɂ���
		100, 50,			// �n�_
		WND_W, WND_H,		// �I�_
		SWP_SHOWWINDOW);	// �I�v�V����:�E�C���h�E�̌��݂̃T�C�Y���ێ�

}

// �x�� (���܂�Ӗ��͂Ȃ�����)
void sleep(int speed)
{
	clock_t c1 = clock(), c2;
	do {
		if ((c2 = clock()) == (clock_t)-1)
			break;
	} while (1000 * (c2 - c1) / CLOCKS_PER_SEC < speed);
}