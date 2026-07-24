#pragma once
#include "Vec2.h"

class Player
{
public:
	Player();
	~Player();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	int m_idleHandle;
};

