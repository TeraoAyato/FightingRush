#pragma once
#include "Vec2.h"
#include "DxLib.h"

class Player
{
public:
	Player();
	~Player();

	void Init();
	void End();
	void Update();
	void Draw();

	float GetPosX()const { return m_posX; }
	float GetPosY() const { return m_posY; }

	// プレイヤー中心座標
	float GetCenterX() const { return static_cast<float>(m_posX + 64); }
	float GetCenterY() const { return static_cast<float>(m_posY + 90); }

	bool GetAttackHitBox(float& outX, float& outY, float& outW, float& outH)const;
	void HitBox(float& outX, float& outY, float& outW, float& outH)const;

	// HPの取得・設定用関数
	int GetHp() const { return m_hp; }
	int GetMaxHp() const { return m_maxHp; }
	bool IsDead() const { return m_isDead; }

	// 被ダメージ開始を知らせる関数
	void OnDamage(int damage = 1);

	// ダメージ中かどうかを外部から取得する関数
	bool IsHit() const { return m_isHit; }

	int GetAttackPower()const;

private:

	// アニメーション画像ハンドル
	int m_idleHandle[7];	// 待機画像
	int m_runHandle[10];	// 走り画像
	int m_attack1Handle[6];	// 攻撃1画像
	int m_attack2Handle[4];	// 攻撃2画像
	int m_attack3Handle[6];	// 攻撃3画像
	int m_DamageHitHandle[4];	// ダメージ判定画像
	int m_DeadHandle[5];	// 死亡画像

	// 座標
	int m_posX;
	int m_posY;

	// 移動方向、向き
	bool m_isMoving;
	bool m_isDirRight;

	// HP用
	int m_hp;	// 現在のHP
	int m_maxHp;	// 最大HP
	int m_isDead;	// 死亡判定

	// 攻撃用の変数
	bool m_isAttacking;	// 攻撃中か判定
	bool m_isAttackInput;	// 攻撃ボタン長押し防止
	bool m_ComboInput;	// 攻撃の連打入力確認
	int m_attackFrame;	// 攻撃が始まってからのフレーム数
	int m_attackType;	// 攻撃の種類（1 or 2)

	// ダメージ状態用
	bool m_isHit;	// ダメージ状態か判定
	int m_hitFrame;	// ダメージフレーム数

	// 死亡用
	int m_deadFrame;	// 死亡フレーム数


	// ボタン入力タイマー
	int m_ComboInputTime;	// 入力猶予タイマー
	int m_nextAttackType;	// 次に出す攻撃タイプ

	// アニメーション用フレームカウント
	int m_frameCount;
};

