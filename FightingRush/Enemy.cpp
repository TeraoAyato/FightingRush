#include "Enemy.h"
#include "DxLib.h"
#include "Vec2.h"
#include<cmath>

namespace
{
	// エネミー
	constexpr float PosX = 1000.0f;	// 初期座標
	constexpr float PosY = 320.0f;	// 初期座標
	constexpr int EnemyMaxHp = 4;	// 敵最大HP
	constexpr int EnemyHp = 4;		// 敵HP
	constexpr int kEnemyWidth = 96;	// キャラクターの横幅
	constexpr int kEnemyHeight = 63;	// キャラクターの高さ
	constexpr float kEnemySpeed = 2.0f;	// 移動速度

	constexpr float kEnemyOffsetY = 12.0f;// 描画位置

	// 当たり判定の半径
	constexpr float kColRadius = 6.0f;
}

Enemy::Enemy() :
	m_EnemyAnimFrame(0),
	m_EnemyAttackFrame(0),
	m_EnemyAttackCoolTime(0),
	m_isMoving(false),
	m_isAttacking(false),
	m_posX(PosX),	// 初期出現位置
	m_posY(PosY), // 初期出現位置
	m_speed(kEnemySpeed),	// 移動スピード
	m_direction(1),	// 移動方向
	m_isHit(false),
	m_hitFrame(0),
	m_knockbackDir(0.0f),
	m_hp(4),		// 現在HP
	m_maxHp(4),		// 最大HP
	m_isDead(false),
	m_deadFrame(0)
{
	for (int i = 0;i < 4;i++)
	{
		m_EnemyIdleHandle[i] = -1;
		m_EnemyWalkHandle[i] = -1;
		m_DeadHandle[i] = -1;
	}
	for (int i = 0;i < 3;i++)
	{
		m_EnemyPunchHandle[i] = -1;
	}
	for (int i = 0; i < 2;i++)
	{
		m_DamageHitHandle[i] = -1;
	}
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	m_EnemyAnimFrame = 0;
	m_EnemyAttackFrame = 0;
	m_EnemyAttackCoolTime = 0;
	m_isMoving = false;
	m_isAttacking = false;
	m_pos.x = m_posX;	// 初期出現位置座標
	m_pos.y = m_posY;	// 初期出現位置座標
	m_speed = kEnemySpeed;	// 移動スピード
	m_direction = 1;	// 移動方向
	m_isHit = false;
	m_hitFrame = 0;
	m_knockbackDir = 0.0f;
	m_hp = EnemyMaxHp;		// 現在HP
	m_maxHp = EnemyHp;		// 最大HP
	m_isDead = false;
	m_deadFrame = 0;

	LoadDivGraph(
		"data/Enemy/idle.png",	// 待機状態画像
		4, 4, 1,            // 総数4コマ（横4コマ、縦1コマ）
		kEnemyWidth, kEnemyHeight,             //1コマの幅と高さ
		m_EnemyIdleHandle
	);

	LoadDivGraph(
		"data/Enemy/walk.png",	// 歩行状態画像
		4, 4, 1,            // 総数4コマ（横4コマ、縦1コマ）
		kEnemyWidth, kEnemyHeight,             // 1コマの幅と高さ
		m_EnemyWalkHandle
	);

	LoadDivGraph(
		"data/Enemy/punch.png",	// パンチ状態画像
		3, 3, 1,            // 総数4コマ（横4コマ、縦1コマ）
		kEnemyWidth, kEnemyHeight,             // 1コマの幅と高さ
		m_EnemyPunchHandle
	);

	LoadDivGraph(
		"data/Enemy/Dead.png",	// ダメージ状態画像
		2, 2, 1,            // 総数2コマ（横2コマ、縦1コマ）
		kEnemyWidth, kEnemyHeight,             // 1コマの幅と高さ
		m_DamageHitHandle
	);

	LoadDivGraph(
		"data/Enemy/Dead.png",	// ダメージ状態画像
		4, 4, 1,            // 総数4コマ（横4コマ、縦1コマ）
		kEnemyWidth, kEnemyHeight,             // 1コマの幅と高さ
		m_DeadHandle
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
		// 死亡状態の画像が読み込まれていれば削除
		if (m_DeadHandle[i] != -1)
		{
			DeleteGraph(m_DeadHandle[i]);
			m_DeadHandle[i] = -1;
		}
	}
	for (int i = 0;i < 3;i++)
	{
		// パンチアニメーション処理
		if (m_EnemyPunchHandle[i] != -1)
		{
			DeleteGraph(m_EnemyPunchHandle[i]);
			m_EnemyPunchHandle[i] = -1;
		}
	}
	for (int i = 0;i < 2;i++)
	{
		// 被ダメージ処理
		if (m_DamageHitHandle[i] != -1)
		{
			DeleteGraph(m_DamageHitHandle[i]);
			m_DamageHitHandle[i] = -1;
		}
	}
}

void Enemy::Update(float playerX, float playerY)
{
	if (m_isDead)
	{
		m_deadFrame++;
		if (m_deadFrame < 15)
		{
			m_pos.x += m_knockbackDir * 2.0f; // 死亡時のノックバック
		}
		return;
	}


	if (m_isHit)
	{
		m_hitFrame++;

		float knockbackSpeed = 4.0f; // ノックバック速度
		m_pos.x += m_knockbackDir * knockbackSpeed;

		if (m_hitFrame >= 20)
		{
			m_isHit = false;
			m_hitFrame = 0;
		}
		return;
	}

	// 攻撃クールタイム
	if (m_EnemyAttackCoolTime)
	{
		m_EnemyAttackCoolTime--;
	}
	// 左右の間合い（プレイヤーの横どの距離で止まるか）
	constexpr float kStopDistanceX = 40.0f;
	constexpr float kStopDistanceY = 6.0f;

	// プレイヤーとの距離
	float diffX = playerX - m_pos.x;
	float diffY = playerY - m_pos.y;

	// 絶対値で計算
	float absDiffX = std::abs(diffX);
	float absDiffY = std::abs(diffY);

	// 攻撃処理
	if (m_isAttacking)
	{
		m_isMoving = false;
		m_EnemyAttackFrame++;

		if (m_EnemyAttackFrame >= 3 * 10) //　エネミー攻撃アニメーション
		{
			m_isAttacking = false;
			m_EnemyAnimFrame = 0;

			m_EnemyAttackCoolTime = 120;	// パンチ後のクールタイムを設定
		}
		return;
	}

	if (!m_isAttacking && m_EnemyAttackCoolTime <= 0 && absDiffX <= kStopDistanceX + 10.0f && absDiffY <= 20.0f)
	{
		m_isAttacking = true;
		m_EnemyAttackFrame = 0;

		// プレイヤーの方向を向かせる
		if (diffX != 0.0f)
		{
			m_direction = (diffX > 0.0f) ? 1 : -1;
		}
		return;
	}

	m_isMoving = false;
	Vec2 moveDir(0.0f, 0.0f);

	// X軸の判定
	if (absDiffX > kStopDistanceX)
	{
		moveDir.x = (diffX > 0.0f) ? 1.0f : -1.0f;
	}
	// Y軸の判定
	if (absDiffY > kStopDistanceY)
	{
		moveDir.y = (diffY > 0.0f) ? 1.0f : -1.0f;
	}

	// kStopDistanceX以上離れてたらプレイヤーを追従する
	if (moveDir.SqLength() > 0.0f)
	{
		m_isMoving = true;

		// 斜め移動時も一定速度
		moveDir.Normalize();

		bool isDiagonal = (moveDir.x != 0.0f && moveDir.y != 0.0f);
		float speedRate = isDiagonal ? 1.15f : 1.0f;

		m_pos.x += moveDir.x * (m_speed * speedRate);
		m_pos.y += moveDir.y * (m_speed * speedRate);

		// 方向は常にプレイヤーの方向
		if (diffX != 0.0f)
		{
			m_direction = (diffX > 0.0f) ? 1 : -1;
		}
	}
	// アニメーションフレームカウント
	m_EnemyAnimFrame++;
}

void Enemy::Draw()
{

	float Size = 1.5;	// プレイヤーサイズ
	float Angle = 0.0;	// 角度

	if (m_isDead && m_deadFrame > 90)
	{
		return;
	}

	const int* currentHandle = m_isMoving ? m_EnemyWalkHandle : m_EnemyIdleHandle;
	int animIndex = 0;

	if (m_isDead)
	{
		currentHandle = m_DeadHandle;
		animIndex = m_deadFrame / 10;
		if (animIndex >= 4)animIndex = 3; // 死亡アニメーションの最後のフレームでストップ
	}
	else if (m_isHit)
	{
		currentHandle = m_DamageHitHandle;
		animIndex = (m_hitFrame / 10) % 2;
	}
	else if (m_isAttacking)
	{
		currentHandle = m_EnemyPunchHandle;
		animIndex = (m_EnemyAttackFrame / 10) % 3; // パンチアニメーション
	}
	else if (m_isMoving)
	{
		currentHandle = m_EnemyWalkHandle;
		animIndex = (m_EnemyAnimFrame / 15) % 4; // 歩行アニメーション
	}
	else
	{
		currentHandle = m_EnemyIdleHandle;
		animIndex = (m_EnemyAnimFrame / 15) % 4; // 待機アニメーション
	}
	if (currentHandle != nullptr && currentHandle[0] != -1)
	{
		// 敵が右を向く時画像を反転させる
		BOOL isTurn = (m_direction == 1) ? TRUE : FALSE;

		DrawRotaGraph(
			static_cast<int>(m_pos.x),	// 描画位置
			static_cast<int>(m_pos.y + kEnemyOffsetY),	// 描画位置
			Size,	// 拡大率
			Angle,	// 回転角度
			currentHandle[animIndex],	// 画像ハンドル
			TRUE,
			isTurn // 反転フラグ
		);
	}


#ifdef _DEBUG
			// 攻撃HitBox
			float atkX, atkY, atkW, atkH;
			if (GetAttackHitBox(atkX, atkY, atkW, atkH))
			{
				::DrawBox(
					static_cast<int>(atkX), static_cast<int>(atkY),
					static_cast<int>(atkX + atkW), static_cast<int>(atkY + atkH),
					GetColor(255, 0, 0), FALSE
				);
			}
			// 当たり判定（HitBox）のデバッグ枠を描画（緑色）
			float boxX, boxY, boxW, boxH;
			HitBox(boxX, boxY, boxW, boxH);

			::DrawBox(
				static_cast<int>(boxX), static_cast<int>(boxY),
				static_cast<int>(boxX + boxW), static_cast<int>(boxY + boxH),
				GetColor(0, 255, 0), FALSE
			);
#endif
}

bool Enemy::GetAttackHitBox(float& outX, float& outY, float& outW, float& outH) const
{
	if (m_isDead)return false;

	// 攻撃（パンチ）アニメーション中のみ判定を発生させる
	if (!m_isAttacking)return false;
	constexpr int kAttackAnimNum = 3;
		constexpr int kAnimSpeed = 10;
		int totalFrames = kAttackAnimNum * kAnimSpeed;

		const int hitStartFrame = static_cast<int>(totalFrames * 0.4f);
		const int hitEndFrame = static_cast<int>(totalFrames * 0.8f);

		if (m_EnemyAttackFrame < hitStartFrame || m_EnemyAttackFrame > hitEndFrame)
		{
			return false;
		}
	
		float attackWidth = 35.0f; // 敵のパンチの横幅
		float attackHeight = 25.0f; // 敵のパンチの高さ

		// 向き（m_direction: 1が右、-1が左）に合わせて位置を設定
		if (m_direction == 1)
		{
			outX = m_pos.x; // 右側へ発生
		}
		else
		{
			outX = m_pos.x - attackWidth; // 左側へ発生
		}

		outY = (m_pos.y + kEnemyOffsetY) - 10.0f; // 高さ
		outW = attackWidth;
		outH = attackHeight;

		return true;
	

	return false;
}

void Enemy::HitBox(float& outX, float& outY, float& outW, float& outH) const
{
	if (m_isDead)return;

	float Size = 1.5;	// プレイヤーサイズ
	// ダメージ判定のサイズ
	outW = kEnemyWidth * Size * 0.25f;
	outH = kEnemyHeight * Size * 0.7f;

	// 中心座標から左上の座標を計算
	outX = m_pos.x - (outW / 2.0f);
	outY = (m_pos.y + kEnemyOffsetY) - (outH / 2.0f);
}

bool Enemy::OnDamage(float playerX, int damage)
{
	if (m_isHit || m_isDead)return false;

	m_isHit = true;
	m_hitFrame = 0;
	m_isAttacking = false;

	m_hp -= damage;
	if (m_hp <= 0)
	{
		m_hp = 0;
		m_isDead = true;	// HPが0で死亡
		m_deadFrame = 0;	// 死亡アニメーション開始
	}
	if (playerX < m_pos.x)
	{
		m_knockbackDir = 0.4f; // プレイヤーが左にいる場合、右方向にノックバック
	}
	else
	{
		m_knockbackDir = -0.4f; // プレイヤーが右にいる場合、左方向にノックバック
	}
	return true;
}

void Enemy::SetIdle()
{
	m_isMoving = false;       // 移動停止
	m_isAttacking = false;    // 攻撃動作キャンセルの停止
	m_EnemyAttackFrame = 0;   // 攻撃フレーム初期化
	m_EnemyAnimFrame++;       // 待機アニメーションのコマだけは進める
}

void Enemy::SetPosition(float x, float y)
{
	m_posX = x;	// 出現位置
	m_posY = y;
	m_pos.x = x;	// 現在位置
	m_pos.y = y;	// 現在位置
}