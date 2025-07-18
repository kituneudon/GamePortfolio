#pragma once

#include <Windows.h>	// Windows用の命令を追加
#include <shellapi.h>  // ShellExecuteのために追加
#include <stdio.h>
#include <iostream>

// ライブラリ宣言
#pragma comment(lib,"winmm.lib")	// 音楽関係ライブラリ
#pragma comment(lib,"msimg32.lib")	// 画像表示関係ライブラリ

// マクロ定義
#define APPLI_NAME "Choice_is_Correct"				// アプリケーション名
#define TITLE_NAME "その選択は正しい?"				// ゲームタイトル
#define WND_W	1280								// ウインドウの幅(width)
#define WND_H	720									// ウインドウの高さ(height)

// マクロ関数
#define ERR_MSG(s,f) MessageBox(NULL, s, f, MB_OK)	// エラーメッセージ
#define TRANS_COLOR RGB(0,255,0)					// 透過色

//=== 関数のプロトタイプ宣言 ===
// 画像読み込み関数
bool ReadBmp(HBITMAP* hBmp, LPCTSTR fileName);
// 画像表示関数
void BBlt(HDC hdc, HDC hMemDC, HBITMAP hBmp);
// 透過表示関数
void TransBlt(HDC hdc, HDC hMemDC, HBITMAP hBmp, int x, int y, int w, int h);

// フォントの組み込み関数
void AddFont(LPCTSTR fontPath);
// フォント作成関数
HFONT MakeFont(int w, int h, LPCTSTR fontName);
// テキスト表示関数
void Text(HDC hdc, int x, int y, const char* text);

// テキスト枠描画関数
void DrawTextFrame(HDC hdc, int left, int top, int right, int bottom);
void DrawTextFrame_2(HDC hdc, int left, int top, int right, int bottom);

// Sound関数
void BGM_Opne(HWND hWnd, const char* fileName);		// オープン
void BGM_Play(HWND hWnd);							// 再生
void BGM_Close(HWND hWnd);							// クローズ
void BGM_Loop(HWND hWnd);							// ループ
void SE_Play(const char* fileName, bool isLoop);	// 効果音(wav)

// フルスクリーンの切り替え
bool FullScreenOn(HWND hWnd);						// フルスクリーンON
void FullScreenOff(HWND hWnd);						// フルスクリーンOFF

// 遅延 (あまり意味はないけど)
void sleep(int speed);