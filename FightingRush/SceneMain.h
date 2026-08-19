#pragma once
#include"Bg.h"
#include "Player.h"
#include "Enemy.h"

class SceneMain
{
public:
	SceneMain(); // コンストラクタ
	~SceneMain(); // デストラクタ

	void Init();
	void End();

	void Update();
	void Draw();

	bool IsEnd()const { return m_isEnd; }

private:
	int m_frameCount;

	// シーン終了フラグ	trueなら遷移する
	bool m_isEnd;

	Bg m_bg;
	Enemy m_enemy;
	Player m_player;

	// フェード管理
	int m_fadeFrame;
	int m_fadeSpeed;	// -1 or +1

	// 当たり判定
	bool m_OnHit = false;
	bool m_OnEnemyHit = false;

};

