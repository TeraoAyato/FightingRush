#pragma once

class Effect
{
public:
	Effect();
	~Effect();

	void Init();
	void End();
	void Update();
	void Draw();

	void Play(float x, float y);

private:
	static constexpr int kMaxFrame = 7; // 画像のコマ数
		
	// エフェクトハンドル
	int m_atkHitHandle[kMaxFrame];


	float m_x;
	float m_y;
	
	// 再生しているか
	bool m_isPlaying;

	// アニメーションフレームカウント
	int m_frame;
};