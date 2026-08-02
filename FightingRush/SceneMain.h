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

private:
	int m_frameCount;

//	Bg m_bg;
	Enemy m_enemy;
	Player m_player;
};

