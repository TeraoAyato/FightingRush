#pragma once
#include<vector>
#include<memory>
#include"Enemy.h"
class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	void Init();
	void End();
	void Update();
	void Draw();

	const std::vector<std::unique_ptr<Enemy>>& GetEnemies()const { return m_enemies; }

	bool IsAllDefeated()const { return(m_spawnedCount >= kMaxSpawnTotalEnemy) && m_enemies.empty(); }

private:
	static constexpr int kMaxScreenEnemy = 3;	// 1画面内に存在する敵の最大数
	static constexpr int kMaxSpawnTotalEnemy = 15;// ゲーム全体で出現する敵の合計

	std::vector<std::unique_ptr<Enemy>>m_enemies;
	int m_spawnedCount;


};

