#include "Effect.h"
#include "DxLib.h"

namespace
{
	constexpr int kWidth = 64;
	constexpr int kHeight = 72;
	constexpr int kAnimeSpeed = 4; // 1コマあたりの表示フレーム数
}

Effect::Effect() :
	m_x(0.0f),
	m_y(0.0f),
	m_frame(0),
	m_isPlaying(false)
{
	for (int i = 0; i < kMaxFrame; i++)
	{
		m_atkHitHandle[i] = -1;
	}
}

Effect::~Effect()
{
}

void Effect::Init()
{
	m_isPlaying = false;
	m_frame = 0;

	// 画像の分割読み込み

		LoadDivGraph(
		"sozai/Effect/63.png",	// ファイル名
		kMaxFrame,
		kMaxFrame,
		1,
		kWidth,
		kHeight,
		m_atkHitHandle
	);
}

void Effect::End()
{
	for (int i = 0; i < kMaxFrame; i++)
	{
		if (m_atkHitHandle[i] != -1)
		{
			DeleteGraph(m_atkHitHandle[i]);
			m_atkHitHandle[i] = -1;
		}
	}
}



void Effect::Update()
{
	if (!m_isPlaying) return;

	m_frame++;

	// 7コマ × 4フレーム = 28フレームで再生終了
	if (m_frame >= kMaxFrame * kAnimeSpeed)
	{
		m_isPlaying = false;
	}
}

void Effect::Draw()
{
	if (!m_isPlaying) return;

	int animeIndex = m_frame / kAnimeSpeed;

	if (animeIndex >= 0 && animeIndex < kMaxFrame)
	{
		// 中心に合わせて描画
		DrawGraph(
			static_cast<int>(m_x - kWidth / 2),
			static_cast<int>(m_y - kHeight / 2),
			m_atkHitHandle[animeIndex],
			TRUE
		);
	}
}

// エフェクト再生
void Effect::Play(float x, float y)
{
	if (m_isPlaying) return;

	m_x = x;
	m_y = y;
	m_frame = 0;
	m_isPlaying = true; // 再生開始
}