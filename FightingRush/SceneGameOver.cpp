#include "SceneGameOver.h"
#include "DxLib.h"
#include "Game.h"
#include <cmath>

namespace
{
	// フェードにかかるフレーム数
	constexpr int kFadeFrame = 30;

	// キー入力を受け付けないフレーム数
	constexpr int kKeyInputWaitFrame = 60;

	// BGMの音量
//	constexpr int kBgmVolume = 128;
}

SceneGameOver::SceneGameOver() :
	m_isEnd(false),
	m_frameCount(0),
	m_BgHandle(-1),
	m_GameOverFontHandle(-1),
	m_fontHandle(-1),
	m_fadeFrame(0),
	m_fadeSpeed(0),
	m_sinAngle(0.0f),
	m_bgmHandle(-1),
	m_seHandle(-1)
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
	// 背景ロード
	m_BgHandle = LoadGraph("sozai/Result/GameOverBg.png");
	// ゲームオーバー文字
	m_GameOverFontHandle = LoadGraph("sozai/Result/GameOverFont.png");
	// フォントの生成
	m_fontHandle = CreateFontToHandle("Noto Sans JP Black", 50, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4, -1, 3);

	// BGM読み込み
	m_bgmHandle = LoadSoundMem("sozai/Sound/SceneGameOverBgm.mp3");
	ChangeVolumeSoundMem(220, m_bgmHandle);
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);
	// SE読み込み
	m_seHandle = LoadSoundMem("sozai/Sound/SceneGameOverSe.mp3");

	// フェードの初期化	真っ暗な状態から始まる
	m_fadeFrame = kFadeFrame;
	m_fadeSpeed = -1;	// フェードイン

	m_frameCount = 0;
	m_isEnd = false;
}

void SceneGameOver::End()
{
	DeleteGraph(m_BgHandle);
	DeleteGraph(m_GameOverFontHandle);
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
	if (m_seHandle != -1)
	{
		StopSoundMem(m_seHandle); // 再生停止
		DeleteSoundMem(m_seHandle); // メモリ解放
		m_bgmHandle = -1;
	}
}

void SceneGameOver::Update()
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
		// Bボタン
		int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if (pad & PAD_INPUT_2)
		{

			if (m_seHandle != -1)
			{
				ChangeVolumeSoundMem(220, m_seHandle);
				PlaySoundMem(m_seHandle, DX_PLAYTYPE_BACK);
			}

			// フェードアウトを開始する
			m_fadeSpeed = +1;
		}
	}
}

void SceneGameOver::Draw()
{
	// 背景の表示 DrawExtendGraph(左上,上,右上,下)
	DrawExtendGraph(0, 0, 1280, 720, m_BgHandle, TRUE);

	DrawExtendGraph(200, 50, 1100, 400, m_GameOverFontHandle, TRUE);

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
		int width = GetDrawStringWidthToHandle("PRESS B TO TITLE", strlen("PRESS B TO TITLE"), m_fontHandle);
		DrawStringToHandle(Game::kScreenWidth / 2 - width / 2, 550, "PRESS B TO TITLE", GetColor(255, 40, 0), m_fontHandle);
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

#ifdef _DEBUG
	DrawString(0, 0, "SceneGameOver", GetColor(0, 255, 0));
	DrawFormatString(0, 16, GetColor(0, 255, 0), "FRAME:%d", m_frameCount);
#endif
}
