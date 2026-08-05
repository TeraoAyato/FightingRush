#include "SceneMain.h"
#include "DxLib.h"
#include "Bg.h"

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
	m_frameCount++;

	m_bg.Update();
	
	m_enemy.Update(m_player.GetCenterX(), m_player.GetCenterY());
	m_player.Update();
	
}

void SceneMain::Draw()
{
	
	m_bg.Draw();
#ifdef _DEBUG
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);


#endif
	
	m_enemy.Draw();
	m_player.Draw();
	
}
