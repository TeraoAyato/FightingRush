#include "SceneTitle.h"
#include "DxLib.h"
#include "Game.h"
#include <cmath>

namespace
{
	// フェードにかかるフレーム数
	constexpr int kFadeFrame = 30;

	// キー入力を受け付けないフレーム数
	constexpr int kKeyInputWaitFrame = 60;
}

SceneTitle::SceneTitle() :
	m_isEnd(false),
	m_frameCount(0),
	m_logoHandle(-1),
	m_TitleBgHandle(-1),
	m_fontHandle(-1),
	m_fadeFrame(0),
	m_fadeSpeed(0),
	m_sinAngle(0.0f),
	m_bgmHandle(-1),
	m_seHandle(-1)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	// グラフィックロード
	m_logoHandle = LoadGraph("sozai/Title/Title.png");
	// タイトル背景ロード
	m_TitleBgHandle = LoadGraph("sozai/Title/TitleBg.png");
	// フォントの生成
	m_fontHandle = CreateFontToHandle("Arial", 50, -1, 1);

	// BGM初期化
	m_bgmHandle = LoadSoundMem("sozai/Sound/SceneTitleBgm.mp3");
	// BGM再生
	ChangeVolumeSoundMem(180, m_bgmHandle);
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);

	// SE初期化
	m_seHandle = LoadSoundMem("sozai/Sound/SceneTitleSe.mp3");


	// フェードの初期化	真っ暗な状態から始まる
	m_fadeFrame = kFadeFrame;
	m_fadeSpeed = -1;	// フェードイン

	m_frameCount = 0;
	m_isEnd = false;
}

void SceneTitle::End()
{
	// グラフィック削除
	DeleteGraph(m_logoHandle);
	// フォントの削除
	DeleteFontToHandle(m_fontHandle);

	// BGMの削除
	if (m_bgmHandle != -1)
	{
		StopSoundMem(m_bgmHandle); // 再生停止
		DeleteSoundMem(m_bgmHandle); // メモリ解放
		m_bgmHandle = -1;
	}
	// SEの削除
	if (m_bgmHandle != -1)
	{
		StopSoundMem(m_seHandle); // 再生停止
		DeleteSoundMem(m_seHandle); // メモリ解放
		m_bgmHandle = -1;
	}
}

void SceneTitle::Update()
{
	m_frameCount++;
	m_sinAngle += 0.05f;

	// フェード処理
	m_fadeFrame += m_fadeSpeed;;
	if (m_fadeFrame < 0)	m_fadeFrame = 0;
	if (m_fadeFrame > kFadeFrame)
	{
		m_fadeFrame = kFadeFrame;
		m_isEnd = true;
	}

	// 一定時間経過しないと入力を受け付けない
	if (m_frameCount >= kKeyInputWaitFrame)
	{
		int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if (pad & PAD_INPUT_1)
		{
			if (m_seHandle != -1)
			{
				PlaySoundMem(m_seHandle, DX_PLAYTYPE_BACK);
			}

			// フェードアウトを開始する
			m_fadeSpeed = +1;

			// フェードアウトが終わったらシーンを終了するよう変更
			// タイトル画面を終了してゲームへ
		//	m_isEnd = true;

			
		}
	}
}

void SceneTitle::Draw()
{
	// 背景色
//	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(255, 250, 205), true);
// 	   // タイトル背景の表示
	DrawExtendGraph(0, 0, 1280, 720, m_TitleBgHandle, TRUE);
	// タイトルロゴの表示
	DrawExtendGraph(250, 100,1050,400, m_logoHandle, TRUE);
	

	// sinカーブを使って透明度を変化させる
	float sinRate = sinf(m_sinAngle);	// -1.0 ~ 1.0
	sinRate /= 2.0f;	// -0.5 ~ 0.5
	sinRate += 0.5f;	//0.0 ~ 1.0
	int alpha = static_cast<int>(255 * sinRate);

	// 半透明で表示を開始
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	if (m_frameCount >= kKeyInputWaitFrame)	// キー入力できない間は表示しない
	{ 
		// ボタンを押してください表示
		int width = GetDrawStringWidthToHandle("PRESS A TO START!", strlen("PRESS A TO START!"), m_fontHandle);
		DrawStringToHandle(Game::kScreenWidth / 2 - width / 2, 550, "PRESS A TO START!", GetColor(255, 40, 0), m_fontHandle);
	}
	// 半透明で表示を終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	

	// 半透明で表示を開始

	// フェード
	// m_fadeFrameの変化に合わせてフェードの透明度を変化させる
	// 0~kFadeFrameまでの間を変化する値を最終的にアルファ値である0~255に変換したい

	// m_fadeFrameをフェードの進行度に変換する
	// 0~kFadeFrameまでの間を変化するが、これを0.0~1.0の割合に変化する
	float frameRate = static_cast<float>(m_fadeFrame) / static_cast<float>(kFadeFrame);	// 浮動小数点数の計算のするためキャスト

	// 0.0~1.0 の割合を 0~255 の間の値に変換する
	alpha = static_cast<int>(255 * frameRate);
	// 半透明で表示を開始
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	// 半透明で表示を終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "SceneTitle", GetColor(0, 255, 0));
	DrawFormatString(0, 16, GetColor(0, 255, 0), "FRAME:%d", m_frameCount);
}
