#include "sub.h"

// 画像読み込み関数
bool ReadBmp(HBITMAP* hBmp, LPCTSTR fileName)
{
	*hBmp = (HBITMAP)LoadImage(
		NULL,					// インスタンス番号(省略)
		fileName,	// 画像ファイル名
		IMAGE_BITMAP,			// ビットマップ画像を設定
		0, 0,					// 画像内の読み込み開始座標(通常は0,0)
		LR_LOADFROMFILE);		// ファイルから読込む指定
	if (*hBmp == NULL)
	{
		MessageBox(NULL, "画像ファイルの読込み失敗", fileName, MB_OK);
		return false;		// 読み込み失敗
	}
	return true;			// 読込み成功
}

// 画像表示関数
void BBlt(HDC hdc, HDC hMemDC, HBITMAP hBmp)
{
	// 表示する画像をメモリDCに展開
	SelectObject(hMemDC, hBmp);

	// 画像の表示
	BitBlt(hdc,			// 画面
		0, 0,			// 表示先の座標x,y
		WND_W, WND_H,	// 画像の幅、高さ
		hMemDC,			// メモリDC
		0, 0,			// 画像上の座標x,y
		SRCCOPY);		// 「メモリDCから画面にコピー」という指定

}

// 画像の透過処理関数
void TransBlt(HDC hdc, HDC hMemDC, HBITMAP hBmp, int x, int y, int w, int h)
{
	// 表示する画像をメモリDCに展開
	SelectObject(hMemDC, hBmp);

	TransparentBlt(hdc,	// 画面
		x, y,			// 表示先座標
		w, h,			// 表示する画像の幅、高さ
		hMemDC,			// メモリDC
		0, 0,			// 画像上の座標
		w, h,			// 画像の幅、高さ
		TRANS_COLOR);	// 透過色
}

// フォントの組み込み関数
void AddFont(LPCTSTR fontPath)
{
	if (AddFontResourceEx(
		fontPath,		// フォントパス
		FR_PRIVATE,		// 終了時に削除
		NULL) == NULL)
	{
		MessageBox(NULL, "文字フォントの読み込みに失敗", fontPath, MB_OK);
	}
}

// フォント作成関数
HFONT MakeFont(int w, int h, LPCTSTR fontName)
{
	// フォントの作成
	HFONT hFont = CreateFont(
		h,							// 文字の大きさ
		w,							// 文字幅
		0,							// 角度
		0,							// ベースライン角度 (メモ:斜め上になるとか)
		FW_REGULAR,					// 太さ(太字:FW_BOLD)
		FALSE,						// 斜体
		FALSE,						// 下線 (メモ:アンダーライン)
		FALSE,						// 打消し線
		SHIFTJIS_CHARSET,			// 文字セット 
		OUT_DEFAULT_PRECIS,			// 出力精度
		CLIP_DEFAULT_PRECIS,		// クリッピング精度
		DEFAULT_QUALITY,			// 出力品質
		VARIABLE_PITCH | FF_ROMAN,	// 可変ピッチとフォントファミリ
		fontName);					// 書体(NULL:現在使用中の書体)

	if (hFont == NULL)
	{
		MessageBox(NULL, "フォントの作成に失敗", fontName, MB_OK);
	}
	return hFont;
}

// テキスト表示関数
void Text(HDC hdc, int x, int y, const char* text)
{
	TextOut(hdc,					// 画面
		x, y,						// 表示先の座標xy
		text,						// 表示する文字列
		lstrlen(text));				// 表示する長さ
	SetBkMode(hdc, TRANSPARENT);	//透明にする
}

// テキストウィンドウ風の枠を描画する関数
void DrawTextFrame(HDC hdc, int left, int top, int right, int bottom)
{
	RECT textRect = { left, top, right, bottom };

	// 背景塗り（薄いグレー）
	HBRUSH hBrush = CreateSolidBrush(RGB(190, 190, 200));
	FillRect(hdc, &textRect, hBrush);
	DeleteObject(hBrush);

	// 枠線を描く（赤）
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

// テキストウィンドウ風の枠を描画する関数
void DrawTextFrame_2(HDC hdc, int left, int top, int right, int bottom)
{
	RECT textRect = { left, top, right, bottom };

	// 背景塗り（薄いグレー）
	HBRUSH hBrush = CreateSolidBrush(RGB(190, 190, 200));
	FillRect(hdc, &textRect, hBrush);
	DeleteObject(hBrush);

	// 枠線を描く（黒）
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


// オープン
void BGM_Opne(HWND hWnd, const char* fileName)
{
	char str[128];

	strcpy_s(str, "open sound/");
	strcat_s(str, fileName);
	strcat_s(str, " alias BGM");

	mciSendString(str, NULL, 0, hWnd);
}
// 再生
void BGM_Play(HWND hWnd)
{
	mciSendString("play BGM notify", NULL, 0, hWnd);
}
// クローズ
void BGM_Close(HWND hWnd)
{
	mciSendString("close BGM", NULL, 0, hWnd);
}
// ループ
void BGM_Loop(HWND hWnd)
{

	char statusMsg[256];	// ステータスメッセージの取得用

	// BGMの状態を取得する
	mciSendString("status BGM mode", statusMsg, 255, hWnd);

	// BGMが停止しているか？
	if (strcmp(statusMsg, "stopped") == 0)
	{
		// BGMの先頭に戻す
		mciSendString("seek BGM to start", NULL, 0, hWnd);
		// BGMの再生
		BGM_Play(hWnd);
	}

}

// 効果音(wav)
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
		// ファイル名が無しなのでループの停止
		PlaySound(NULL, NULL, 0);
		return;
	}

	if (isLoop == true)
	{
		// ループあり
		PlaySound(str, NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}
	else
	{
		// ループなし
		PlaySound(str, NULL, SND_ASYNC | SND_FILENAME);
	}
}

// ---フルスクリーンの切り替え---

// フルスクリーンON
bool FullScreenOn(HWND hWnd)
{
	// ウィンドウ種別を枠無しに変更
	SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);

	// デバイスモードを変更
	DEVMODE devMode;
	devMode.dmSize = sizeof(DEVMODE);					// 構造体のサイズ
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;	// 幅、高さの設定
	devMode.dmPelsWidth = WND_W;						// ウインドウの幅
	devMode.dmPelsHeight = WND_H;						// ウインドウの高さ

	// 解像度の変更
	LONG lResult = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	if (lResult != DISP_CHANGE_SUCCESSFUL)
	{
		ERR_MSG("ディスプレイの設定を変更できません", "ERROR", MB_OK);
		return false;
	}

	// 画面更新
	ShowWindow(hWnd, SW_SHOW);

	SetWindowPos(hWnd,
		HWND_TOPMOST,		// 最前面のウインドウにする
		0, 0,				// 始点
		WND_W, WND_H,		// 終点
		SWP_NOSIZE);		// オプション:ウインドウの現在のサイズを維持

	return true;
}

// フルスクリーンOFF
void FullScreenOff(HWND hWnd)
{
	// ウインドウ種別を戻す
	SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

	// 解像度を戻す
	LONG lResult = ChangeDisplaySettings(NULL, 0);

	// 画面更新
	ShowWindow(hWnd, SW_SHOW);

	SetWindowPos(hWnd,
		HWND_TOPMOST,		// 最前面のウインドウにする
		100, 50,			// 始点
		WND_W, WND_H,		// 終点
		SWP_SHOWWINDOW);	// オプション:ウインドウの現在のサイズを維持

}

// 遅延 (あまり意味はないけど)
void sleep(int speed)
{
	clock_t c1 = clock(), c2;
	do {
		if ((c2 = clock()) == (clock_t)-1)
			break;
	} while (1000 * (c2 - c1) / CLOCKS_PER_SEC < speed);
}