#pragma once
#include"Bg.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Effect.h"

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
	bool IsClear()const { return m_isClear; }

private:
	int m_frameCount;

	// シーン終了フラグ	trueなら遷移する
	bool m_isEnd;

	bool m_isClear;

	Bg m_bg;
	Player m_player;
	EnemyManager m_enemyManager;

	// フェード管理
	int m_fadeFrame;
	int m_fadeSpeed;	// -1 or +1

	// 当たり判定
	bool m_OnHit = false;
	bool m_OnEnemyHit = false;

	// エフェクト
	Effect m_hitEffect;

	// サウンド
	int m_PlayerhitSoundHandle;
	int m_EnemyhitSoundHandle;

	int m_bgmHandle;
	int m_PlayerDieSeHandle;

};

