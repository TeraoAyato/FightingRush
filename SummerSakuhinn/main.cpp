#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"

namespace
{
	constexpr int kDefaultPosX = 200;	// 初期X座標
	constexpr int kDefaultPosY = 200;	// 初期Y座標
	constexpr int kMoveSpeed = 6;	// 移動速度
	constexpr int kWidth = 128;	// キャラクターの横幅
	constexpr int kHeight = 128;	// キャラクターの縦幅

	// プレイヤー空白サイズ
	// 余白調整
	constexpr int kMarginLeft = 40;
	constexpr int kMarginRight = 50;
	constexpr int kMarginTop = 50;
	constexpr int kMarginBottom = 0;
}


// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// windowモードで起動する
	ChangeWindowMode(1);

	// ウィンドウサイズを16:9に
	SetGraphMode(Game::kScreenWidth,Game::kScreenHeight,Game::kColorDepth);

	//　ウィンドウのタイトル表示を変更
	SetMainWindowText("DxLibTemplate");

	// 背景色
	SetBackgroundColor(0, 0, 0);

	// アニメーション管理用の変数
	int frameCount = 0;	// 全体のフレーム数を数える

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	int idleHandle[7];	// 画像ハンドルの配列
	LoadDivGraph(
		"sozai/Player/Idle.png",	// ファイル名
		7,	// 総コマ数
		7,	// 横コマ数
		1,	// 縦コマ数
		kWidth,	// 1コマの幅
		kHeight,	// １コマの高さ
		idleHandle	// 保存配列
	);

		// 走り画像
	int runHandle[10];	// 画像ハンドルの配列
	LoadDivGraph(
		"sozai/Player/run.png",	// ファイル名
		10,	// 総コマ数
		10,	// 横コマ数
		1,	// 縦コマ数
		kWidth,	// 1コマの幅
		kHeight,	// １コマの高さ
		runHandle	// 保存配列
	);

	int posX = kDefaultPosX;
	int posY = kDefaultPosY;

	// キャラクターの向き
	bool isDirRight = true;

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

		// 1フレームごとにカウントアップ
		frameCount++;

		// コントローラー入力
		int padState = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// 移動中か判定
		bool isMoving = false;
		// 入力に合わせて向きを変える
		if (padState & PAD_INPUT_LEFT)
		{
			// 左入力、向き
			posX -= kMoveSpeed;
			isMoving = true;
			isDirRight = false;
		}	
		if (padState & PAD_INPUT_RIGHT)
		{ 
			// 右入力、向き
			posX += kMoveSpeed;
			isMoving = true;
			isDirRight = true;
		}	
		if (padState & PAD_INPUT_UP) { posY -= kMoveSpeed;isMoving = true;}	// 上入力
		if (padState & PAD_INPUT_DOWN) { posY += kMoveSpeed;isMoving = true;}	// 下入力

		// 画面外に出ない処理
		// 左端制限
		if (posX + kMarginLeft< 0)
		{
			posX = -kMarginLeft;
		}

		// 右端制限
		if (posX + kWidth - kMarginRight > Game::kScreenWidth)
		{
			posX = Game::kScreenWidth - kWidth + kMarginRight;
		}

		// 上端制限
		if (posY + kMarginTop < 0)
		{
			posY = -kMarginTop;
		}

		// 下端制限
		if (posY + kHeight - kMarginBottom> Game::kScreenHeight)
		{
			posY = Game::kScreenHeight - kHeight + kMarginBottom;
		}

		// ゲームの処理
		scene.Update();

		scene.Draw();

		int centerX = posX + kWidth / 2;
		int centerY = posY + kHeight / 2;
		
		int turnFlag = isDirRight ? FALSE : TRUE;
		// 描画処理
		// 状態に応じたアニメーション処理
		if (isMoving)
		{
			// 【移動中】 Run画像アニメーション
			int animIndex = (frameCount / 5) % 10;

			DrawRotaGraph(centerX, centerY,1.0,0.0,runHandle[animIndex],TRUE,turnFlag);
		}
		else
		{
			// 【待機中】 Idle画像アニメーション
			int animIndex = (frameCount / 10) % 7;
			DrawRotaGraph(centerX, centerY,1.0,0.0,idleHandle[animIndex],TRUE,turnFlag);
		}

		
		DrawFormatString(0,40, GetColor(255, 255, 255), "X:%d",posX);
		DrawFormatString(0,60, GetColor(255, 255, 255), "Y:%d",posY);
		DrawFormatString(0, 80, GetColor(255, 255, 255), "PAD:%d", padState);
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