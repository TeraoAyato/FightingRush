#pragma once
class SceneTitle
{
public:
	SceneTitle();
	~SceneTitle();

	void Init();
	void End();
	void Update();
	void Draw();

	// シーン終了フラグの取得
	bool IsEnd() const { return m_isEnd; }

private:
	// タイトル終了フラグ	trueならゲームに移行する
	bool m_isEnd;
	// Aボタン押したかどうか
	bool m_isStart;

	int m_frameCount;
	// タイトルロゴ画像のハンドル
	int m_logoHandle;

	// タイトル背景ハンドル
	int m_TitleBgHandle;

	// フォント
	int m_fontHandle;

	// フェード管理
	int m_fadeFrame;
	int m_fadeSpeed;	// -1 or +1

	// 演出サインカーブ
	float m_sinAngle;

	// サウンド
	int m_bgmHandle;
	int m_seHandle;
};

