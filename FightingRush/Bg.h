#pragma once
class Bg
{
public:
	Bg();
	~Bg();

	void Init();
	void End();
	void Update();
	void Draw();

private:
	int m_BgHandle;	// 背景ハンドル
};

