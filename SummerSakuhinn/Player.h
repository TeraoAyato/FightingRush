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

private:
	// 座標
	int m_posX;
	int m_posY;
	// 移動方向、向き
	bool m_isMoving;
	bool m_isDirRight;

	// 攻撃用の変数
	bool m_isAttacking;	// 攻撃中か判定
	bool m_isAttackInput;	// 攻撃ボタン長押し防止
	bool m_ComboInput;	// 攻撃の連打入力確認
	int m_attackFrame;	// 攻撃が始まってからのフレーム数
	int m_attackType;	// 攻撃の種類（1 or 2)

	// ボタン入力タイマー
	
	// アニメーション用フレームカウント
	int m_frameCount;

	// アニメーション画像ハンドル
	int m_idleHandle[7];	// 待機画像
	int m_runHandle[10];	// 走り画像
	int m_attack1Handle[6];	// 攻撃1画像
	int m_attack2Handle[4];	// 攻撃2画像
	int m_attack3Handle[6];	// 攻撃3画像
	
};

