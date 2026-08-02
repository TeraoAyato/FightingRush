#include "Enemy.h"
#include "DxLib.h"
#include<cmath>

namespace
{
	// エネミー
	constexpr int kEnemyWidth = 96;	// キャラクターの横幅
	constexpr int kEnemyHeight = 63;	// キャラクターの高さ
	constexpr int kEnemySpeed = 2.0f;	// 移動速度
	constexpr int kEnemySize = 1.0;	// キャラクターの大きさ
	constexpr int kEnemyAngle = 0.0;	// キャラクターの角度


}

Enemy::Enemy() :
	m_EnemyAnimFrame(0),
	m_isMoving(false),
	m_posX(400.0f),	// 初期出現位置
	m_posY(300.0f),
	m_speed(kEnemySpeed),	// 移動スピード
	m_direction(1)	// 移動方向
{
	for (int i = 0;i < 4;i++)
	{
		m_EnemyIdleHandle[i] = -1;
		m_EnemyWalkHandle[i] = -1;
	}
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	LoadDivGraph(
		"sozai/Enemy/idle.png",
		4, 4, 1,            // 総数4コマ（横4コマ、縦1コマ）
		kEnemyWidth, kEnemyHeight,             // ★ここ！1コマの幅と高さ（素材に合わせて変更）
		m_EnemyIdleHandle
	);

	LoadDivGraph(
		"sozai/Enemy/walk.png",
		4, 4, 1,            // 総数4コマ（横4コマ、縦1コマ）
		kEnemyWidth, kEnemyHeight,             // ★ここ！1コマの幅と高さ（素材に合わせて変更）
		m_EnemyWalkHandle
	);
}

void Enemy::End()
{
	for (int i = 0; i < 4; i++)
	{
		if (m_EnemyIdleHandle[i] != -1)
		{
			DeleteGraph(m_EnemyIdleHandle[i]);
			m_EnemyIdleHandle[i] = -1;
		}
		if (m_EnemyWalkHandle[i] != -1)
		{
			DeleteGraph(m_EnemyWalkHandle[i]);
			m_EnemyWalkHandle[i] = -1;
		}

	}
}

void Enemy::Update(float playerX, float playerY)
{
	// 左右の間合い（プレイヤーの横何ピクセルで止まるか）
	constexpr float kStopDistanceX = 40.0f;

	// プレイヤーとの距離
	float diffX = playerX - m_posX;
	float diffY = playerY - m_posY;

	float absDiffX = std::abs(diffX);
	float absDiffY = std::abs(diffY);

	bool isMovingX = false;
	bool isMovingY = false;


	if (absDiffX > kStopDistanceX)
	{
		isMovingX = true;

		if (diffX > 0.0f)
		{
			// プレイヤーが右にいる ➔ 右に移動
			m_direction = 1;
			m_posX += m_speed;

			if (m_posX > playerX - kStopDistanceX)
			{
				m_posX = playerX - kStopDistanceX;
			}
		}
		else
		{
			// プレイヤーが左にいる ➔ 左に移動
			m_direction = -1;
			m_posX -= m_speed;

			if (m_posX < playerX + kStopDistanceX)
			{
				m_posX = playerX + kStopDistanceX;
			}
		}
	}

	// Y軸の間合いは「移動速度（m_speed）」程度にして、できるだけ同じ高さに合わせる
	if (absDiffY > m_speed)
	{
		isMovingY = true;

		if (diffY > 0.0f)
		{
			// プレイヤーが下にいる ➔ 下へ移動
			m_posY += m_speed;
		}
		else
		{
			// プレイヤーが上にいる ➔ 上へ移動
			m_posY -= m_speed;
		}
	}
	else
	{
		// 差がわずか（m_speed以下）なら、プレイヤーとピッタリ同じ高さ（Y座標）にする！
		m_posY = playerY;
	}

	// 動いているか
	m_isMoving = (isMovingX || isMovingY);

	// 向きは常にプレイヤーのいる方向を向く
	if (diffX != 0.0f)
	{
		m_direction = (diffX > 0.0f) ? 1 : -1;
	}

	m_EnemyAnimFrame++;
}

void Enemy::Draw()
{
	int animIndex = (m_EnemyAnimFrame / 15) % 4; // 15フレームごとに次のコマに切り替え

	const int* currentHandle = m_isMoving ? m_EnemyWalkHandle : m_EnemyIdleHandle;

	if (currentHandle[0] != -1)
	{
		// 敵が右を向く時画像を反転させる
		BOOL isTurn = (m_direction == 1) ? TRUE : FALSE;

		DrawRotaGraph(
			static_cast<int>(m_posX),
			static_cast<int>(m_posY),
			kEnemySize,
			kEnemyAngle,
			currentHandle[animIndex],
			TRUE,
			isTurn // 反転フラグ
		);

		// デバッグ
		DrawCircle(static_cast<int>(m_posX), static_cast<int>(m_posY), 5, GetColor(255, 0, 0), TRUE);
	}
}
