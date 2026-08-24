#pragma once

class Effect
{
public:
	Effect();
	~Effect();

	void Init();
	void End();
	void Play(float x, float y);
	void Update();
	void Draw();

private:
	static constexpr int kMaxFrames = 7; // 画像のコマ数

	float m_x;
	float m_y;
	int m_frame;
	bool m_isPlaying;
	int m_handles[kMaxFrames];
};