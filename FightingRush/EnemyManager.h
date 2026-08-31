#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"

class EnemyManager
{
public:

    EnemyManager();
    ~EnemyManager();

    void Init();
    void End();
    void Update(float playerX, float playerY);
    void Draw();

    // 敵をすべて倒したか？（クリア判定用）
    bool IsAllEnemyDie() const { return m_killCount >= kMaxTotalEnemies; }

    // 当たり判定用に敵のリストを取得する関数
    std::vector<std::shared_ptr<Enemy>>& GetEnemies() { return m_enemies; }

    int GetSpawnCount() const { return m_killCount; }
    int GetTotalCount() const { return kMaxTotalEnemies; }

private:
    void SpawnEnemy(); // 敵を1体生成する内部関数

private:
    // 画面上に存在する敵のリスト
    std::vector<std::shared_ptr<Enemy>> m_enemies;

    static constexpr int kMaxOnScreen = 3;   // 画面上に同時に存在できる敵の最大数
    static constexpr int kMaxTotalEnemies = 18; // ゲーム全体で出現する敵の総数

    int m_spawnCount; // これまでに生成した敵の数
    int m_killCount;  // これまでに倒した敵の数
};