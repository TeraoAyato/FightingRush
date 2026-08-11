#pragma once
class Enemy
{
public:
	Enemy();
	~Enemy();

	void Init();
	void End();
	void Update(float playerX, float playerY);
	void Draw();

	bool GetAttackHitBox(float& outX, float& outY, float& outW, float& outH)const;
	void HitBox(float& outX,float& outY,float& outW,float& outH)const;
	void OnDamage(float playerX);

private:
	int m_EnemyIdleHandle[4];	// 敵待機状態ハンドル
	int m_EnemyWalkHandle[4];	// 敵歩行ハンドル
	int m_EnemyPunchHandle[3];	// 敵攻撃ハンドル
	int m_DamageHitHandle[2];	// ダメージ判定画像


	int m_EnemyAnimFrame;
	int m_EnemyAttackFrame;	// 攻撃中のアニメーションフレーム数
	int m_EnemyAttackCoolTime;	// 攻撃後のクールタイム
	bool m_isMoving;	// 移動中かどうか
	bool m_isAttacking;	// 攻撃中かどうか
	float m_posX;	// X座標
	float m_posY;	// Y座標
	float m_speed;	// 移動速度
	int m_direction;	// 移動方向
	
	// ダメージ状態用
	bool m_isHit;	// ダメージ状態か判定
	int m_hitFrame;	// ダメージフレーム数
	float m_knockbackDir;	// ノックバック距離
};

