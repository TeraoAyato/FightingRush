#pragma once
#include "DxLib.h"
#include "Vec2.h"
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
	void OnDamage(float playerX,int damage = 1);
	void SetPosition(float x, float y);
	float GetPosX() const { return m_pos.x; } // 必要に応じて現在座標を返す
	float GetPosY() const { return m_pos.y; }

	void SetIdle();
private:
	int m_EnemyIdleHandle[4];	// 敵待機状態ハンドル
	int m_EnemyWalkHandle[4];	// 敵歩行ハンドル
	int m_EnemyPunchHandle[3];	// 敵攻撃ハンドル
	int m_DamageHitHandle[2];	// ダメージ判定画像
	int m_DeadHandle[4];	// 死亡画像


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

	// HP管理用
	int m_hp;	// 敵のHP
	int m_maxHp;	// 敵の最大HP
	int m_isDead;	// 敵が死亡したか判定
	int m_deadFrame;	// 死亡フレーム数
	
	Vec2 m_pos;

public:

	// Enemy.h の public: 内に追加
// 死亡アニメーション（90フレーム等）が終わったら true
	bool IsDestroyed() const { return m_isDead && m_deadFrame > 90; }
	int GetHp() const { return m_hp; }
	bool IsDead() const { return m_isDead; }

};

