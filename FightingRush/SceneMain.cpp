#include "SceneMain.h"
#include "DxLib.h"
#include "Bg.h"

namespace
{
	// 2つの矩形（四角形）が重なっているかチェックする関数 (AABB判定)
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
}

SceneMain::SceneMain(): 
	m_frameCount(0)
{
	
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	m_frameCount = 0;

	m_bg.Init();
	
	m_enemy.Init();
	m_player.Init();
	
}

void SceneMain::End()
{
	m_bg.End();
	
	m_enemy.End();
	m_player.End();
	
}

void SceneMain::Update()
{
	m_player.Update();
	m_enemy.Update(m_player.GetCenterX(), m_player.GetCenterY());

	m_OnHit = false;
	m_OnEnemyHit = false;

	// プレイヤーの攻撃判定
	float pAtkX, pAtkY, pAtkW, pAtkH;
	if (m_player.GetAttackHitBox(pAtkX, pAtkY, pAtkW, pAtkH))
	{
		float eBodyX, eBodyY, eBodyW, eBodyH;
		m_enemy.HitBox(eBodyX, eBodyY, eBodyW, eBodyH);

		if (CheckAABB(pAtkX, pAtkY, pAtkW, pAtkH, eBodyX, eBodyY, eBodyW, eBodyH))
		{
			m_OnHit = true;

			m_enemy.OnDamage(m_player.GetCenterX()); // 敵にダメージを与える
		}
	}

	// 敵の攻撃判定
	float eAtkX, eAtkY, eAtkW, eAtkH;
	if (m_enemy.GetAttackHitBox(eAtkX, eAtkY, eAtkW, eAtkH))
	{
		float pBodyX, pBodyY, pBodyW, pBodyH;
		m_player.HitBox(pBodyX, pBodyY, pBodyW, pBodyH);

		// 当たり判定
		if (CheckAABB(eAtkX, eAtkY, eAtkW, eAtkH, pBodyX, pBodyY, pBodyW, pBodyH))
		{
			m_OnEnemyHit = true;

			m_player.OnDamage(); // プレイヤーにダメージを与える

			
		}
	}


	m_frameCount++;


	m_bg.Update();
	
	
	
	
}

void SceneMain::Draw()
{
	
	m_bg.Draw();
#ifdef _DEBUG
	DrawString(0, 0, "SceneMain", GetColor(0, 255, 0));
	DrawFormatString(0, 16, GetColor(0, 255, 0), "FRAME:%d", m_frameCount);

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
	
	m_enemy.Draw();
	m_player.Draw();
	
}
