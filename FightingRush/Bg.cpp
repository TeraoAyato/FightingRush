#include "Bg.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
}

Bg::Bg():
	m_BgHandle(0)
{
}

Bg::~Bg()
{
}

void Bg::Init()
{
	m_BgHandle = LoadGraph("sozai/Map/tileset.png");
}

void Bg::End()
{// プログラム終了前にロードしたグラフィックをメモリから削除
	DeleteGraph(m_BgHandle);
}

void Bg::Update()
{
}

void Bg::Draw()
{
	if (m_BgHandle != -1)
	{
		DrawGraph(0, 0, m_BgHandle, TRUE);
	}
}
