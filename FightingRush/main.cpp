#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
#include "Player.h"
#include "SceneTitle.h"
#include "SceneGameOver.h"
#include "SceneClear.h"

namespace
{
	enum SceneType
	{
		kSceneTitle,
		kSceneMain,
		kSceneGameOver,
		kSceneClear,
	};
}


// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// windowモードで起動する
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを16:9に
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

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
	SceneMain sceneMain;
	SceneTitle sceneTitle;
	SceneGameOver sceneGameOver;
	SceneClear sceneClear;

	// 現在実行したいシーンを変数で持つ
	SceneType type = kSceneTitle;

	// 最初に実行したいシーンを初期化
	switch (type)
	{
	case kSceneTitle:
		sceneTitle.Init();
		break;
	case kSceneMain:
		sceneMain.Init();
		break;
	case kSceneGameOver:
		sceneGameOver;
		sceneGameOver.Init();
		break;
	case kSceneClear:
		sceneClear;
		sceneClear.Init(sceneMain.GetClearTimeFrame());
		break;
	}

	// メインループ
	while (ProcessMessage() == 0)
	{
		// 現在のフレーム開始時刻を取得
		LONGLONG start = GetNowHiPerformanceCount();

		// 画面をクリア
		ClearDrawScreen();

		switch (type)
		{
		case kSceneTitle:
			sceneTitle.Update();
			sceneTitle.Draw();
			// シーン終了フラグが立っていたらTitleからMainに移行する
			if (sceneTitle.IsEnd())
			{
				// 現在のシーンの終了処理
				sceneTitle.End();

				type = kSceneMain;
				// 次のシーンの初期化を行う
				sceneMain.Init();
			}
			break;
		case kSceneMain:
			sceneMain.Update();
			sceneMain.Draw();
			if (sceneMain.IsEnd())
			{
				// シーンの終了処理
				sceneMain.End();

				if (sceneMain.IsClear())
				{
					type = kSceneClear;	// クリア画面に遷移
					sceneClear.Init(sceneMain.GetClearTimeFrame());
				}
				else
				{
					type = kSceneGameOver;	// ゲームオーバー画面に遷移
					sceneGameOver.Init();
				}
			}
			break;

			// ゲームオーバーシーン
		case kSceneGameOver:
			sceneGameOver.Update();
			sceneGameOver.Draw();
			if (sceneGameOver.IsEnd())
			{
				sceneGameOver.End();
				type = kSceneTitle;
				sceneTitle.Init();
			}
			break;

			// クリアシーン
		case kSceneClear:
			sceneClear.Update();
			sceneClear.Draw();
			if (sceneClear.IsEnd())
			{
				sceneClear.End();
				type = kSceneTitle;
				sceneTitle.Init();
			}
			break;
		}

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

		switch (type)
		{
		case kSceneTitle:
			sceneTitle.End();
			break;
		case kSceneMain:
			sceneMain.End();
			break;
		case kSceneGameOver:
			sceneGameOver.End();
			break;
		case kSceneClear:
			sceneClear.End();
			break;
		}

		DxLib_End();				// ＤＸライブラリ使用の終了処理

		return 0;				// ソフトの終了 
	}