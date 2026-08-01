#pragma once
class Bg
{
public:
	Bg();	// コンストラクタ
	~Bg();	// デストラクタ

	void Init();	// 初期化
	void End();		// 終了処理
	void Update();	// 更新
	void Draw();	// 描画

private:
	// グラフィックハンドル
	int m_BgHandle;
};

