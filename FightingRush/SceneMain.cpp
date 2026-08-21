#include "SceneMain.h"
#include "DxLib.h"
#include "Bg.h"
#include "Enemy.h"
#include "SceneTitle.h"
#include "Game.h"
#include "EnemyManager.h"

namespace
{
	// 2つの矩形が重なっているかチェック
	bool CheckAABB(float x1, float y1, float w1, float h1,
		float x2, float y2, float w2, float h2)
	{
		if (x1 < x2 + w2 &&
			x1 + w1 > x2 &&
			y1 < y2 + h2 &&
			y1 + h1 > y2)
		{
			return true; // 重なっている
		}
		return false; // 重なっていない
	}

	// フェードにかかるフレーム数
	constexpr int kFadeFrame = 30;

	// キー入力を受け付けないフレーム数
	constexpr int kKeyInputWaitFrame = 60;

	// BGMの音量
//	constexpr int kBgmVolume = 128;
}

SceneMain::SceneMain() :
	m_frameCount(0),
	m_isEnd(false),
	m_fadeFrame(0),
	m_fadeSpeed(0)
{
	
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// 初期化時終了フラグ
	m_isEnd = false;
	m_frameCount = 0;

	m_fadeFrame = kFadeFrame;
	m_fadeSpeed = -1;

	m_bg.Init();
	
	m_player.Init();
	m_enemyManager.Init();
	
}

void SceneMain::End()
{
	m_bg.End();
	
	m_player.End();
	m_enemyManager.End();
}

void SceneMain::Update()
{
	// プレイヤー座標取得
	float playerX = m_player.GetCenterX();
	float playerY = m_player.GetCenterY();

	// エネミー全体の更新（中で移動や生成が行われる）
	m_enemyManager.Update(playerX, playerY);

	// フェード処理
	m_fadeFrame += m_fadeSpeed;
	if (m_fadeFrame < 0)
	{
		m_fadeFrame = 0;
		m_fadeSpeed = 0;
	}
	if (m_fadeFrame > kFadeFrame)
	{
		m_fadeFrame = kFadeFrame;
		m_isEnd = true;
	}

	m_player.Update();

	// プレイヤー死亡処理
	if (m_player.IsDead())
	{
		// 画面上の全敵を待機状態にする
		for (auto& enemy : m_enemyManager.GetEnemies())
		{
			enemy->SetIdle();
		}

		if (m_fadeSpeed == 0)
		{
			m_fadeSpeed = 1; // フェードアウト開始
		}

		m_bg.Update();
		m_frameCount++;
		return;
	}

	m_OnHit = false;
	m_OnEnemyHit = false;

	// 当たり判定
	// プレイヤーの攻撃ヒットボックス
	float pAtkX, pAtkY, pAtkW, pAtkH;
	bool isPlayerAttacking = m_player.GetAttackHitBox(pAtkX, pAtkY, pAtkW, pAtkH);

	// プレイヤーの本体ヒットボックスを取得
	float pBodyX, pBodyY, pBodyW, pBodyH;
	m_player.HitBox(pBodyX, pBodyY, pBodyW, pBodyH);

	// 全ての敵と当たり判定チェック
	for (auto& enemy : m_enemyManager.GetEnemies())
	{
		// プレイヤー攻撃 -> 敵本体
		if (isPlayerAttacking)
		{
			float eBodyX, eBodyY, eBodyW, eBodyH;
			enemy->HitBox(eBodyX, eBodyY, eBodyW, eBodyH);

			if (!enemy->IsDead() && CheckAABB(pAtkX, pAtkY, pAtkW, pAtkH, eBodyX, eBodyY, eBodyW, eBodyH))
			{
				m_OnHit = true;
				enemy->OnDamage(playerX, 1); // 敵に1ダメージを与える
			}
		}

		// 敵攻撃 -> プレイヤー本体
		float eAtkX, eAtkY, eAtkW, eAtkH;
		if (enemy->GetAttackHitBox(eAtkX, eAtkY, eAtkW, eAtkH))
		{
			if (CheckAABB(eAtkX, eAtkY, eAtkW, eAtkH, pBodyX, pBodyY, pBodyW, pBodyH))
			{
				m_OnEnemyHit = true;
				m_player.OnDamage(); // プレイヤーにダメージを与える
			}
		}
		// フェード処理
		m_fadeFrame += m_fadeSpeed;;
		if (m_fadeFrame < 0)	m_fadeFrame = 0;
		if (m_fadeFrame > kFadeFrame)
		{
			m_fadeFrame = kFadeFrame;
			m_isEnd = true;
		}
	}

	m_frameCount++;
	m_bg.Update();
}
// ゲームオーバーやクリア条件を満たしたらm_isEndをtrueにする
		// if (player.IsDead())
		// {
		//	m_isEnd = true;
		// }



void SceneMain::Draw()
{
	
	m_bg.Draw();
	m_enemyManager.Draw();
	m_player.Draw();

	// HP表示
	int hp = m_player.GetHp();
	int maxHp = m_player.GetMaxHp();

	// UIサイズ
	int startX = 80;
	int startY = 30;
	int  blockWidth = 14;	// ブロック1個の横幅
	int barHeight = 16;		// ブロックの高さ
	int blockSpacing = 3;	// ブロック同士の感覚

	unsigned int cWhite = GetColor(255, 0, 0);	// 体力背景
	unsigned int cBlack = GetColor(0, 0, 0);
	unsigned int cRed = GetColor(230, 30, 30);	// 体力ゲージ

	for (int i = 0; i < maxHp;i++)
	{
		int x1 = startX + i * (blockWidth + blockSpacing);
		int y1 = startY;
		int x2 = x1 + blockWidth;
		int y2 = y1 + barHeight;

		DrawBox(x1 - 1, y1 - 1, x2 + 1, y2 + 1, cBlack, TRUE);

		if (i < hp)
		{
			// のこりHP (赤)
			DrawBox(x1, y1, x2, y2, cWhite, TRUE);
		}
		else
		{
			// 被ダメ(黒)
			DrawBox(x1, y1, x2, y2, cBlack, TRUE);
		}
	}

	// フェード
	float frameRate = static_cast<float>(m_fadeFrame) / static_cast<float>(kFadeFrame);	// 浮動小数点数の計算のするためキャスト

	// 0.0~1.0 の割合を 0~255 の間の値に変換する
	int alpha = static_cast<int>(255 * frameRate);
	// 半透明で表示を開始
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	// 半透明で表示を終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	DrawString(0, 0, "SceneMain", GetColor(0, 255, 0));
	DrawFormatString(0, 16, GetColor(0, 255, 0), "FRAME:%d", m_frameCount);
	DrawFormatString(0, 80, GetColor(0, 255, 0), "ENEMYKILLCOUNT:%d/%d", m_enemyManager.GetSpawnCount(),m_enemyManager.GetTotalCount());

	// プレイヤーの攻撃が敵に当たった場合
	if (m_OnHit)
	{
		DrawString(0, 100, "PLAYER ATTACK HIT", GetColor(255, 255, 0));
	}

	// 敵の攻撃がプレイヤーに当たった場合
	if (m_OnEnemyHit)
	{
		DrawString(0, 120, "ENEMY ATTACK HIT!!", GetColor(255, 0, 0));
	}
#endif
}
