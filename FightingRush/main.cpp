#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
#include "Player.h"




// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// windowモードで起動する
	ChangeWindowMode(1);

	// ウィンドウサイズを16:9に
	SetGraphMode(Game::kScreenWidth,Game::kScreenHeight,Game::kColorDepth);

	//　ウィンドウのタイトル表示を変更
	SetMainWindowText("Fighting Rush");

	// 背景色
	SetBackgroundColor(0, 0, 0);

	// アニメーション管理用の変数
	int frameCount = 0;	// 全体のフレーム数を数える

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// ゲームシーンの作成
	SceneMain scene;
	scene.Init();

	// メインループ
	while (ProcessMessage() == 0)
	{
		// 現在のフレーム開始時刻を取得
		LONGLONG start = GetNowHiPerformanceCount();

		// 画面をクリア
		ClearDrawScreen();

		scene.Update();
		scene.Draw();

		// 1フレームごとにカウントアップ
		frameCount++;
		
		// 画面の書き換えを待つ
		ScreenFlip();

		// escキーを押したらゲームを強制終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
		

		// 画面のリフレッシュレートにかかわらず1/60経過するまで待つ
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 時間経過まで何もせずに待つ
		}
	}

	scene.End();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}