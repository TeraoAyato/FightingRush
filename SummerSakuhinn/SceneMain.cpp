#include "SceneMain.h"
#include "DxLib.h"

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
}

void SceneMain::End()
{
}

void SceneMain::Update()
{
	m_frameCount++;
}

void SceneMain::Draw()
{
	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME:%d", m_frameCount);
}
