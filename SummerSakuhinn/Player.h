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

	// アニメーション用フレームカウント
	int m_frameCount;

	// アニメーション画像ハンドル
	int m_idleHandle[7];	// 待機画像
	int m_runHandle[10];	// 走り画像
};

