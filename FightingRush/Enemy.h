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


	int m_EnemyAnimFrame;
	bool m_isMoving;	// 移動中かどうか

	float m_posX;	// X座標
	float m_posY;	// Y座標
	float m_speed;	// 移動速度
	int m_direction;	// 移動方向
};

