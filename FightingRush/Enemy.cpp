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
	m_EnemyAttackFrame(0),
	m_isMoving(false),
	m_isAttacking(false),
	m_posX(1000.0f),	// 初期出現位置
	m_posY(320.0f), // 初期出現位置
	m_speed(kEnemySpeed),	// 移動スピード
	m_direction(1)	// 移動方向
{
	for (int i = 0;i < 4;i++)
	{
		m_EnemyIdleHandle[i] = -1;
		m_EnemyWalkHandle[i] = -1;
	}
	for (int i = 0;i < 3;i++)
	{
		m_EnemyPunchHandle[i] = -1;
	}
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	LoadDivGraph(
		"sozai/Enemy/idle.png",	// 待機状態画像
		4, 4, 1,            // 総数4コマ（横4コマ、縦1コマ）
		kEnemyWidth, kEnemyHeight,             //1コマの幅と高さ
		m_EnemyIdleHandle
	);

	LoadDivGraph(
		"sozai/Enemy/walk.png",	// 歩行状態画像
		4, 4, 1,            // 総数4コマ（横4コマ、縦1コマ）
		kEnemyWidth, kEnemyHeight,             // 1コマの幅と高さ
		m_EnemyWalkHandle
	);

	LoadDivGraph(
		"sozai/Enemy/punch.png",	// パンチ状態画像
		3, 3, 1,            // 総数4コマ（横4コマ、縦1コマ）
		kEnemyWidth, kEnemyHeight,             // 1コマの幅と高さ
		m_EnemyPunchHandle
	);
}

void Enemy::End()
{
	for (int i = 0; i < 4; i++)
	{
		// 待機状態の画像が読み込まれていれば削除
		if (m_EnemyIdleHandle[i] != -1)
		{
			DeleteGraph(m_EnemyIdleHandle[i]);
			m_EnemyIdleHandle[i] = -1;
		}
		// 歩行状態の画像が読み込まれていれば削除
		if (m_EnemyWalkHandle[i] != -1)
		{
			DeleteGraph(m_EnemyWalkHandle[i]);
			m_EnemyWalkHandle[i] = -1;
		}
	}
	for (int i = 0;i < 3;i++)
	{
		if(m_EnemyPunchHandle[i] != -1)
		{
		DeleteGraph(m_EnemyPunchHandle[i]);
		m_EnemyPunchHandle[i] = -1;
		}
	}
}

void Enemy::Update(float playerX, float playerY)
{
	// 攻撃クールタイム
	if (m_EnemyAttackCoolTime)
	{
		m_EnemyAttackCoolTime--;
	}
	// 左右の間合い（プレイヤーの横どの距離で止まるか）
	constexpr float kStopDistanceX = 40.0f;

	// プレイヤーとの距離
	float diffX = playerX - m_posX;
	float diffY = playerY - m_posY;

	// 絶対値で計算
	float absDiffX = std::abs(diffX);
	float absDiffY = std::abs(diffY);


	if (m_isAttacking)
	{
		m_isMoving = false;
		m_EnemyAttackFrame++;

		if (m_EnemyAttackFrame >= 3 * 10)
		{
			m_isAttacking = false;
			m_EnemyAnimFrame = 0;

			m_EnemyAttackCoolTime = 60;	// パンチ後のクールタイムを設定
		}
		return;
	}

	if (!m_isAttacking && m_EnemyAttackCoolTime <= 0 && absDiffX <= kStopDistanceX + 10.0f && absDiffY <= 20.0f)
	{
		m_isAttacking = true;
		m_EnemyAttackFrame = 0;

		// プレイヤーの方向を向かせる
		if(diffX != 0.0f)
		{
			m_direction = (diffX > 0.0f) ? 1 : -1;
		}
		return;
	}

	bool isMovingX = false;
	bool isMovingY = false;

	// kStopDistanceX以上離れてたらプレイヤーを追従する
	if (absDiffX > kStopDistanceX)
	{
		isMovingX = true;

		if (diffX > 0.0f)
		{
			// プレイヤーが右にいると右に移動
			m_direction = 1;
			m_posX += m_speed;

			if (m_posX > playerX - kStopDistanceX)
			{
				m_posX = playerX - kStopDistanceX;
			}
		}
		else
		{
			// プレイヤーが左にいると左に移動
			m_direction = -1;
			m_posX -= m_speed;

			if (m_posX < playerX + kStopDistanceX)
			{
				m_posX = playerX + kStopDistanceX;
			}
		}
	}

	// 絶対値の差がm_speedより大きければプレイヤー追従
	if (absDiffY > m_speed)
	{
		isMovingY = true;

		if (diffY > 0.0f)
		{
			// プレイヤーが下にいると下へ移動
			m_posY += m_speed;
		}
		else
		{
			// プレイヤーが上にいると上へ移動
			m_posY -= m_speed;
		}
	}
	else
	{
		// プレイヤーと距離が近かったら、プレイヤーのY軸座標に合わせる
		m_posY = playerY;
	}

	// 動いているか
	m_isMoving = (isMovingX || isMovingY);

	// 向きは常にプレイヤーのいる方向を向く
	if (diffX != 0.0f)
	{
		m_direction = (diffX > 0.0f) ? 1 : -1;	// 右にいるなら1、左にいるなら-1
	}
	// アニメーションフレームカウント
	m_EnemyAnimFrame++;
}

void Enemy::Draw()
{
//	int animIndex = (m_EnemyAnimFrame / 15) % 4; // 15フレームごとに次のコマに切り替え

	const int* currentHandle = m_isMoving ? m_EnemyWalkHandle : m_EnemyIdleHandle ;
	int animIndex = 0;

	if (m_isAttacking)
	{
		currentHandle = m_EnemyPunchHandle;
		animIndex = (m_EnemyAttackFrame / 10) % 3; // パンチアニメーション
	}
	else if (m_isMoving)
	{
		animIndex = (m_EnemyAnimFrame / 15) % 4; // 歩行アニメーション
	}
	else
	{
		animIndex = (m_EnemyAnimFrame / 15) % 4; // 待機アニメーション
	}
	if (currentHandle != nullptr && currentHandle[0] != -1)
	{
		// 敵が右を向く時画像を反転させる
		BOOL isTurn = (m_direction == 1) ? TRUE : FALSE;

		DrawRotaGraph(
			static_cast<int>(m_posX),	// 描画位置
			static_cast<int>(m_posY),	// 描画位置
			kEnemySize,	// 拡大率
			kEnemyAngle,	// 回転角度
			currentHandle[animIndex],	// 画像ハンドル
			TRUE,
			isTurn // 反転フラグ
		);

		

		// デバッグ
#ifdef _DEBUG
		DrawCircle(static_cast<int>(m_posX), static_cast<int>(m_posY), 5, GetColor(255, 0, 0), TRUE);
#endif
	}
}
