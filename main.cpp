#include "sub.h"		// 自身で作成したものは"ダブルクォーテーション"で記載する

// ウインドウ関数(ゲームのプログラムを置く所)
LRESULT CALLBACK WinidowProc(
	HWND hWnd,		// ウインドウハンドル
	UINT uMsg,		// メッセージ(キーが押された等)
	WPARAM wParam,	// メッセージに対応した情報
	LPARAM lParam)	// メッセージに対応した情報
{

	// 変数宣言
	HDC				hdc;			// デバイスコンテキスト(以下DC)。画面の事。
	PAINTSTRUCT		ps;				// 描画構造体
	static HDC		hMemDC;			// メモリデバイスコンテキスト(メモリDC)


	// ヘルプ画面
	static HBITMAP	hHelp;			// ヘルプ画面を表示

	// フルスクリーン用
	static bool		fullScreen = false;		// フルスクリーン


	// ---クイズ用---
	static HBITMAP	hTitle;			// クイズ用タイトル画像
	static HBITMAP	hQuiz1;			// クイズ1問目画像
	static HBITMAP  hOK;			// 正解画像
	static HBITMAP  hNG;			// 不正解画像 1と2併用
	static HBITMAP	hQuiz2;			// クイズ2問目画像
	static HBITMAP  hOK2;			// 正解画像
	static HBITMAP	hQuiz1Kotae;	// クイズ1と2問目答え画像
	static HBITMAP	hSteam;			// Steam 画像表示


	// ---RPG用---
	static HBITMAP	hRPGTitle;		// RPG用タイトル画像
	static HBITMAP	hSetting;		// 設定画面


	static HBITMAP	hStage;			// 冒頭
	static HBITMAP	hStage_0;		// ステージ0 玉座
	static HBITMAP	hStage_1;		// ステージ1 スライム戦
	static HBITMAP	hStage_2;		// ステージ2 ゴブリン戦
	static HBITMAP	hStage_3;		// ステージ3 ラスボス戦

	static HBITMAP	hStage_10;		// 隠しステージ(封印剣イベント用)


	static HBITMAP	hHappyFin;		// ハッピーエンド 隠しイベントクリアで🎶
	static HBITMAP	hTrueFin;		// トルゥーエンド
	static HBITMAP	hBadFin;		// バットエンド(殺戮エンド)

	static HBITMAP	hGameOver;		// ゲームオーバー
	static HBITMAP	hGameOver_2;	// 異例ゲームオーバー


	static HBITMAP	hKing;			// 王
	static HBITMAP	hBrave;			// 勇者
	static HBITMAP	hBrave_b;		// 勇者 拡大版
	static HBITMAP	hBrave_c;		// 勇者 超拡大版
	static HBITMAP	hWizard;		// 魔法使い
	static HBITMAP	hWizard_b;		// 魔法使い 拡大版
	static HBITMAP	hWizard_c;		// 魔法使い 超拡大版

	static HBITMAP	hSlime;			// スライム
	static HBITMAP	hGoblin;		// ゴブリン
	static HBITMAP	hBoss;			// ボス
	static HBITMAP	hBoss_b;		// ボス 拡大版
	static HBITMAP	hBoss_c;		// ボス オーラなし

	static HBITMAP	hSorad;			// 封印剣

	// ---RPGシステム変数---
	static bool		has_princess = false;		// 姫同行フラグ
	static bool		has_sealed_sword = false;	// 封印剣所持フラグ
	static int		sealed_sword = 0;			// 封印剣獲得出来るか否か
	static int		enemies_killed = 0;			// 倒した敵の数
	
	static int		escapeTry = 0;				// ボス戦用 --- 逃げる判定（3回失敗で強制死亡） ---
	static int		attack_count = 0;			// ボス戦用	--- 攻撃の上限 (5回成功で体力に関係なく強制ゲームクリア) ---

	static int		braveHP = 999;				// 勇者の体力
	static int		princessHP = 600;			// 魔法使い(姫)の体力
	static int		bossHP = 1200;				// 魔王の体力


	// ---フォント設定---
	static HFONT	hFont;			// フォントハンドル
	static HFONT	hFont_2;		// フォントハンドル2
	static HFONT	hFont_3;		// フォントハンドル3
	static HFONT	hFont_4;		// フォントハンドル4
	static HFONT	hFont_5;		// フォントハンドル5
	static HFONT	hFont_6;		// フォントハンドル6
	static LPCTSTR	fontPath;		// フォントパス


	// ---BGM---
	static int bgmVolume = 50; // 音量（0〜100）


	// ---頁と選択---
	static int		page = 1000;			// 頁カウンター (ページ)
	static int		sel = 0;				// 選択番号
	static int		command_sel = 0;		// コマンド用選択番号
	static int		RPGCommand_sel = 0;		// RPG用コマンド
	static int		previous_page = 1000;	// 音量調整前のページを保存
	static int		help_page = 1000;		// ヘルプ画面のページを保存


	// メッセージ(ウインドウに何をしたか?)別の処理
	switch (uMsg) 
	{
		case WM_CREATE:			// ウインドウが作成される時
			//=== 初期処理を書く所 ===
			// フォントの作成
			hFont = MakeFont(25, 50,"ＭＳ ゴシック");			// 書体(NULL:現在使用中の書体)

			hFont_2 = MakeFont(50, 100, "ＭＳ ゴシック");

			hFont_3 = MakeFont(30, 60, "ＭＳ ゴシック");

			hFont_4 = MakeFont(15, 30, "ＭＳ ゴシック");

			fontPath = "font/azuki.ttf";						// フリーフォント名
			AddFont(fontPath);									// フリーフォントの組み込み
			hFont_5 = MakeFont(15, 30, "あずきフォント");

			hFont_6 = MakeFont(50, 100, "あずきフォント");


			// ---ヘルプ画面---
			if (ReadBmp(&hHelp, "Image/ヘルプ画面.bmp") == false)return 0;
			
			// ---クイズ用---
			// クイズ用タイトル画像
			if (ReadBmp(&hTitle, "Image/背景.bmp") == false)return 0;
			// 一問目
			if (ReadBmp(&hQuiz1, "Image/VRC恋愛について.bmp") == false)return 0;
			// 正解
			if (ReadBmp(&hOK, "Image/お砂糖_正解.bmp") == false)return 0;
			// 不正解
			if (ReadBmp(&hNG, "Image/不正解.bmp") == false)return 0;
			// 二問目
			if (ReadBmp(&hQuiz2, "Image/お砂糖_別れることを.bmp") == false)return 0;
			// 二問目 正解
			if (ReadBmp(&hOK2, "Image/お塩_正解.bmp") == false)return 0;
			// 一二問目 の 解説
			if (ReadBmp(&hQuiz1Kotae, "Image/お砂糖_お塩文化.bmp") == false)return 0;
			// steam 表示
			if (ReadBmp(&hSteam, "Image/VRhat.bmp") == false)return 0;

			// ---RPG用---
			// タイトル
			if (ReadBmp(&hRPGTitle, "Image/RPGタイトル.bmp") == false)return 0;
			// 冒頭
			if (ReadBmp(&hStage, "Image/冒頭.bmp") == false)return 0;
			// 玉座の間
			if(ReadBmp(&hStage_0, "Image/玉座の間.bmp") == false)return 0;
			// 草原
			if (ReadBmp(&hStage_1, "Image/草原.bmp") == false)return 0;
			// 廃墟
			if (ReadBmp(&hStage_2, "Image/廃墟.bmp") == false)return 0;
			// ラスボスの間
			if (ReadBmp(&hStage_3, "Image/ラスボスの間.bmp") == false)return 0;
			// 隠しステージ(封印剣イベント用)
			if (ReadBmp(&hStage_10, "Image/封印剣イベント.bmp") == false)return 0;

			// キング
			if (ReadBmp(&hKing, "Image/王.bmp") == false)return 0;
			// 勇者
			if (ReadBmp(&hBrave, "Image/勇者.bmp") == false)return 0;
			// 勇者(拡大版)
			if (ReadBmp(&hBrave_b, "Image/勇者_b.bmp") == false)return 0;
			// 勇者(超拡大版)
			if (ReadBmp(&hBrave_c, "Image/勇者_c.bmp") == false)return 0;
			// 魔法使い
			if (ReadBmp(&hWizard, "Image/魔法使い.bmp") == false)return 0;
			// 魔法使い(拡大版)
			if (ReadBmp(&hWizard_b, "Image/魔法使い_B.bmp") == false)return 0;
			// 魔法使い(超拡大版)
			if (ReadBmp(&hWizard_c, "Image/魔法使い_C.bmp") == false)return 0;
			// スライム
			if (ReadBmp(&hSlime, "Image/スライム.bmp") == false)return 0;
			// ゴブリン
			if (ReadBmp(&hGoblin, "Image/ゴブリン.bmp") == false)return 0;
			// ラスボス
			if (ReadBmp(&hBoss, "Image/魔王.bmp") == false)return 0;
			// ラスボス(拡大版)
			if (ReadBmp(&hBoss_b, "Image/魔王_B.bmp") == false)return 0;
			// ラスボス(超拡大版)
			if (ReadBmp(&hBoss_c, "Image/魔王_C.bmp") == false)return 0;

			// 封印剣
			if (ReadBmp(&hSorad, "Image/封印剣.bmp") == false)return 0;
			
			
			// バッドエンド
			if (ReadBmp(&hBadFin, "Image/Bad_Fin.bmp") == false)return 0;
			// トルゥーエンド
			if (ReadBmp(&hTrueFin, "Image/True_Fin.bmp") == false)return 0;
			// ハッピーエンド
			if (ReadBmp(&hHappyFin, "Image/Happy_Fin.bmp") == false)return 0;
			// ゲームオーバー
			if (ReadBmp(&hGameOver, "Image/ゲームオーバー.bmp") == false)return 0;
			// 異例ゲームオーバー
			if (ReadBmp(&hGameOver_2, "Image/ゲームオーバー_2.bmp") == false)return 0;


			// メモリDCの作成
			hMemDC = CreateCompatibleDC(NULL);

			// BGMのオープン
			BGM_Opne(hWnd, "omochanokuninomarch.mp3");

			// 初期音量設定
			char command[128];
			sprintf_s(command, 128, "setaudio BGM volume to %d", bgmVolume * 5);
			mciSendString(command, NULL, 0, NULL);

			// BGMの再生
			BGM_Play(hWnd);

			// タイマーの設定
			SetTimer(hWnd,
				1,		// タイマーNo
				100,	// 時間(ミリ秒,100=0.1秒)
				NULL);	// 一定時間に動かす関数

			return 0;				// WinMainに戻る

		case WM_TIMER:				// 時間(タイマー)で動く

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

			return 0;

		case WM_KEYDOWN:			// 何かキーが押された時
			// キー別の処理
			switch (wParam)
			{
			case VK_ESCAPE:				// ESCキーが押された時
				PostQuitMessage(0);		// アプリケーションの終了をwindowsに伝える
				break;

			// ---デバック用---
			case 'G':
				page = 1;
				break;

			case '0':
				page = 1000;
				break;

			case 'K':
				//page = 2036;
				//page = 2040;
				//page = 2199;
				//page = 2208;
				//page = 2240;
				//page = 2399;
				//page = 2599;
				//page = 2600;
				//page = 2899;
				//page = 2900;
				//page = 5000;

				//enemies_killed = 0;
				//enemies_killed = 1;
				//enemies_killed = 2;
				
				//sealed_sword = 0;
				//sealed_sword = 1;
				//sealed_sword = 2;
				
				//has_princess = false;
				//has_princess = true;

				//has_sealed_sword = false;
				//has_sealed_sword = true;

				break;
			// ----------------

			case 'L':					// Lキー

				if (fullScreen == false)
				{
					fullScreen = FullScreenOn(hWnd);
				}
				else
				{
					FullScreenOff(hWnd);
					fullScreen = false;
				}

				break;


			case 'B':
				// 音量調整画面にいる場合は元のページに戻る
				if (page == 11) {
					page = previous_page;  // 保存していたページに戻る
					sel = 0;  // 選択をリセット
				}
				else {
					// 音量調整画面以外からBが押された場合
					previous_page = page;  // 現在のページを保存
					page = 11;  // 音量調整画面へ
					sel = 0;  // 選択をリセット
				}
				break;


			case 'Q':
				// ヘルプ画面にいる場合は元のページに戻る
				if (page == 10000) {
					page = help_page;  // 保存していたページに戻る
				}
				else {
					// ヘルプ画面以外からQが押された場合
					help_page = page;  // 現在のページを保存
					page = 10000;  // ヘルプ画面へ
				}
				break;


			case 'P':
				if (page == 1006)
				{
				ShellExecute(NULL, "open", "https://store.steampowered.com/app/438100/VRChat/?l=japanese", NULL, NULL, SW_SHOWNORMAL);
				break;
				}

			case VK_UP:					// 上矢印キー
			case 'W':
				if (sel > 0)sel--;					// 選択を上に進める
				break;

			case VK_DOWN:				// 下矢印キー
			case 'S':
				if (page == 1 || page == 10 || page == 2044 || page == 2208 || page == 2233 || page == 2238 ||page == 2612||page == 2850)
				{
					if (sel < 1)
					{
						sel++;
					}
				}
				else if (page == 2411 || page == 2902)
				{
					if (sel < 2)
					{
						sel++;
					}
				}
				else if (sel < 3)
				{
					sel++;					// 選択を下に進める
				}
				break;

			case VK_LEFT:				// 左矢印キー
			case 'A':
				if (command_sel > 0)command_sel--;
				break;

			case VK_RIGHT:				// 右矢印キー
			case 'D':
				if (command_sel < 3)command_sel++;
				break;

			case VK_RETURN:				// Enterキー

				if (page == 1001)			// 問題1
				{
					if (sel != 1)		//正解ではない時
					{
						page = 1099;	//不正解にする
						sel = 0;
					}
				}
				else if (page == 1003)
				{
					if (sel != 0)		//正解ではない時
					{
						page = 1099;	//不正解にする;
						sel = 0;
					}
				}

				// RPGタイトル画面
				else if (page == 1) {
					if (sel == 0) { // ゲームスタート
						page = 1999;
						has_princess = false;
						has_sealed_sword = false;
						sealed_sword = 0;
						enemies_killed = 0;
						escapeTry = 0;
						attack_count = 0;
						braveHP = 999;
						princessHP = 600;
						bossHP = 1200;
					}
					else if (sel == 1) { // 設定
						page = 9;
					}
				}

				// 設定画面
				else if (page == 10) {
					if (sel == 0) { // BGM音量調整
						page = 10;
					}
					else if (sel == 1) { // 戻る
						page = 0;
					}
				}

				// 音量調整画面でEnterが押された場合
				if (page == 11) {
					page = 1;  // RPGタイトル画面に戻る
					sel = 0;  // 選択をリセット
					InvalidateRect(hWnd, NULL, TRUE);
					break;  // ここでbreakして、page++を実行させない
				}

				// 姫同行
				if (page == 2044)
				{
					if (sel == 0) { // 同行する
						has_princess = true;
					}
					else if (sel == 1) { // 同行しない
						has_princess = false;
					}
				}

				if (page == 2047 && has_princess == true)
				{
					page = 2048;
				}

				if (page == 2050)			// ■序章→■ステージ1 に遷移
				{
					page = 2199;
				}


// ----------スライム戦----------
	
				// スライム戦 戦う/戦わない 処理
				if (page == 2208)
				{
					if (sel == 0) { // 戦う
						page = 2229;
					}
					else if (sel == 1) { // 戦わない
						page = 2208;
					}
				}
				if (page == 2233)
				{
					if (sel == 0) { // 戦う
						page = 2233;
					}
					else if (sel == 1) { // 戦わない
						page = 2208;
					}
				}
				if (page == 2238)
				{
					if (sel == 0) { // No
						page = 2208;
					}
					else if (sel == 1) { // Yes
						page = 2238;
					}
				}

				if (page == 2241) // 攻撃 何もなし
				{
					if (sel == 1) // 防御
					{
						page = 2249;
					}
					else if (sel == 2) //見逃す
					{
						page = 2299;
					}
					else if (sel == 3) //戦いを辞める
					{
						page = 2349;
					}
				}
				// 戦いを辞めた時 戦わない時と同じ分岐のため、ページを共有
				if (page == 2353)
				{
					page = 2210;
				}

				// 封印剣へのフラグ
				if (page == 2221)
				{
					sealed_sword++;
				}
				
				// スライムを倒したとき
				if (page == 2244)
				{
					enemies_killed +=1;			// 倒した敵の数
				}

				{
				static int i = 1;
					// 防御を取った時
					if (page == 2254 && i < 3)
					{
							page = 2240;
							i++;
					}
					else if (page == 2254 && i >= 3)
					{
						page = 2379;
					}
				}
				// 防御を取り過ぎたら 異例のゲームオーバーに飛ばす!!!
				if (page == 2384)
				{
					page = 8999;
				}

				if (page == 2222 || page == 2246 || page == 2307)			// ■ステージ1→■ステージ2 に遷移
				{
					page = 2399;
				}


// ----------ゴブリン戦----------
				if (page == 2405)
				{
					if (enemies_killed == 0)
					{
						page = 2549;  // ゴブリン無害演出 → 封印剣イベント導線
					}
				}

				if (page == 2414)
				{
					enemies_killed++;			// 倒した敵の数
				}

				if (page == 2411) // 攻撃 何もなし
				{
					if (sel == 1) // 防御
					{
						page = 2449;
					}
					else if (sel == 2) //逃げる
					{
						page = 2499;
					}
				}

				// 撃破後、魔王戦へ
				if (page == 2416)
				{
					page = 2799;
				}

				{
					static int i = 1;
					// 防御を取った時
					if (page == 2454 && i < 3)
					{
						page = 2410;
						i++;
					}
					else if (page == 2454 && i >= 3)
					{
						page = 2579;
					}
				}

				{
					static int i = 1;
					// 逃げるを取った時
					if (page == 2503 && i < 2)
					{
						page = 2410;
						i++;
					}
					else if (page == 2501 && i >= 2)
					{
						page = 2503;
					}
				}

				// 逃きった時
				if (page == 2507)
				{
					page = 2599;
				}


				// 防御を取り過ぎたら 異例のゲームオーバーに飛ばす!!!
				if (page == 2584)
				{
					page = 8999;
				}

				// 封印剣フラグ
				if (page == 2555)
				{
					sealed_sword++;
					page = 2599; // 封印剣イベントへ
				}

				
// ----------封印剣----------

				// 封印剣取得資格有無
				if (page == 2607)
				{
					if (enemies_killed >= 0 && sealed_sword != 2)
					{
						page = 2651;
					}
					else
					{
						page = 2609;
					}
				}

				// 姫が同行しているか否か
				if (page == 2610)
				{
					if (has_princess == true)
					{
						page = 2610;
					}
					else
					{
						page = 2611;
					}
				}

				// 剣を抜くか否か
				if (page == 2612)
				{
					if (sel == 0)	// 抜く
					{ 
						if (sealed_sword >= 2)
						{
							has_sealed_sword = true;
							page = 2699;
						}
						else
						{
							// 剣を抜く資格なし!!!
							page = 2649;
						}
					}
					else if (sel == 1) { // 抜かない
						has_sealed_sword = false;
						page = 2749;
					}
				}

				if (page == 2706 || page == 2754 || page == 2658)			// ■???→■最終章 に遷移
				{
					page = 2799;
				}


// ----------魔王戦----------
 
				// 封印剣所持有無
				if (page == 2808)
				{
					if (has_sealed_sword == true)
					{
						page = 2849;
					}
					else
					{
						page = 2899;
					}
				}
				if (page == 2850)
				{
					if (sel == 1)// 使用しない
					{
						page = 2899;
					}
				}
				
				// 封印剣使用後 ハッピーエンドに移行
				if (page == 2857)
				{
					page = 3999;
				}


				// 選択肢 分岐（キー押下時のみ実行されるはず）
				if (page == 2902)
				{
					// ここは選択時（EnterやSpaceキー押下時）のみ実行される想定
					if (sel == 0)  // 攻撃選択時
					{
						page = 2909;  // 2903へ（page++で）
					}
					else if (sel == 1)  // 防御選択時
					{
						page = 2919;  // 2907へ（page++で）
					}
					else if (sel == 2)  // 逃げる選択時
					{
						page = 2929;  // 2920へ（page++で）
					}
				}


			

				// //攻撃防御判定
				{
					static int i = 1;

					if (page == 2911)
					{
						attack_count++;
						// ボス撃破 アタックカウント
						if (bossHP <= 0 || attack_count >= 5)
						{
							page = 2979;
						}
						if (i < 3)
						{
							page = 2900;
							i++;
						}
					}
					if (i == 3)
					{
						if (page == 2912)
						{
							page = 2901;
							i++;
						}
					}
					if (i >= 4)
					{
						if (page == 2911)
						{
							page = 2912;
						}
						if (page == 2913)
						{
							page = 2901;
							i++;
						}
					}

					// 防御
					if (page == 2921)
					{
						if (i < 3)
						{
							page = 2900;
							i++;
						}
					}
					if (i == 3)
					{
						if (page == 2922)
						{
							page = 2901;
							i++;
						}
					}
					if (i >= 4)
					{
						if (page == 2921)
						{
							page = 2922;
						}
						if (page == 2923)
						{
							page = 2901;
							i++;
						}
					}
						
					// 逃げる場合
					if (page == 2932)
					{
						if (i >= 4)
						{
							page = 2912;
						}
						else if (i == 3)
						{
							page = 2932;
							i++;
						}
						else if (i < 3)
						{
							page = 2900;
							i++;
						}
					}
					if (page == 2933)
					{
						page = 2901;
					}

					if (page == 7000)
					{
						i = 1;
					}
				}

				if (page == 7003)
				{
					page = 2899;
					escapeTry = 0;
					attack_count = 0;
					braveHP = 999;
					princessHP = 600;
					bossHP = 1200;
				}

					// ----------その他 調整等-----------

 
				// ヘルプ画面でエンターを押してしまった場合
				if (page == 10000)
				{
					page = 999;
				}

					page++;  // 頁を進める


				// 最後のページか?
				if (page == 1101 || page == 9008)
				{
					page = 1000;			// クイズタイトルに戻す
				}
				if (page == 1007)
				{
					page = 0;				// RPGゲームタイトルに戻す
				}

				if (page == 3003 || page == 4003 || page == 5003)
				{
					page = 1;				// RPGゲームタイトルに戻す
				}

				// 選択肢リセット
				if (page == 1 || page == 10 || page == 1001 || page == 1003 || page == 2044 || page == 2208 || page == 2233 ||page == 2238 || page == 2241 || page == 2411 || page == 2612 || page == 2850|| page == 2902)
				{
					sel = 0;
				}

				// BGMの変更
				switch (page)
				{
				//タイトル画面に戻った時用
				case 1000:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "omochanokuninomarch.mp3");

					// 音量設定を復元
					char command1[128];
					sprintf_s(command1, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command1, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;
				case 1002:

					SE_Play("maou_se_onepoint15.wav", false);
					break;
				case 1004:

					SE_Play("maou_se_onepoint15.wav", false);
					break;
				case 1100:
					BGM_Close(hWnd);

					SE_Play("maou_se_onepoint32.wav", false);
					break;

				case 1:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "maou_bgm_fantasy14.mp3");

					// 音量設定を復元
					char command2[128];
					sprintf_s(command2, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command2, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;

				// RPG本編スタート
				case 2000:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "RPG_start.mp3");

					// 音量設定を復元
					char command3[128];
					sprintf_s(command3, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command3, NULL, 0, NULL);

					BGM_Play(hWnd);

					break;


				case 2200:
				case 2245:
				case 2255:
				case 2280:
				case 2300:
				case 2350:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "slime.mp3");
					// 音量設定を復元
					char command4[128];
					sprintf_s(command4, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command4, NULL, 0, NULL);
					BGM_Play(hWnd);
					break;

				case 2239:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "slime_battle.mp3");
					// 音量設定を復元
					char command5[128];
					sprintf_s(command5, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command5, NULL, 0, NULL);
					BGM_Play(hWnd);
					break;

					//撃破 共通
				case 2244:
				case 2414:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "Destroy.mp3");
					// 音量設定を復元
					char command6[128];
					sprintf_s(command6, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command6, NULL, 0, NULL);
					BGM_Play(hWnd);
					break;

					// ゴブリン
				case 2400:
				case 2415:
				case 2505:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "goblin.mp3");

					// 音量設定を復元
					char command7[128];
					sprintf_s(command7, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command7, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;

				case 2410:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "goblin_battle.mp3");

					// 音量設定を復元
					char command8[128];
					sprintf_s(command8, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command8, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;

					// 封印剣
				case 2600:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "sealed_sword.mp3");

					// 音量設定を復元
					char command9[128];
					sprintf_s(command9, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command9, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;

					// 魔王戦
				case 2800:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "Demon_King.mp3");

					// 音量設定を復元
					char command10[128];
					sprintf_s(command10, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command10, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;

					// 封印剣使用の時
				case 2851:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "sealed_sword_used.mp3");

					// 音量設定を復元
					char command11[128];
					sprintf_s(command11, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command11, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;

					// 戦闘
				case 2900:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "Demon_King_battle.mp3");

					// 音量設定を復元
					char command12[128];
					sprintf_s(command12, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command12, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;

					// 撃破後
				case 2980:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "Demon_King_Destroy.mp3");

					// 音量設定を復元
					char command13[128];
					sprintf_s(command13, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command13, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;

				// ゲームクリア(トルゥーエンド)
				case 3000:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "Peritune_Moonlit_Overture.mp3");

					// 音量設定を復元
					char command17[128];
					sprintf_s(command17, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command17, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;

					// ハッピーエンド
				case 4000:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "happy_fin.mp3");

					// 音量設定を復元
					char command18[128];
					sprintf_s(command18, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command18, NULL, 0, NULL);

					BGM_Play(hWnd);
					
					break;

					// バッドエンド
				case 5000:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "bad_fin.mp3");

					// 音量設定を復元
					char command19[128];
					sprintf_s(command19, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command19, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;

					// ゲームオーバー
				case 7000:
					BGM_Close(hWnd);
					BGM_Opne(hWnd, "game_over.mp3");

					// 音量設定を復元
					char command20[128];
					sprintf_s(command20, 128, "setaudio BGM volume to %d", bgmVolume * 5);
					mciSendString(command20, NULL, 0, NULL);

					BGM_Play(hWnd);
					break;
				}
				break;
			}

			if (page == 11) { // 音量調整画面
				if (wParam == VK_LEFT) {
					if (bgmVolume > 0) {
						bgmVolume -= 5;
						if (bgmVolume < 0) bgmVolume = 0;
						char command[128];
						sprintf_s(command, 128, "setaudio BGM volume to %d", bgmVolume * 5);
						if (mciSendString(command, NULL, 0, NULL) != 0) {
							// エラー処理（デバッグ用）
							MessageBox(hWnd, "音量設定に失敗しました", "エラー", MB_OK);
						}
						InvalidateRect(hWnd, NULL, TRUE);
					}
				}
				else if (wParam == VK_RIGHT) {
					if (bgmVolume < 100) {
						bgmVolume += 5;
						if (bgmVolume > 100) bgmVolume = 100;
						char command[128];
						sprintf_s(command, 128, "setaudio BGM volume to %d", bgmVolume * 5);  // 修正済み！
						if (mciSendString(command, NULL, 0, NULL) != 0) {
							MessageBox(hWnd, "音量設定に失敗しました", "エラー", MB_OK);
						}
						InvalidateRect(hWnd, NULL, TRUE);
					}
				}
			}

			// 画面を再描画(WM_PAINTを動かす)
			InvalidateRect(hWnd, NULL, FALSE); // FALSE:上書き (メモ: TRUE:新規(白くなる))
			return 0;				// WinMainに戻る

		case WM_PAINT:				// 表示する時(最初とウインドウが更新された時)

			hdc = BeginPaint(hWnd, &ps);	// 表示開始

			// 表示する画像をメモリDCに展開
			switch (page)
			{
			case 1000:			// タイトル画面
				BBlt(hdc, hMemDC, hTitle);
				break;

			case 1001:			// 1問目			
				BBlt(hdc, hMemDC, hQuiz1);
				//SelectObject(hMemDC, hBack);
				break;

			case 1002:			// 2頁目(正解)
				BBlt(hdc, hMemDC, hOK);
				//SelectObject(hMemDC, hEnemy);
				break;

			case 1003:			// 2問目
				BBlt(hdc, hMemDC, hQuiz2);
				//SelectObject(hMemDC, hMagic);
				break;

			case 1004:
				BBlt(hdc, hMemDC, hOK2);
				//SelectObject(hMemDC, hMagic);
				break;

			case 1005:
				BBlt(hdc, hMemDC, hQuiz1Kotae);
				break;

			case 1006:
				BBlt(hdc, hMemDC, hSteam);
				break;

			case 1100:			// 100頁目(不正解)
				BBlt(hdc, hMemDC, hNG);
				//SelectObject(hMemDC, hMakaron);
				break;

			// ---RPG---
			// タイトル
			case 0:
			case 1:
			case 10:
				BBlt(hdc, hMemDC, hRPGTitle);
				break;

			// スタート
				//冒頭
			case 2000:
			case 2004:
				BBlt(hdc, hMemDC, hStage);
				break;

			case 2007:
			case 2010:
			case 2044:
				BBlt(hdc, hMemDC, hStage_0);
				break;

			case 2011:
				// 王様
				TransBlt(hdc, hMemDC, hKing, 450, 50, 333, 500);
				break;

			case 2014:
			case 2016:
			case 2018:
			case 2020:
			case 2022:
			case 2023:
			case 2025:
			case 2027:
			case 2032:
			case 2040:
			case 2041:
			case 2042:
				BBlt(hdc, hMemDC, hStage_0);
				// 王様
				TransBlt(hdc, hMemDC, hKing, 450, 50, 333, 500);
				break;

			case 2021:
			case 2024:
			case 2029:
				BBlt(hdc, hMemDC, hStage_0);
				// 勇者(拡大版)
				TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				break;

			case 2031:
			case 2036:
				BBlt(hdc, hMemDC, hStage_0);
				// 勇者(超拡大版)
				TransBlt(hdc, hMemDC, hBrave_c, 150, 100, 999, 1500);
				break;

			case 2037:
				BBlt(hdc, hMemDC, hStage_0);
				//姫 & 魔法使い(超拡大版)
				//TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				TransBlt(hdc, hMemDC, hWizard_c, 130, 30, 999, 1500);
				break;

			case 2038:
				BBlt(hdc, hMemDC, hStage_0);
				//姫 & 魔法使い(拡大版)
				TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				break;

			case 2045:
				if (has_princess == true)
				{
					BBlt(hdc, hMemDC, hStage_0);
					//姫 & 魔法使い(拡大版)
					TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				}
				else
				{
					BBlt(hdc, hMemDC, hStage_0);
					// 勇者(拡大版)
					TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				}
				break;

			case 2047:
				if (has_princess == true)
				{
					BBlt(hdc, hMemDC, hStage_0);
					// 勇者(拡大版)
					TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				}
				else
				{
					BBlt(hdc, hMemDC, hStage_0);
					//姫 & 魔法使い(拡大版)
					TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				}
					break;

			case 2048:
				if (has_princess == true)
				{
					BBlt(hdc, hMemDC, hStage_0);
				}
				break;

			case 2049:
				BBlt(hdc, hMemDC, hStage_0);
				break;


//------------------■ステージ1 スライム戦------------------

			// スライム戦
			case 2200:
			case 2208:
			case 2221:
			case 2233:
			case 2238:
			case 2239:
			case 2244:
			case 2245:
			case 2305:
			case 2382:
				BBlt(hdc, hMemDC, hStage_1);
				break;

			case 2205:
			case 2206:
			case 2209:
			case 2211:
			case 2213:
			case 2215:
			case 2217:
			case 2219:
			case 2230:
			case 2232:
			case 2234:
			case 2240:
			case 2241:
			case 2242:
			case 2243:
			case 2250:
			case 2252:
			case 2302:
			case 2304:
			case 2351:
			case 2352:
			case 2380:
				BBlt(hdc, hMemDC, hStage_1);
				TransBlt(hdc, hMemDC, hSlime, 450, 50, 400, 400);
				break;

			case 2300:
			case 2350:
				BBlt(hdc, hMemDC, hStage_1);
				TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				break;


			case 2204:
				if (has_princess == true)
				{
					BBlt(hdc, hMemDC, hStage_1);
					//姫 & 魔法使い(拡大版)
					TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				}
				else
				{
					BBlt(hdc, hMemDC, hStage_1);
					// 勇者(拡大版)
					TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				}
				break;

			case 2207:
				if (has_princess == true)
				{
					BBlt(hdc, hMemDC, hStage_1);
					//姫 & 魔法使い(拡大版)
					TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				}
				else
				{
					BBlt(hdc, hMemDC, hStage_1);
					// 勇者(拡大版)
					TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				}
				break;

//------------------■ステージ2 ゴブリン戦------------------

			// ゴブリン戦
			case 2400:
			case 2414:
			case 2415:
			case 2504:
			case 2506:
			case 2554:
				BBlt(hdc, hMemDC, hStage_2);
				break;

			case 2404:
			case 2553:
				if (has_princess == true)
				{
					BBlt(hdc, hMemDC, hStage_2);
					//姫 & 魔法使い(拡大版)
					TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				}
				else
				{
					BBlt(hdc, hMemDC, hStage_2);
					// 勇者(拡大版)
					TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				}
				break;

			case 2405:
			case 2406:
			case 2408:
			case 2410:
			case 2411:
			case 2452:
			case 2500:
			case 2502:
			case 2505:
			case 2550:
			case 2580:
			case 2582:
				BBlt(hdc, hMemDC, hStage_2);
				TransBlt(hdc, hMemDC, hGoblin, 450, 0, 320, 640);
				break;



//------------------■隠しステージ 封印剣イベント------------------


			// 封印剣イベント
			case 2600:
			case 2606:
			case 2612:
			case 2650:
			case 2652:
			case 2656:
			case 2700:
			case 2705:
			case 2752:
				BBlt(hdc, hMemDC, hStage_10);
				break;

			case 2603:
			case 2610:
			case 2750:
				BBlt(hdc, hMemDC, hStage_10);
				TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				break;

			case 2604:
			case 2611:
				BBlt(hdc, hMemDC, hStage_10);
				if (has_princess == true)
				{
					//姫 & 魔法使い(拡大版)
					TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				}
				break;

			case 2703:
				BBlt(hdc, hMemDC, hStage_10);
				if (has_princess == true)
				{
					//姫 & 魔法使い(拡大版)
					TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				}
				else
				{
					TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				}
				break;


			case 2655:
			case 2751:
				BBlt(hdc, hMemDC, hStage_10);

				if (has_princess == true)
				{
					//姫 & 魔法使い(拡大版)
					TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				}
				else
				{
					TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				}
				break;



//------------------■ステージ3 ラスボス戦------------------

			// ラスボス
			case 2800:
			case 2803:
			case 2850:
			case 2856:
			case 2981:
			case 2983:
			case 2991:
			case 2996:
				BBlt(hdc, hMemDC, hStage_3);
				break;

			case 2802:
			case 2855:
			case 2982:
				BBlt(hdc, hMemDC, hStage_3);
				if (has_princess == true)
				{
					//姫 & 魔法使い(拡大版)
					TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				}
				else
				{
					TransBlt(hdc, hMemDC, hBrave_b, 300, 100, 666, 1000);
				}
				break;

			case 2993:
				BBlt(hdc, hMemDC, hStage_3);
				if (has_princess == true)
				{
					//姫 & 魔法使い(拡大版)
					TransBlt(hdc, hMemDC, hWizard_b, 300, 100, 666, 1000);
				}
				break;

			case 2995:
				BBlt(hdc, hMemDC, hStage_3);
				//姫 & 魔法使い(超拡大版)
				TransBlt(hdc, hMemDC, hWizard_c, 130, 30, 999, 1500);
				break;


			case 2805:
			case 2900:
			case 2901:
			case 2902:
			case 2910:
			case 2911:
			case 2912:
			case 2913:
			case 2920:
			case 2921:
			case 2922:
			case 2923:
			case 2930:
			case 2931:
			case 2932:
			case 2933:
			case 2935:
				BBlt(hdc, hMemDC, hStage_3);
				TransBlt(hdc, hMemDC, hBoss, 450, 50, 333, 500);
				break;

			case 2807:
			case 2851:
			case 2852:
			case 2937:
			case 2980:
				BBlt(hdc, hMemDC, hStage_3);
				TransBlt(hdc, hMemDC, hBoss_b, 300, 0, 666, 1000);
				break;

			case 2854:
				BBlt(hdc, hMemDC, hStage_3);
				TransBlt(hdc, hMemDC, hBoss_c, 300, 0, 666, 1000);
				break;

			case 2808:
				BBlt(hdc, hMemDC, hStage_3);
				if (has_sealed_sword == true)
				{
					TransBlt(hdc, hMemDC, hSorad, 500, 150, 250, 375);
				}
				break;

			case 2990:
				BBlt(hdc, hMemDC, hStage_3);
				TransBlt(hdc, hMemDC, hBrave_c, 150, 100, 999, 1500);
				break;

			// トルゥーエンド
			case 3000:
			case 3001:
			case 3002:
				BBlt(hdc, hMemDC, hTrueFin);
				break;

			// ハッピーエンド
			case 4000:
			case 4001:
			case 4002:
				BBlt(hdc, hMemDC, hHappyFin);
				break;

				// バッドエンド
			case 5000:
			case 5001:
			case 5002:
				BBlt(hdc, hMemDC, hBadFin);
				break;

				// ゲームオーバー画面
			case 7000:
			case 7001:
				BBlt(hdc, hMemDC, hGameOver);
				break;

				// 異例ゲームオーバー
			case 9000:
				BBlt(hdc, hMemDC, hGameOver_2);
				break;

				// ヘルプ画面
			case 10000:
				BBlt(hdc, hMemDC, hHelp);
				break;

			}

			// 文字色の変更(RGB:左から赤、緑、青を0～255で指定)
			SetTextColor(hdc, RGB(0, 0, 0));	// 黒文字

			// 文字の背景色
			SetBkColor(hdc, RGB(255, 255, 0));	// 黄

			// 文字の背景色の透過
			SetBkMode(hdc, TRANSPARENT);		// 戻す:QPAQUE

			// 文字フォントの変更
			SelectObject(hdc, hFont);

			// テキストの表示
			switch (page)
			{
			case 1000:			// タイトル画面
				SelectObject(hdc, hFont_2);
				SetTextColor(hdc, RGB(0, 0, 0));	// 黒文字
				Text(hdc, 350, 330, "VRChatクイズ");

				SelectObject(hdc, hFont_4);
				SetBkColor(hdc, RGB(10, 120, 200));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 450, 500, "Enterキーで進行します♪");

				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 10, "'B'を押下で音量調整画面に飛ぶよ♪");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 50, "'Q'を押下でヘルプ画面を表示");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 90, "'L'を押下でフルスクリーン画面orHD画面に変更");
				break;

			case 1001:			// 1頁目
				if(sel == 0) SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 50, 300, "A:恋人");

				if (sel == 1) SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 50, 350, "B:お砂糖");

				if (sel == 2) SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 50, 400, "C:交際");

				if (sel == 3) SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 50, 450, "D:付人");

				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(200, 0, 200));	// 黒文字
				SetBkColor(hdc, RGB(240, 240, 240));	
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 30, 520, "第1問");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 30, 580, "VRChatにおける、恋人/パートナー/親友");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 30, 640, "としてお付き合いすることを何と言う?");


				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 10, "'B'を押下で音量調整画面に飛ぶよ♪");
				break;

			case 1002:			// 2頁目
				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(0, 0, 255));	// 黒文字
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 600, 200, "正解!");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 180, 290, "もう一問目も正解で解説するよ～♪");


				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 10, "'B'を押下で音量調整画面に飛ぶよ♪");
				break;

			case 1003:			// 3頁目
				if (sel == 0) SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 50, 300, "A:お塩");

				if (sel == 1) SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 50, 350, "B:お別れ");

				if (sel == 2) SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 50, 400, "C:破局");

				if (sel == 3) SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 50, 450, "D:離婚");

				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(200, 0, 200));	// 黒文字
				SetBkColor(hdc, RGB(240, 240, 240));
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 30, 520, "第2問");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 30, 580, "お砂糖でなくなることを何と言う?");


				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 10, "'B'を押下で音量調整画面に飛ぶよ♪");

				break;

			case 1004:			// 4頁目
				SetTextColor(hdc, RGB(0, 0, 255));	// 黒文字
				Text(hdc, 600, 550, "ではまとめに移ります！");
				Text(hdc, 550, 600, "Enterを押下してください♪");
				SelectObject(hdc, hFont_2);
				Text(hdc, 600, 80, "正解～♪");


				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 10, "'B'を押下で音量調整画面に飛ぶよ♪");

				break;

			case 1005:
				SetBkColor(hdc, RGB(255, 240, 245));
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 15, 300, "VRC内での");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 15, 350, "付き合うことを「お砂糖」");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 15, 400, "お砂糖が分かれることを「お塩」と呼びます");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 150, 670, "気になった方はぜひ、調べて楽しんで下さいw");

				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 10, "'B'を押下で音量調整画面に飛ぶよ♪");

				break;

			case 1006:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 200, 45, "そんな VRhat は Steam で無料だよ♬");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 200, 105, "ぜひダウンロードしようね!!!");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 250, 570, "Pを押下で下記URLに飛べるよ♪");
				SelectObject(hdc, hFont_4);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 150, 630, "https://store.steampowered.com/app/438100/VRChat/?l=japanese");

				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 10, "'B'を押下で音量調整画面に飛ぶよ♪");

				break;

			case 1100:			// 100頁目(不正解)
				SelectObject(hdc, hFont_2);
				SetTextColor(hdc, RGB(255, 0, 0));	// 文字
				Text(hdc, 350, 230, "不正解...");
				Text(hdc, 180, 370, "もう一度挑戦してね♪");

				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 10, "'B'を押下で音量調整画面に飛ぶよ♪");

				break;

			case 0:
				SetBkColor(hdc, RGB(255, 240, 245));
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 250, 50, "さっきで終わりだと思った???");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 320, 100, "草草草の草ですわ～ｗ");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 350, 500, "こっからが本番だよ♪");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 450, 550, "頑張ってね♬");

				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 10, "'B'を押下で音量調整画面に飛ぶよ♪");

				break;

			case 1:
				// メニュー表示
				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(0, 0, 0));
				SetBkColor(hdc, RGB(255, 255, 255));

				if (sel == 0) SetBkMode(hdc, OPAQUE);
				Text(hdc, 370, 400, "ゲームスタート");
				SetBkMode(hdc, TRANSPARENT);

				if (sel == 1) SetBkMode(hdc, OPAQUE);
				Text(hdc, 370, 500, "設定");
				SetBkMode(hdc, TRANSPARENT);

				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 10, "'B'を押下で音量調整画面に飛ぶよ♪");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 50, "'Q'を押下でヘルプ画面を表示");
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 10, 90, "'L'を押下でフルスクリーン画面orHD画面に変更");

				SelectObject(hdc, hFont_6);
				SetTextColor(hdc, RGB(0, 0, 255));
				SetBkColor(hdc, RGB(190, 255, 190));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 200, 200, "Choice is Correct?");

				SelectObject(hdc, hFont_5);
				SetTextColor(hdc, RGB(0, 0, 255));	// 青文字
				SetBkColor(hdc, RGB(255, 255, 255));	// 背景
				SetBkMode(hdc, OPAQUE);	//背景色を戻す
				Text(hdc, 120, 650, "RPG戦闘モードは時間の都合上、廃止しています。今後手を加えるかも???汗");
				break;

			case 10:
				// 設定画面
				if (sel == 0) SetBkMode(hdc, OPAQUE);
				Text(hdc, 300, 300, "音量設定");
				SetBkMode(hdc, TRANSPARENT);

				if (sel == 1) SetBkMode(hdc, OPAQUE);
				Text(hdc, 300, 400, "戻る");
				SetBkMode(hdc, TRANSPARENT);

				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(0, 0, 0));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 300, 200, "設定画面");

				break;

				extern int volume;

			case 11:{
				// 仮のメモリDC作成
				HDC memDC = CreateCompatibleDC(hdc);
				HBITMAP memBitmap = CreateCompatibleBitmap(hdc, 1280, 720);
				HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

				// 背景（タイトル画像など）描画
				BBlt(memDC, hMemDC, hRPGTitle);  // ←ここに描く（直接 hdc に描かない！）

				// テキスト描画処理（memDC に全部描く！）
				SelectObject(memDC, hFont_3);
				SetBkMode(memDC, OPAQUE);
				SetBkColor(memDC, RGB(200, 255, 200));
				TextOut(memDC, 200, 600, TEXT("EnterキーでRPGタイトルに戻る"), lstrlen(TEXT("EnterキーでRPGタイトルに戻る")));

				SelectObject(memDC, hFont_2);
				SetBkColor(memDC, RGB(0, 0, 0));
				SetTextColor(memDC, RGB(255, 255, 255));
				TextOut(memDC, 200, 120, TEXT("BGM音量調整"), lstrlen(TEXT("BGM音量調整")));

				TCHAR volumeStr[64];
				wsprintf(volumeStr, TEXT("現在の音量: %d"), bgmVolume);
				TextOut(memDC, 200, 220, volumeStr, lstrlen(volumeStr));
				TextOut(memDC, 200, 320, TEXT("←  音量を下げる"), lstrlen(TEXT("←  音量を下げる")));
				TextOut(memDC, 200, 420, TEXT("→  音量を上げる"), lstrlen(TEXT("→  音量を上げる")));

				SelectObject(memDC, hFont_5);
				SetTextColor(memDC, RGB(0, 0, 255));
				SetBkColor(memDC, RGB(255, 255, 255));
				Text(memDC, 200, 550, "'B'を押下で元の画面に飛ぶよ♪");

				// 最後にまとめて画面に転送（ちらつき防止の肝！）
				BitBlt(hdc, 0, 0, 1280, 720, memDC, 0, 0, SRCCOPY);

				// 後始末
				SelectObject(memDC, oldBitmap);
				DeleteObject(memBitmap);
				DeleteDC(memDC);
				break;
			}


			// ---RPG本編---
//------------------■ステージ0 序章------------------

				// ---冒頭---
			case 2000:
				SetBkColor(hdc, RGB(255, 150, 150)); // ■用 赤枠
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 100, "■序章");
				break;

			case 2001:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "とあるところに、凄腕の冒険者がいた。");
				break;

			case 2002:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "その者は、勇気と優しさを兼ね備えた人物だった。");
				break;

			case 2003:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "多くの者から「勇者」と称えられていた。");
				break;

			case 2004:
				SetBkColor(hdc, RGB(255, 150, 150));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 100, "■序章");

				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "ある日、その勇者は王の目に留まり、");
				break;

			case 2005:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "王から召喚の声がかかった。");
				break;

			case 2006:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "勇者は謁見の場へと招かれ、王城へと足を運んだ。");
				break;


			case 2007:
				// --- テキストウィンドウ風の枠を描画する ---
				DrawTextFrame(hdc, 50, 520, 1230, 700);  // 位置とサイズはお好みで
				DrawTextFrame_2(hdc, 50, 450, 400, 520);  // 位置とサイズはお好みで

				Text(hdc, 70, 460, "勇者(心の声)");
				break;

			case 2008:
				Text(hdc, 100, 550, "王様から声がかかるなんて……");

				break;
			case 2009:
				Text(hdc, 100, 620, "僕も、ついにここまで来たんだ！");
				break;

			case 2010:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "でも、気を緩めちゃいけない……！");
				Text(hdc, 70, 460, "勇者(心の声)");
				break;

			case 2011:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 190, 460, "王");
				break;

			case 2012:
				Text(hdc, 100, 550, "勇者よ、よくぞ参った。 ");
				break;

			case 2013:
				Text(hdc, 100, 620, "そなたの武勇は、すでに耳に届いておる。");
				break;

			case 2014:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "民を代表し、感謝の意を伝えたい。");

				Text(hdc, 190, 460, "王");
				break;

			case 2015:
				Text(hdc, 100, 620, "しかし、王という立場ゆえ、");
				break;

			case 2016:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "頭を下げることはできぬ、すまぬ…。");
				Text(hdc, 190, 460, "王");
				break;

			case 2017:
				Text(hdc, 100, 620, "褒美は後ほど、きちんと用意しよう。");
				break;

			case 2018:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "それとは別に、そなたに頼みごとがある。");
				Text(hdc, 190, 460, "王");
				break;

			case 2019:
				Text(hdc, 100, 620, "それは――");
				break;

			case 2020:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "魔王をどうにかしてほしいのだ。");
				Text(hdc, 190, 460, "王");
				break;

			case 2021:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "魔王……ですか？");
				Text(hdc, 170, 460, "勇者");
				break;

			case 2022:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "うむ。");
				Text(hdc, 190, 460, "王");
				break;

			case 2023:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "最近、世界中で魔物の活動が活発");
				Text(hdc, 100, 620, "になっておるのは知っておるか？");
				Text(hdc, 190, 460, "王");
				break;

			case 2024:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "ええ、最近、魔物が少し強くなっ");
				Text(hdc, 100, 620, "ていると感じています。");
				Text(hdc, 170, 460, "勇者");
				break;

			case 2025:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "「少し」か……ふふ。");
				Text(hdc, 190, 460, "王");
				break;

			case 2026:
				Text(hdc, 100, 620, "そなたにとってはそうかもしれぬが、");
				break;

			case 2027:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "一般の冒険者たちは、もはや手が出せぬ状況だ。");
				Text(hdc, 190, 460, "王");
				break;

			case 2028:
				Text(hdc, 100, 620, "このままでは、民が安心して暮らせなくなる…！");
				break;

			case 2029:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "それは困ります。");
				Text(hdc, 170, 460, "勇者");
				break;

			case 2030:
				Text(hdc, 100, 620, "民の暮らしを守るため、微力ながら……");
				break;

			case 2031:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 170, 460, "勇者");
				SelectObject(hdc, hFont_3);
				Text(hdc, 100, 585, "私が、その原因を断ち切ってみせます！");

				break;

			case 2032:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "おお……頼もしい限りだ。");
				Text(hdc, 190, 460, "王");
				break;

			case 2033:
				Text(hdc, 100, 620, "ことの元凶は、魔王にあるとされておる。");
				break;

			case 2034:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "どうか、魔王のもとへ向かってくれ――");
				Text(hdc, 190, 460, "王");
				break;

			case 2035:
				Text(hdc, 100, 620, "勇者よ！");
				break;

			case 2036:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 170, 460, "勇者");
				SelectObject(hdc, hFont_3);
				Text(hdc, 100, 585, "はい、必ず！");

				break;

			case 2037:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 190, 460, "姫");
				SelectObject(hdc, hFont_3);
				Text(hdc, 100, 585, "お待ちください！");
				break;

			case 2038:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "勇者様、私もお供してよろしいでしょうか？");
				Text(hdc, 190, 460, "姫");
				break;

			case 2039:
				Text(hdc, 100, 620, "私、魔法が得意なんです！");
				break;

			case 2040:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "おぉ、我が娘よ。");
				Text(hdc, 190, 460, "王");
				break;

			case 2041:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "確かに、そなたも類稀なる");
				Text(hdc, 100, 620, "魔法の才を持っていたな。");
				Text(hdc, 190, 460, "王");
				break;

			case 2042:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "勇者よ、娘は必ずそなたの力になるだろう。");
				Text(hdc, 190, 460, "王");
				break;

			case 2043:
				Text(hdc, 100, 620, "どうか、連れて行ってはくれぬか？");
				break;

				// 分岐処理　姫を連れていくか否か
			case 2044:
				SetTextColor(hdc, RGB(250, 0, 130));
				SetBkColor(hdc, RGB(255, 255, 255));
				if (sel == 0) SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 300, "連れていく");
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(50, 30, 250));
				if (sel == 1) SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 400, "連れて行かない");
				SetBkMode(hdc, TRANSPARENT);

				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(255, 30, 200));
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 600, 100, "姫を");
				break;

			case 2045:
				if (has_princess == true)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);

					Text(hdc, 100, 550, "ありがとうございます！");
					Text(hdc, 190, 460, "姫");

				}
				else
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);

					Text(hdc, 100, 550, "申し訳ありません。");
					Text(hdc, 170, 460, "勇者");
				}
				break;

			case 2046:

				if (has_princess == true)
				{
					Text(hdc, 100, 620, "勇者様の旅のお力になれるよう、頑張ります！");
				}
				else
				{
					Text(hdc, 100, 620, "姫を危険な目に遭わせるわけにはいきません……。");
				}
				break;

			case 2047:

				if (has_princess == true)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);

					Text(hdc, 100, 550, "よろしくお願いします！");
					Text(hdc, 170, 460, "勇者");
				}
				else
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);

					Text(hdc, 100, 550, "そ、そうですか……");
					Text(hdc, 190, 460, "姫");
				}
				break;

			case 2048:
				Text(hdc, 100, 620, "わかりました……");
				break;

			case 2049:
				if (has_princess == true)
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "こうして、勇者と姫は");
				}
				else
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "こうして、勇者は");
				}
				break;

			case 2050:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "魔王のもとへと旅立つこととなった。");
				break;


//------------------■ステージ1 スライム戦------------------


			case 2200:
				SetBkColor(hdc, RGB(180, 180, 255)); // ■用 青枠
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 100, "■第一章");
				break;

			case 2201:
				if (has_princess == true)
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "魔王の住処を目指して、一行は進む。");
				}
				else
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "魔王の住処を目指して、勇者は進む。");
				}
				break;

			case 2202:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "草原を抜けようとしていたところ――");
				break;

			case 2203:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "何やら、ぷるぷるとした影が道をふさいでいた……");
				break;

			case 2204:
				if (has_princess == true) {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "勇者様、あれ……何かいます！");
					Text(hdc, 190, 460, "姫");
				}
				else {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "……あれは、スライムか？");
					Text(hdc, 170, 460, "勇者");
				}
				break;

			case 2205:
				SetBkColor(hdc, RGB(200, 200, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 450, "スライムと遭遇した");
				break;

			case 2206:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "プルプル ボク 悪いスライムじゃないよ！");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2207:
				if (has_princess == true)
				{
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "こんなかわいいスライムと戦うのですか? ");
				Text(hdc, 100, 620, "勇者様。");
				Text(hdc, 190, 460, "姫");
				}
				else
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "スライムか、戦うべきか...。");
					Text(hdc, 170, 460, "勇者");
				}
				break;

			case 2208:
				SetTextColor(hdc, RGB(250, 0, 130));
				SetBkColor(hdc, RGB(255, 255, 255));

				if (sel == 0) SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 300, "戦う");
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(50, 30, 250));
				if (sel == 1) SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 400, "戦かわない");
				SetBkMode(hdc, TRANSPARENT);
				break;

				// 戦わない場合
			case 2209:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "ありがとう～");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2210:
				Text(hdc, 100, 620, "心優しい勇者さま!");
				break;

			case 2211:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "魔王様は今、悪いやつに支配されています。");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2212:
				Text(hdc, 100, 620, "おそらく...ゴブリン先輩が、");
				break;

			case 2213:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "その邪を払うものを、");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2214:
				Text(hdc, 100, 620, "知っているかもしれません。");
				break;

			case 2215:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "ぜひ勇者様に魔王様を、");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2216:
				Text(hdc, 100, 620, "お救いいただきたいのです～。");
				break;

			case 2217:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "ゴブリン先輩はおそらく廃墟に");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2218:
				Text(hdc, 100, 620, "いると思うのです～。");
				break;

			case 2219:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "真っ直ぐ行けば");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2220:
				Text(hdc, 100, 620, "会えると思います！");
				break;

			case 2221:
				SetBkColor(hdc, RGB(255, 255, 255));
				if (has_princess == true)
				{
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者一行はスライムの助言通りに");
				}
				else
				{
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者はスライムの助言通りに");
				}
				break;

			case 2222:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "旅路を急ぐのであった。");
				break;

				// 戦う場合
			case 2230:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "そ、そんなぁ～");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2231:
				Text(hdc, 100, 620, "ボク、悪いスライムじゃないよ!");
				break;

			case 2232:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "本当だよ!");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2233:
				SetTextColor(hdc, RGB(250, 0, 130));
				SetBkColor(hdc, RGB(255, 255, 255));

				if (sel == 0) SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 300, "本当に戦いますか?");
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(50, 30, 250));
				if (sel == 1) SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 400, "戦かわない");
				SetBkMode(hdc, TRANSPARENT);
				break;

			case 2234:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "な、なんでなの?");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2235:
				Text(hdc, 100, 620, "分かってほしいの～");
				break;

			case 2236:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "ボ...ボク、悪いスライムじゃないよ!");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2237:
				Text(hdc, 100, 620, "本当だよ!!");
				break;

			case 2238:
				SetBkColor(hdc, RGB(85, 85, 85));
				SetBkMode(hdc, OPAQUE);
				SetTextColor(hdc, RGB(0, 49, 73));
				if (sel == 0)
				{
					SetBkColor(hdc, RGB(255, 255, 255));
				}
				Text(hdc, 500, 300, "No No No");

				SetBkColor(hdc, RGB(85, 85, 85));
				SetTextColor(hdc, RGB(96, 15, 24));
				SetBkMode(hdc, OPAQUE);
				if (sel == 1)
				{
					SetBkColor(hdc, RGB(255, 255, 255));
				}
				Text(hdc, 100, 400, "Yes? 本当に??? ほんとうに? ホントウニ...?");

				SetTextColor(hdc, RGB(0, 0, 0));
				SetBkColor(hdc, RGB(120, 30, 60));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 300, 150, "え...? タタカウノデスカ...???");
				break;

			case 2239:
				SetTextColor(hdc, RGB(255, 0, 0));
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 200, 50, "戦闘を開始します");
				break;

			case 2240:
				SetBkColor(hdc, RGB(200, 200, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 450, "スライムは怯えている");
				break;

			case 2241:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620,"MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}

				SelectObject(hdc, hFont_4);

				SetBkColor(hdc, RGB(245, 245, 245));
				if (sel == 0)SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 515, "攻撃");
				SetBkMode(hdc, TRANSPARENT);

				if (sel == 1)SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 565, "防御");
				SetBkMode(hdc, TRANSPARENT);

				if (sel == 2)SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 615, "見逃す");
				SetBkMode(hdc, TRANSPARENT);

				if (sel == 3)SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 665, "戦いを辞める");
				SetBkMode(hdc, TRANSPARENT);

				break;

				// 攻撃の場合
			case 2242:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}

				SelectObject(hdc, hFont_4);
				SetBkColor(hdc, RGB(245, 245, 245));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 515, "スライム");
				break;

				
			case 2243:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				DrawTextFrame_2(hdc, 800, 50, 1200, 200);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}
				SelectObject(hdc, hFont_4);
				Text(hdc, 820, 80, "勇者の攻撃 1ヒット");
				break;

			case 2244:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				DrawTextFrame_2(hdc, 800, 50, 1200, 200);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}
				SelectObject(hdc, hFont_4);
				Text(hdc, 820, 80, "勇者の攻撃 1ヒット");
				Text(hdc, 820, 140, "スライムは経験値となった");
				break;

			case 2245:
				if (has_princess == true)
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者一行は見事、スライムを倒し");
				}
				else
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者は見事、スライムを倒し");
				}
				break;

			case 2246:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "旅路を急ぐのであった");
				break;

				// 防御の場合
			case 2250:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				DrawTextFrame_2(hdc, 800, 50, 1200, 200);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 80, "勇者と魔法使い(姫)は");

				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 80, "勇者は");
				}
				break;

			case 2251:
				if (has_princess == true)
				{
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 140, "防御の姿勢をとった");

				}
				else
				{
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 140, "防御の姿勢をとった");
				}
				break;

			case 2252:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}
				DrawTextFrame_2(hdc, 100, 50, 500, 255);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "スライムのたーん");
				break;

			case 2253:
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 140, "ぽよぽよはねる");
				break;

			case 2254:
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 200, "何も起こらなかった...");
				break;


				// 見逃す場合
			case 2300:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);

				Text(hdc, 100, 550, "そんなに怯えているなら、");
				Text(hdc, 170, 460, "勇者");
				break;

			case 2301:
				Text(hdc, 100, 620, "見逃してやろう。");
				break;

			case 2302:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "ひ、ひぃ～");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2303:
				Text(hdc, 100, 620, "たたかいたくなかったよ～");
				break;

			case 2304:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "よかったよ～");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2305:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "スライムは");
				break;

			case 2306:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "そそくさと逃げていきました...。");
				break;

			case 2307:
				SetBkColor(hdc, RGB(255, 255, 255));
				if (has_princess == true)
				{
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 400, "勇者一行は旅路を急ぐのであった。");
				}
				else
				{
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 400, "勇者は旅路を急ぐのであった。");
				}
				break;


				// 戦いを辞める場合
			case 2350:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "分かった、戦いをやめよう");
				Text(hdc, 170, 460, "勇者");
				break;

			case 2351:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "うぅ～、怖かったよ～");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2352:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "勇者さま");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2353:
				Text(hdc, 100, 620, "1つ助言を");
				break;


			// 防御3回 強制ゲームオーバー
			case 2380:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "うぅ～、何なのですか～!!!");
				Text(hdc, 125, 460, "スライム");
				break;

			case 2381:
				Text(hdc, 100, 620, "もうっ! これでもくらえ～!!!");
				break;

			case 2382:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "スライムが投げたものが");
				break;

			case 2383:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "地面で炸裂し");
				break;

			case 2384:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "あたり一面を更地へとかえた...");
				break;



//------------------■ステージ2 ゴブリン戦------------------



				// ---ステージ2 ゴブリン戦---
			case 2400:
				SetBkColor(hdc, RGB(180, 180, 255)); // ■用 青枠
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 100, "■第2章");
				break;

			case 2401:
				if (has_princess == true)
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "程なくして、勇者一行は");

				}
				else
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "程なくして、勇者は");

				}
				break;

			case 2402:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "遺跡のような場所にたどり着いた。");
				break;


			case 2403:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "突如、ガサゴソと音がして、何かが現れた--");
				break;

			case 2404:
				if (has_princess == true)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "な、何かいます！　勇者様！ ");
					Text(hdc, 190, 460, "姫");
				}
				else
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "ご、ゴブリン…!?");
					Text(hdc, 170, 460, "勇者");
				}
				break;


			case 2405:
				SetBkColor(hdc, RGB(200, 200, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 450, "ゴブリンと遭遇した");
				break;


			case 2406:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "よくも...");
				Text(hdc, 125, 460, "ゴブリン");
				break;


			case 2407:
				Text(hdc, 100, 620, "かわいい...すらいむ...倒した!");
				break;


			case 2408:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "許さない");
				Text(hdc, 125, 460, "ゴブリン");
				break;


			case 2409:
				Text(hdc, 100, 620, "許さない...ぐぎゃぎゃぎゃーーー！！！");
				break;


			case 2410:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 550, "ゴブリンは怒り狂って襲いかかってきた！");
				break;


			// 戦闘開始
			case 2411:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}

				SelectObject(hdc, hFont_4);

				SetBkColor(hdc, RGB(245, 245, 245));
				if (sel == 0)SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 515, "攻撃");
				SetBkMode(hdc, TRANSPARENT);

				if (sel == 1)SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 565, "防御");
				SetBkMode(hdc, TRANSPARENT);

				if (sel == 2)SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 615, "逃げる");
				SetBkMode(hdc, TRANSPARENT);
				break;

				// 攻撃
			case 2412:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}

				SelectObject(hdc, hFont_4);
				SetBkColor(hdc, RGB(245, 245, 245));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 515, "ゴブリン");
				break;


			case 2413:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				DrawTextFrame_2(hdc, 800, 50, 1200, 200);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}
				SelectObject(hdc, hFont_4);
				Text(hdc, 820, 80, "勇者の攻撃 クリティカル");
				break;

			case 2414:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				DrawTextFrame_2(hdc, 800, 50, 1200, 200);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}
				SelectObject(hdc, hFont_4);
				Text(hdc, 820, 80, "勇者の攻撃 クリティカル");
				Text(hdc, 820, 140, "ゴブリンは経験値となった");
				break;


			case 2415:
				if (has_princess == true)
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者一行は見事、ゴブリンをも倒し");
				}
				else
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者は見事、ゴブリンをも倒し");
				}
				break;

			case 2416:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "魔王への旅路を急ぐのであった");
				break;

				// 防御
			case 2450:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				DrawTextFrame_2(hdc, 800, 50, 1200, 200);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 80, "勇者と魔法使い(姫)は");

				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 80, "勇者は");
				}
				break;

			case 2451:
				if (has_princess == true)
				{
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 140, "防御の姿勢をとった");

				}
				else
				{
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 140, "防御の姿勢をとった");
				}
				break;

			case 2452:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}
				DrawTextFrame_2(hdc, 100, 50, 500, 255);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "ゴブリンのターン");
				break;

			case 2453:
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 140, "たたきつけ");
				break;

			case 2454:
				if (has_princess == true)
				{
					SelectObject(hdc, hFont_4);
					Text(hdc, 120, 200, "勇者が攻撃を防いだ");
				}
				else
				{
					SelectObject(hdc, hFont_4);
					Text(hdc, 120, 200, "勇者は攻撃を防いだ");
				}
				break;


				// 逃げる
			case 2500:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				DrawTextFrame_2(hdc, 800, 50, 1200, 200);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 80, "勇者と魔法使い(姫)は");

				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 80, "勇者は");
				}
				break;

			case 2501:
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 140, "逃げ出そうとした");
				break;

			case 2502:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);

				if (has_princess == true)
				{
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 770, 550, "HP 999");
					Text(hdc, 1000, 550, "MP 300");

					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 770, 620, "HP 600");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					SelectObject(hdc, hFont_2);
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 580, 550, "HP 999");
					Text(hdc, 900, 550, "MP 300");
				}
				DrawTextFrame_2(hdc, 100, 50, 500, 255);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "ゴブリンは");
				break;

			case 2503:
				DrawTextFrame_2(hdc, 100, 50, 500, 255);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "ゴブリンは");
				Text(hdc, 120, 140, "逃がしてくれない!!!");
				break;

			case 2504:
				if (has_princess == true)
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者一行は必死で逃げ出した！");
				}
				else
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者は必死で逃げ出した！");
				}
				break;

			case 2505:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "ぎゃ!……逃がした……!");
				Text(hdc, 125, 460, "ゴブリン");
				break;

			case 2506:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "逃げ込んだ先は、静かな神殿のような場所だった。");
				break;

			case 2507:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "その中心には、台座に刺さった剣が一本……");
				break;

			// スライムを倒していない場合
			case 2550:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "……襲ってくる気配はない。");
				break;

			case 2551:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "何かを伝えたげに、");
				break;

			case 2552:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 500, "ゴブリンが遺跡の奥を指さした。");
				break;

			case 2553:
				if (has_princess == true) {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "……この先に何かあるのかしら？");
					Text(hdc, 190, 460, "姫");
				}
				else {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "……あの方向か……");
					Text(hdc, 170, 460, "勇者");
				}
				break;


			case 2554:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "導かれるように、");
				break;

			case 2555:
				if (has_princess == true) {
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 300, "勇者一行は奥へと足を踏み入れた……");
				}
				else {
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 300, "勇者は奥へと足を踏み入れた……");
				}
				break;

				// 防御をし過ぎた場合
			case 2580:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "防いでばかり!");
				Text(hdc, 125, 460, "ゴブリン");
				break;

			case 2581:
				Text(hdc, 100, 620, "ふざけるな！ これでもくらっていろ!!!");
				break;

			case 2582:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "ゴブリンが投げたものが");
				break;

			case 2583:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "地面で炸裂し");
				break;

			case 2584:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "あたり一面を更地へとかえた...");
				break;




//------------------■隠しステージ 封印剣イベント------------------


				// 隠しステージ(封印剣イベント用)
			case 2600:
				SetBkColor(hdc, RGB(180, 180, 255)); // ■用 青枠
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 100, "■第???章");
				break;

			case 2601:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "祭壇のような台座に、");
				break;

			case 2602:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "一本の剣が突き刺さっている……");
				break;


			case 2603:
				if (enemies_killed > 0)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "……これは、剣……？");
					Text(hdc, 170, 460, "勇者");
				}
				else
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "……これが、封印剣……？");
					Text(hdc, 170, 460, "勇者");
				}
				break;

			case 2604:
				if (has_princess == true)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "とても不思議な……優しさと、");
					Text(hdc, 190, 460, "姫");
				}
				else
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "不思議な気配が");
				}
				break;

			case 2605:
				if (has_princess == true)
				{
					Text(hdc, 100, 620, "禍々しさが混ざったような……");
				}
				else
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 300, "剣から溢れている。");
				}
				break;

			case 2606:
				if (enemies_killed > 0 && sealed_sword != 2)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "……汝、既に手を血で汚した者……");
					Text(hdc, 150, 460, "？？？");

				}
				else if (enemies_killed == 0 && sealed_sword != 2)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "……汝、心の弱き者……");
					Text(hdc, 150, 460, "？？？");
				}
				else
				{
					SetTextColor(hdc, RGB(255, 255, 255));
					SetBkColor(hdc, RGB(0, 0, 0));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 100, "剣が、まるで応えるかのように");
				}
				break;

			case 2607:
				if (enemies_killed >= 0 && sealed_sword != 2)
				{
					Text(hdc, 100, 620, "この剣に触れる資格は、ない……");
				}
				else
				{
					SetTextColor(hdc, RGB(255, 255, 255));
					SetBkColor(hdc, RGB(0, 0, 0));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 500, "輝きを放った！");
				}
				break;


				// 封印剣の取得資格有り 抜くか否かの選択
			case 2610:
				if (has_princess == true)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "この剣を抜くべきか……？");
					Text(hdc, 170, 460, "勇者");

				}
				else
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "この剣を抜くべきか……？");
					Text(hdc, 70, 460, "勇者(心の声)");
				}
				break;

			case 2611:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "……決めるのは、あなたです。");
				Text(hdc, 190, 460, "姫");
				break;

			case 2612:
				SetTextColor(hdc, RGB(250, 0, 130));
				SetBkColor(hdc, RGB(255, 255, 255));
				if (sel == 0) SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 300, "抜く");
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(50, 30, 250));
				if (sel == 1) SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 400, "抜かない");
				SetBkMode(hdc, TRANSPARENT);

				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(0, 0, 0));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 600, 100, "剣を");
				break;

				// 封印剣の取得資格がない場合
			case 2650:
				break;

			case 2651:
				Text(hdc, 100, 620, "この剣に触れる資格は、ない。");
				break;

			case 2652:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "剣は闇に飲み込まれ");
				break;

			case 2653:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "まるで最初からなかったかのように");
				break;

			case 2654:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "目の前から消えていった...。");
				break;

			case 2655:
				if (has_princess == true)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "...そ、そんな。");
					Text(hdc, 190, 460, "姫");
				}
				else
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "なっ!？");
					Text(hdc, 170, 460, "勇者");
				}
				break;

			case 2656:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "勇者は、剣を手にすることができず");

				break;

			case 2657:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "重く沈む心を抱えたまま");

				break;

			case 2658:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "魔王の元へ向かうのだった...。");

				break;


				// 封印剣の取得資格が有り 取得する場合
			case 2700:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "勇者が手を伸ばすと");
				break;

			case 2701:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "剣はすっと抜け");
				break;

			case 2702:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "光を放った--");
				break;

			case 2703:
				if (has_princess == true)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "その剣……");
					Text(hdc, 190, 460, "姫");
				}
				else
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "この剣……");
					Text(hdc, 170, 460, "勇者");
				}

				break;

			case 2704:
				if (has_princess == true)
				{
					Text(hdc, 100, 620, "世界を救えるかも知れません……！");
				}
				else
				{
					Text(hdc, 100, 620, "世界を救えるかも知れない……！");
				}
				break;

			case 2705:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "封印剣を手にした勇者は");
				break;

			case 2706:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "魔王の元へ向かうのだった!");
				break;



				// 封印剣の取得資格が有り 取得しない場合
			case 2750:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 100, 550, "……やめておこう。");
				Text(hdc, 170, 460, "勇者");
				break;

			case 2751:
				if (has_princess == true)
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "はい……私は、勇者様の判断を信じます。");
					Text(hdc, 190, 460, "姫");
				}
				else
				{
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "……やめておこう。");
					Text(hdc, 100, 620, "力に頼るだけでは、救えないものがある。");
					Text(hdc, 170, 460, "勇者");
				}
				break;

			case 2752:
				if (has_princess == true)
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者と姫は");
				}
				else
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者は");
				}
				break;

			case 2753:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "剣に背を向け");
				break;

			case 2754:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "その場を去った...。");
				break;



//------------------■ステージ3 ラスボス戦------------------

				
				// ---ステージ3 ボス戦---
			case 2800:
				SetBkColor(hdc, RGB(180, 180, 255)); // ■用 青枠
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 100, "■最終章");
				break;

			case 2801:
				if (has_princess == true)
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者一行は、ついに魔王の間へと辿り着いた。");
				}
				else
				{
					SetBkColor(hdc, RGB(255, 255, 255));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "勇者は、ついに魔王の間へと辿り着いた。");
				}
				break;

			case 2802:
				if (has_princess == true) {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "ついに……来たのですね");
					Text(hdc, 190, 460, "姫");
				}
				else {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "ここが、魔王の…");
					Text(hdc, 170, 460, "勇者");
				}
				break;

			case 2803:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "玉座の間に禍々しいオーラを");
				break;

			case 2804:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "纏った魔王が立っていた...");
				break;

			case 2805:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 170, 460, "魔王");
				if (has_princess == true)
				{
					Text(hdc, 100, 550, "ほう……姫まで連れてくるとは……愚かだな。");
				}
				else
				{
					Text(hdc, 100, 550, "……来たか、勇者よ。");
				}
				break;

			case 2806:
				if (has_princess == true)
				{
					Text(hdc, 100, 620, "まぁよい……来るがいい!");
				}
				else
				{
					Text(hdc, 100, 620, "さぁ、来るがよい...");
				}

				break;

			case 2807:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 170, 460, "魔王");
				SelectObject(hdc, hFont_3);
				Text(hdc, 420, 585, "蹴散らしてやる!!!");
				break;

			case 2808:
				SelectObject(hdc, hFont_2);
				SetTextColor(hdc, RGB(250, 0, 250));
				SetBkColor(hdc, RGB(0, 0, 0));
				if (has_sealed_sword == true)
				{
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 30, 550, "勇者は封印剣を構えた……");
				}
				else if (has_princess == true && has_sealed_sword == false)
				{
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 30, 550, "勇者一行は武器を構えた…");
				}
				else
				{
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 120, 550, "勇者は剣を構えた……");
				}
				break;

				// 封印剣所持時ルート
			case 2850:
				SetTextColor(hdc, RGB(150, 0, 0));
				SetBkColor(hdc, RGB(255, 255, 255));
				if (sel == 0) SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 300, "使用する");
				SetBkMode(hdc, TRANSPARENT);

				SetTextColor(hdc, RGB(0, 0, 150));
				if (sel == 1) SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 400, "使用しない");
				SetBkMode(hdc, TRANSPARENT);

				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(0, 0, 0));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 300, 100, "封印剣を使用しますか？");
				break;

			case 2851:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 170, 460, "魔王");
				Text(hdc, 100, 550, "……そ、それは……！やめろぉぉぉ！");
				break;

			case 2852:
				SelectObject(hdc, hFont_2);
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(0, 0, 0));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 200, 500, "封印剣が光を放ち");
				break;

			case 2853:
				SelectObject(hdc, hFont_2);
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(0, 0, 0));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 200, 600, "魔王を包み込んだ");
				break;

			case 2854:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 170, 460, "魔王");
				Text(hdc, 100, 550, "う……我は……いったい……？");
				break;

			case 2855:
				if (has_princess == true) {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "正気に戻ったのですね……！");
					Text(hdc, 190, 460, "姫");
				}
				else {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "これで……全て終わったのか");
					Text(hdc, 170, 460, "勇者");
				}
				break;

			case 2856:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "こうして、魔王から邪気が取り除かれ");
				break;

			case 2857:
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "世界に平和が訪れた!");
				break;


				// 通常戦闘ルート (やり方が分からないため...全部書き出し)

			case 2900:
				SetTextColor(hdc, RGB(255, 0, 0));
				SetBkColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 200, 50, "戦闘を開始します");
				break;

				// 1&2ターンのみ繰り返し使用
			case 2901:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}


				SetTextColor(hdc, RGB(0, 0, 0));
				DrawTextFrame_2(hdc, 100, 50, 500, 255);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "魔王は不敵な笑みを");
				Text(hdc, 120, 140, "浮かべている...");
				break;

				// 選択肢
			case 2902:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}


				SelectObject(hdc, hFont_4);
				SetBkColor(hdc, RGB(245, 245, 245));
				if (sel == 0)SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 515, "攻撃");
				SetBkMode(hdc, TRANSPARENT);

				if (sel == 1)SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 565, "防御");
				SetBkMode(hdc, TRANSPARENT);

				if (sel == 2)SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 615, "逃げる");
				SetBkMode(hdc, TRANSPARENT);
				break;


				// 攻撃の場合
			case 2910:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}

				SelectObject(hdc, hFont_4);
				SetBkColor(hdc, RGB(245, 245, 245));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 65, 515, "魔王");
				break;

			case 2911:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}			
				DrawTextFrame_2(hdc, 800, 50, 1200, 320);
				SelectObject(hdc, hFont_4);
				Text(hdc, 820, 80, "勇者の攻撃!");
				Text(hdc, 820, 140, "魔王に100のダメージ！");
				bossHP -= 100;
				
				if (has_princess == true && princessHP > 0)
				{
					SelectObject(hdc, hFont_4);
					Text(hdc, 820, 200, "魔法使い(姫)の攻撃！");
					Text(hdc, 820, 260, "魔王に150のダメージ！");
					bossHP -= 150;
				}

			break;

			// 3ターン目使用
			case 2912:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}

				SetTextColor(hdc, RGB(0, 0, 0));
				DrawTextFrame_2(hdc, 100, 50, 500, 255);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "魔王が動き出した！");
				break;

			// 4ターン目から行動する
			case 2913:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}

				SetTextColor(hdc, RGB(0, 0, 0));
				DrawTextFrame_2(hdc, 100, 50, 500, 300);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "魔王の攻撃！");
				Text(hdc, 120, 140, "勇者に300のダメージ！");
				braveHP -= 300;
				if (has_princess == true && princessHP > 0)
				{
					Text(hdc, 120, 200, "魔王使い(姫)に");
					Text(hdc, 120, 260, "150のダメージ！");
					princessHP -= 150;
				}
				if (braveHP <= 0) page = 6999;
				break;


				// --- 防御（軽減なし、演出のみ） ---
			case 2920:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);

				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}
				if (has_princess == true)
				{
					SelectObject(hdc, hFont_4);
					SetBkColor(hdc, RGB(245, 245, 245));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 65, 515, "勇者と魔法使い");
				}
				else
				{
					SelectObject(hdc, hFont_4);
					SetBkColor(hdc, RGB(245, 245, 245));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 65, 515, "勇者");
				}
				
				break;


			case 2921:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}

				DrawTextFrame_2(hdc, 800, 50, 1200, 320);
				SelectObject(hdc, hFont_4);
				Text(hdc, 820, 80, "勇者は防御した！");
				if (has_princess == true && princessHP > 0)
				{
					Text(hdc, 820, 140, "魔法使い(姫)も防御した！");
				}
				break;

				// 3ターン目使用
			case 2922:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}

				SetTextColor(hdc, RGB(0, 0, 0));
				DrawTextFrame_2(hdc, 100, 50, 500, 255);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "魔王が動き出した！");
				break;


				// 防御の時の魔王の攻撃
			case 2923:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}

				SetTextColor(hdc, RGB(0, 0, 0));
				DrawTextFrame_2(hdc, 100, 50, 500, 370);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "魔王の攻撃！");
				Text(hdc, 120, 140, "ダメージは防げなかった！");
				Text(hdc, 120, 200, "勇者に200のダメージ！");
				braveHP -= 200;
				if (has_princess == true && princessHP > 0)
				{
					Text(hdc, 120, 260, "魔王使い(姫)に");
					Text(hdc, 120, 320, "100のダメージ！");
					princessHP -= 100;
				}
				if (braveHP <= 0)
				{
					page = 6999;
				}
				break;



				//逃げる 3回目で 強制ゲームオーバー
			case 2930:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}
				DrawTextFrame_2(hdc, 800, 50, 1200, 320);
				SelectObject(hdc, hFont_4);
				Text(hdc, 820, 80, "勇者は逃げだそうとした！");
				if (has_princess == true)
				{
					Text(hdc, 820, 140, "魔法使い(姫)も");
					Text(hdc, 820, 200, "逃げ出そうとした！");
				}
				escapeTry++;
				if (escapeTry >= 3)
				{
					page = 2934; // ゲームオーバールート
				}
				break;

			case 2931:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 170, 460, "魔王");
				Text(hdc, 100, 550, "逃げられると思ったか！");
				break;

			case 2932:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}

				SetTextColor(hdc, RGB(0, 0, 0));
				DrawTextFrame_2(hdc, 100, 50, 500, 255);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "魔王は逃がしてくれない!!!");
				break;

				// 3ターン目使用
			case 2933:
				DrawTextFrame(hdc, 310, 500, 1230, 710);
				DrawTextFrame_2(hdc, 50, 500, 300, 710);
				if (has_princess == true)
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					TextOut(hdc, 770, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 380, 550, "勇者　　 　  : ");
					Text(hdc, 1000, 550, "MP 300");

					TCHAR princessStr[64];
					wsprintf(princessStr, TEXT("HP %d"), princessHP);
					TextOut(hdc, 770, 620, princessStr, lstrlen(princessStr));
					Text(hdc, 380, 620, "魔法使い(姫) : ");
					Text(hdc, 1000, 620, "MP 999");
				}
				else
				{
					TCHAR breavStr[64];
					wsprintf(breavStr, TEXT("HP %d"), braveHP);
					SelectObject(hdc, hFont_2);
					TextOut(hdc, 580, 550, breavStr, lstrlen(breavStr));
					Text(hdc, 330, 550, "勇者:");
					Text(hdc, 900, 550, "MP 300");
				}

				SetTextColor(hdc, RGB(0, 0, 0));
				DrawTextFrame_2(hdc, 100, 50, 500, 255);
				SelectObject(hdc, hFont_4);
				Text(hdc, 120, 80, "魔王が動き出した！");
				break;


			case 2935:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 170, 460, "魔王");
				Text(hdc, 100, 550, "逃げてばかり...もういい");
				break;


			case 2936:
				Text(hdc, 100, 620, "消えろ!!!");
				break;

			case 2937:
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(0, 0, 0));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 550, "波動に勇者は飲み込まれた...");
				braveHP = 0;
				page = 6999;
				break;


				// --- 魔王撃破後 エンド分岐 ---
			case 2980:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 170, 460, "魔王");
				SelectObject(hdc, hFont_3);
				Text(hdc, 200, 585, "なぜだ!!なぜ我が負ける～!!!");
				break;


			case 2981:
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(0, 0, 0));
				SetBkMode(hdc, OPAQUE);
				SelectObject(hdc, hFont_2);
				Text(hdc, 100, 500, "魔王は崩れ落ちた……");
				break;

			case 2982:
				if (has_princess == true) {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "終わったの...ですね?");
					Text(hdc, 190, 460, "姫");
				}
				else {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "終わった...のか?");
					Text(hdc, 170, 460, "勇者");
				}
				break;

			case 2983:
				if (enemies_killed == 2)
				{
					SetTextColor(hdc, RGB(255, 255, 255));
					SetBkColor(hdc, RGB(0, 0, 0));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 100, 200, "しかし、邪悪なオーラは……勇者に取り憑き……");
					page = 2989;
				}
				else
				{
					SelectObject(hdc, hFont_2);
					SetTextColor(hdc, RGB(255, 255, 255));
					SetBkColor(hdc, RGB(0, 0, 0));
					SetBkMode(hdc, OPAQUE);
					Text(hdc, 50, 500, "こうして世界は救われた--");
					page = 2999;
				}
				break;


			case 2990:
				DrawTextFrame(hdc, 50, 520, 1230, 700);
				DrawTextFrame_2(hdc, 50, 450, 400, 520);
				Text(hdc, 170, 460, "勇者");
				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(200, 0,0));
				Text(hdc, 400, 585, "ぐ、ぐわぁ～");
				break;

			case 2991:
				SetTextColor(hdc, RGB(100, 15, 35));
				SetBkColor(hdc, RGB(240, 240, 240));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "勇者は我を忘れて");
				break;

			case 2992:
				SetTextColor(hdc, RGB(100, 15, 35));
				SetBkColor(hdc, RGB(240, 240, 240));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "手あたり次第に剣を振り回すのであった");
				break;

			case 2993:
				if (has_princess == true) {
					DrawTextFrame(hdc, 50, 520, 1230, 700);
					DrawTextFrame_2(hdc, 50, 450, 400, 520);
					Text(hdc, 100, 550, "そ、そんな勇者様...");
					Text(hdc, 190, 460, "姫");
				}
				else
				{
					page = 2995;
				}
				break;

			case 2994:
				Text(hdc, 100, 620, "た、たすけない...かはっ!?!?!");
				break;

			case 2995:
				SelectObject(hdc, hFont_2);
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkColor(hdc, RGB(0, 0, 0));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 500, "勇者の剣が姫を貫いた...");
				princessHP = 0;
				break;

			case 2996:
				SetTextColor(hdc, RGB(100, 15, 35));
				SetBkColor(hdc, RGB(100, 100, 100));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 100, "その後の話");
				break;

			case 2997:
				SetTextColor(hdc, RGB(100, 15, 35));
				SetBkColor(hdc, RGB(100, 100, 100));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "それはそれは悲惨であった");
				break;

			case 2998:
				SetTextColor(hdc, RGB(100, 15, 35));
				SetBkColor(hdc, RGB(100, 100, 100));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 300, "勇者の持つ力と");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "邪悪なオーラの力によって");
				break;

			case 2999:
				SetTextColor(hdc, RGB(100, 15, 35));
				SetBkColor(hdc, RGB(100, 100, 100));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 300, 600, "世界は終焉へと向かった...");
				page = 4999;
				break;


// ----------■エンディング----------


				// トルゥーエンディング
			case 3001: 
				SelectObject(hdc, hFont_2);
				SetTextColor(hdc, RGB(0, 0, 0));
				SetBkColor(hdc, RGB(220, 200, 220));	// 背景
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 300, 300, "ゲームクリア！");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "おめでとうございます！");
				break;

			case 3002:
				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(0, 0, 0));
				SetBkColor(hdc, RGB(220, 200, 220));	// 背景
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 100, "遊んでくれありがとうね～!");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 200, "あと何個か、隠しエンディング");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 300, "あるんだけど…");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 400, "ぜひ、探してみてね♬");

				SelectObject(hdc, hFont_3);
				SetBkMode(hdc, OPAQUE);
				SetBkColor(hdc, RGB(200, 255, 200));	// 背景
				TextOut(hdc, 200, 630, TEXT("EnterキーでRPGタイトルに戻る"), lstrlen(TEXT("EnterキーでRPGタイトルに戻る")));
				break;


				// ハッピーエンド(封印剣イベントクリア)
			case 4001:
				SelectObject(hdc, hFont_2);
				SetTextColor(hdc, RGB(0, 0, 0));
				SetBkColor(hdc, RGB(220, 200, 220));	// 背景
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 300, 300, "ゲームクリア！");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 400, "おめでとうございます！");
				break;

			case 4002:
				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(0, 0, 0));
				SetBkColor(hdc, RGB(220, 200, 220));	// 背景
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 50, "遊んでくれありがとうね～!");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 150, "ここに来れるなんて...");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 250, "すごいすご～い!!!");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 350, "他にもエンディングがあるから");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 450, "ぜひ、探してみてね♬");


				SelectObject(hdc, hFont_3);
				SetBkMode(hdc, OPAQUE);
				SetBkColor(hdc, RGB(200, 255, 200));
				TextOut(hdc, 200, 640, TEXT("EnterキーでRPGタイトルに戻る"), lstrlen(TEXT("EnterキーでRPGタイトルに戻る")));
				break;


				// バッドエンド(殺戮エンド)
			case 5001:
				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(15, 1, 3));
				SetBkColor(hdc, RGB(30, 30, 30));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 300, 100, "ゲーム完走です...");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 250, 540, "おめでとうございます...");
				break;

			case 5002:
				SelectObject(hdc, hFont_3);
				SetTextColor(hdc, RGB(0, 0, 0));
				SetBkColor(hdc, RGB(105, 50, 105));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 30, "遊んでくれありがとございます...。");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 120, "ここに来られるなんて...");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 220, "おどろきです...。");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 320, "他エンドもあります。");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 50, 420, "見つけていただけると幸いです...。");

				SelectObject(hdc, hFont_3);
				SetBkMode(hdc, OPAQUE);
				SetBkColor(hdc, RGB(105, 50, 50));
				TextOut(hdc, 200, 520, TEXT("EnterキーでRPGタイトルに戻る"), lstrlen(TEXT("EnterキーでRPGタイトルに戻る")));
				break;


				// ゲームオーバー
			case 7001:
				SetTextColor(hdc, RGB(150, 10, 30));
				SetBkColor(hdc, RGB(25, 25, 25));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 100, "魔王戦で負けたのかな?");

				break;

			case 7002:
				SetTextColor(hdc, RGB(150, 10, 30));
				SetBkColor(hdc, RGB(25, 25, 25));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "行動パターンを見れば、攻略簡単だよ?");
				break;

			case 7003:
				SetTextColor(hdc, RGB(150, 10, 30));
				SetBkColor(hdc, RGB(25, 25, 25));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 450, "もう一度、頑張って	!!!");

				SelectObject(hdc, hFont_3);
				SetBkMode(hdc, OPAQUE);
				SetBkColor(hdc, RGB(100, 100, 200));
				TextOut(hdc, 300, 550, TEXT("Enterキーで魔王戦に戻る"), lstrlen(TEXT("Enterキーで魔王戦に戻る")));
				break;


				// 異例ゲームオーバー
			case 9001:
				SetBkColor(hdc, RGB(25, 25, 25));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 100, "はい!?");
				break;

			case 9002:
				SetBkColor(hdc, RGB(25, 25, 25));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 400, 100, "なにしたの!!!???");
				break;

			case 9003:
				SetBkColor(hdc, RGB(25, 25, 25));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 200, "ここに来たってことは...");
				break;

			case 9004:
				SetBkColor(hdc, RGB(25, 25, 25));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 800, 200, "まさか...");
				break;

			case 9005:
				SetBkColor(hdc, RGB(25, 25, 25));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 450, "もう～ちゃんとあそんでくれないと～");
				break;

			case 9006:
				SetBkColor(hdc, RGB(25, 25, 25));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 550, "ぼくはかなしいです、本当にかなしい泣");
				break;

			case 9007:
				SetBkColor(hdc, RGB(25, 25, 25));
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 650, "さいっっっしょからやり直しだから...いいね!?");
				break;


			// ---ヘルプ画面---
			case 10000:
				SelectObject(hdc, hFont);
				SetBkColor(hdc, RGB(250, 200, 250));	// 背景
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 90, "Q");

				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 150, "L");

				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 260, "B");

				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 370, "W");
				Text(hdc, 150, 370, "or");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 220, 370, "↑");
				
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 430, "S");
				Text(hdc, 150, 430, "or");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 220, 430, "↓");
				
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 100, 500, "ENTER");

				SetBkMode(hdc, OPAQUE);
				Text(hdc, 200, 630, "この画面ではQで元の画面に戻ります！");


				SetBkColor(hdc, RGB(220, 200, 220));	// 背景
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 350, 90, "で元の画面に戻る");

				SetBkMode(hdc, OPAQUE);
				Text(hdc, 350, 150, "でフルスクリーン画面に変更");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 410, 200, "HD画面に変更");
				
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 350, 260, "で音量調整画面を表示");
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 350, 310, "音量調整画面から戻る");

				SetBkMode(hdc, OPAQUE);
				Text(hdc, 350, 370, "で選択肢↑に移動");
				
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 350, 430, "で選択肢↓に移動");

				SetBkMode(hdc, OPAQUE);
				Text(hdc, 350, 500, "で次の画面に移る");

				SetBkMode(hdc, OPAQUE);
				Text(hdc, 350, 550, "選択を決定する");


				SetBkColor(hdc, RGB(220, 150, 220));	// 背景
				SetBkMode(hdc, OPAQUE);
				Text(hdc, 500, 20, "ヘルプ画面");
				break;
			}

			EndPaint(hWnd, &ps);			// 表示終了
			return 0;

		case WM_DESTROY:			// ウインドウが終了された時
			//=== 終了処理を置く所 ===

			// BGMの終了
			BGM_Close(hWnd);

			// フリーフォントの削除
			RemoveFontResource(fontPath);

			PostQuitMessage(0);		// アプリケーションの終了をwindowsに伝える
			return 0;				// WinMainに戻る
	}

	// メインに返す情報
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//メイン(Windowsプログラムのメイン)
int WINAPI WinMain(
	HINSTANCE hInstance,		// インスタンス番号(ウィンドウの番号)
	HINSTANCE hPrevInstance,	// 今回は使わない
	PSTR lpCmdLine,				// 今回は使わない
	int nCmdShow)				// 今回は使わない
{
	WNDCLASS wc;				// ウィンドウプログラム構造体
	HWND hWnd;					// ウィンドウハンドル
	MSG msg;					// ウインドウメッセージ

	// ウィンドウクラス(作るウィンドウの情報)の登録
	wc.style = CS_HREDRAW | CS_VREDRAW;					// 縦横のサイズが変更された時再描画
	wc.lpfnWndProc = WinidowProc;						// ウインドウ関数の登録
	wc.cbClsExtra = 0;									// 使わない
	wc.cbWndExtra = 0;									// 使わない
	wc.hInstance = hInstance;							// インスタンス番号
	wc.hIcon = NULL;									// アイコンの登録(デフォルトを設定)
	wc.hCursor = NULL;									// カーソルの登録(デフォルトを設定)
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND + 1;	// ウインドウの背景色
	wc.lpszMenuName = NULL;								// 使わない
	wc.lpszClassName = APPLI_NAME;						// アプリケーション名

	// ウインドウクラスをWindowsに登録
	if (RegisterClass(&wc) == false)
	{
		// 登録できなかった時のエラーメッセージの表示
		MessageBox(NULL, "ウィンドウクラスの登録に失敗","エラーメッセージ",MB_OK);
		return 0;
	}

	// ウインドウ領域の整理
	RECT rect;							// 矩形構造体	"四角形"のことを"矩形(くけい)"と呼ぶ
	DWORD dwStyle;						// ウインドウスタイル
	rect.top = 0;						// 上
	rect.left = 0;						// 左
	rect.right = WND_W;					// 右
	rect.bottom = WND_H;				// 下
	dwStyle = WS_OVERLAPPEDWINDOW;		// 普通のウインドウ (最大最小閉じるボタンとタイトルバー付き)

	if (!AdjustWindowRect(		// アジャスト 調整 rect四角
		&rect,		// (in)画面サイズが入った矩形構造体
					// (out)計算結果
		dwStyle,	// ウインドウスタイル
		NULL))		// メニューを持つかどうかの指定
	{
		MessageBox(NULL, "ウインドウ領域の調整に失敗", "エラーメッセージ", MB_OK);
		return 0;
	}

	// ウインドウの作成
	hWnd = CreateWindow(
		APPLI_NAME,				// アプリケーション名
		TITLE_NAME,				// ウインドウのタイトル
		dwStyle,				// 普通のウインドウ (最大最小閉じるボタンとタイトルバーのウインドウ)
		100, 50,				// 表示するXY座標
		rect.right-rect.left,	// ウインドウの幅
		rect.bottom-rect.top,	// ウインドウの高さ
		NULL,					// 親ウインドウのハンドル(これが親なので無し)
		NULL,					// メニューの設定(メニューは使用しない)
		hInstance,				// インスタンス番号
		NULL);					// 使いません(NULLを入れておく)

	if (hWnd == NULL)
	{
		// 登録できなかった時のエラーメッセージの表示
		MessageBox(NULL, "ウィンドウクラスの作成に失敗", "エラーメッセージ", MB_OK);
		return 0;
	}

	// ウインドウを表示する
	ShowWindow(hWnd, SW_SHOW);

	// メッセージループ(ウインドウが閉じられるまで無限ループ)
	while (GetMessage(&msg,NULL,0,0) > 0)
	{
		// ウインドウ関数にメッセージを送る
		DispatchMessage(&msg);
	}

	return 0;		// プログラムの終了をWindowsに伝える
}