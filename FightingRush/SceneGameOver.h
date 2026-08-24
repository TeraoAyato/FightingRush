#pragma once
class SceneGameOver
{
public:
	SceneGameOver();
	~SceneGameOver();

	void Init();
	void End();
	void Update();
	void Draw();

	// シーン終了フラグの取得
	bool IsEnd() const { return m_isEnd; }

private:
	// タイトル終了フラグ	trueならゲームに移行する
	bool m_isEnd;

	int m_frameCount;

	// 背景ハンドル
	int m_BgHandle;
	int m_GameOverFontHandle;

	// フォント
	int m_fontHandle;

	// フェード管理
	int m_fadeFrame;
	int m_fadeSpeed;	// -1 or +1

	// 演出サインカーブ
	float m_sinAngle;

	// サウンド
	int m_bgmHandle;
};

