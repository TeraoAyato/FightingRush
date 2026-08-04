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

private:
	int m_EnemyIdleHandle[4];	// 画像ハンドル
	int m_EnemyWalkHandle[4];	// 画像ハンドル
	int m_EnemyPunchHandle[3];	// 画像ハンドル


	int m_EnemyAnimFrame;
	int m_EnemyAttackFrame;	// 攻撃中のアニメーションフレーム数
	int m_EnemyAttackCoolTime;	// 攻撃後のクールタイム
	bool m_isMoving;	// 移動中かどうか
	bool m_isAttacking;	// 攻撃中かどうか
	float m_posX;	// X座標
	float m_posY;	// Y座標
	float m_speed;	// 移動速度
	int m_direction;	// 移動方向
};

