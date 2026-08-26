#include "EnemyManager.h"
#include "Enemy.h"

EnemyManager::EnemyManager():
m_spawnCount(0),
m_killCount(0)
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init()
{
    // 敵リセット
    m_spawnCount = 0;
    m_killCount = 0;

    m_enemies.clear();


    // 初期化時に3体スポーン
    for (int i = 0; i < kMaxOnScreen; ++i)
    {
        SpawnEnemy();
    }
}

void EnemyManager::End()
{
}



void EnemyManager::Update(float playerX, float playerY)
{
    // 画面内の敵が3体未満、敵の登場人数が15体未満だったら追加スポーン
    while (m_enemies.size() < kMaxOnScreen && m_spawnCount < kMaxTotalEnemies)
    {
        SpawnEnemy();
    }

    // エネミーの死亡チェック
    for (auto it = m_enemies.begin(); it != m_enemies.end(); )
    {
        (*it)->Update(playerX, playerY);

        // 死亡アニメーションが終了したら消す
        if ((*it)->IsDestroyed())
        {
            m_killCount++;
            it = m_enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void EnemyManager::Draw()
{
    for (auto& enemy : m_enemies)
    {
        enemy->Draw();
    }
}

void EnemyManager::SpawnEnemy()
{
    if (m_spawnCount >= kMaxTotalEnemies) return;

    auto newEnemy = std::make_shared<Enemy>();
    newEnemy->Init();

    // 出現位置
    float spawnX = 0.0f;
    float spawnY = 260.0f + (m_spawnCount % 3) * 20.0f; 

    // 出現時の敵同士の距離
    float distanceOffset = (m_spawnCount % 3) * 120.0f + (rand() % 40);

    // ランダムでスポーン（0: 左から / 1: 右から）
    bool isLeft = (rand() % 2 == 0);

    if (isLeft)
    {
        // 画面左から出現
        spawnX = -20.0f - distanceOffset;
    }
    else
    {
        // 画面右から出現
        spawnX = 1320.0f + distanceOffset;
    }



    newEnemy->SetPosition(spawnX, spawnY);

    m_enemies.push_back(newEnemy);
    m_spawnCount++;
}