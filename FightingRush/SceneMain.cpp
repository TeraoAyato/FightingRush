#include "SceneMain.h"
#include "DxLib.h"
#include "Bg.h"
#include "Enemy.h"
#include "SceneTitle.h"
#include "Game.h"
#include "EnemyManager.h"
#include "Player.h"

namespace
{
	// 2つの矩形が重なっているかチェック
	bool CheckAABB(float x1, float y1, float w1, float h1,
		float x2, float y2, float w2, float h2)
	{
		if (x1 < x2 + w2 &&
			x1 + w1 > x2 &&
			y1 < y2 + h2 &&
			y1 + h1 > y2)
		{
			return true; // 重なっている
		}
		return false; // 重なっていない
	}

	// フェードにかかるフレーム数
	constexpr int kFadeFrame = 30;

	// キー入力を受け付けないフレーム数
	constexpr int kKeyInputWaitFrame = 60;
}

SceneMain::SceneMain() :
	m_frameCount(0),
	m_isEnd(false),
	m_isClear(false),
	m_fadeFrame(0),
	m_fadeSpeed(0),
	m_OnHit(false),
	m_PlayerhitSoundHandle(-1),
	m_EnemyhitSoundHandle(-1),
	m_bgmHandle(-1),
	m_PlayerDieSeHandle(-1),
	m_playTimeFrame(0),
	m_fontHandle(-1),
	m_playerFontHandle(-1),
	m_manualFontHandle(-1)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	// 初期化時終了フラグ
	m_isEnd = false;
	m_isClear = false;
	m_frameCount = 0;
	m_playTimeFrame = 0;

	m_fadeFrame = kFadeFrame;
	m_fadeSpeed = -1;

	m_bg.Init();

	m_player.Init();
	m_hitEffect.Init();
	m_enemyManager.Init();

	// ヒットサウンド読み込み
	m_PlayerhitSoundHandle = LoadSoundMem("sozai/Sound/PlayerPunch.mp3");
	m_EnemyhitSoundHandle = LoadSoundMem("sozai/Sound/EnemyPunch.mp3");
	m_PlayerDieSeHandle = LoadSoundMem("sozai/sound/Die.mp3");

	// BGM読み込み
	m_bgmHandle = LoadSoundMem("sozai/Sound/SceneMainBgm.mp3");
	ChangeVolumeSoundMem(180, m_bgmHandle);
	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);

	// タイマー文字
	m_fontHandle = CreateFontToHandle("Noto Sans JP Black", 40, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4,-1,3);

	m_playerFontHandle = CreateFontToHandle("Noto Sans JP Black", 15, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4,-1,2);
	// シーンメイン内操作方法フォント
	m_manualFontHandle = CreateFontToHandle("Noto Sans JP Black", 15, -1, DX_FONTTYPE_ANTIALIASING_EDGE_4X4,-1,2);
}

void SceneMain::End()
{
	m_bg.End();

	m_player.End();
	m_hitEffect.End();
	m_enemyManager.End();

	// プレイヤーヒットサウンドの削除
	if (m_PlayerhitSoundHandle != 1)
	{
		DeleteSoundMem(m_PlayerhitSoundHandle);
		m_PlayerhitSoundHandle = -1;
	}
	// エネミーヒットサウンドの削除
	if (m_EnemyhitSoundHandle != 1)
	{
		DeleteSoundMem(m_EnemyhitSoundHandle);
		m_EnemyhitSoundHandle = -1;
	}

	if (m_PlayerDieSeHandle != 1)
	{
		DeleteSoundMem(m_EnemyhitSoundHandle);
		m_EnemyhitSoundHandle = -1;
	}

	// BGMの削除
	if (m_bgmHandle != -1)
	{
		StopSoundMem(m_bgmHandle); // 再生停止
		DeleteSoundMem(m_bgmHandle); // メモリ解放
		m_bgmHandle = -1;
	}

	if (m_fontHandle != -1)
	{
		DeleteFontToHandle(m_fontHandle);
			m_fontHandle = -1;
	}

	if (m_playerFontHandle != -1)
	{
		DeleteFontToHandle(m_playerFontHandle);
			m_playerFontHandle = -1;
	}
	if (m_manualFontHandle != -1)
	{
		DeleteFontToHandle(m_manualFontHandle);
			m_manualFontHandle = -1;
	}
}

void SceneMain::Update()
{
	// プレイヤー座標取得
	float playerX = m_player.GetCenterX();
	float playerY = m_player.GetCenterY();

	// エネミー全体の更新
	m_enemyManager.Update(playerX, playerY);

	// フェード処理
	m_fadeFrame += m_fadeSpeed;
	if (m_fadeFrame < 0)
	{
		m_fadeFrame = 0;
		m_fadeSpeed = 0;
	}
	if (m_fadeFrame > kFadeFrame)
	{
		m_fadeFrame = kFadeFrame;
		m_isEnd = true;
	}

	m_player.Update();
	m_hitEffect.Update();

	// プレイヤー死亡処理
	if (m_player.IsDead())
	{
		// 画面上の全敵を待機状態にする
		for (auto& enemy : m_enemyManager.GetEnemies())
		{
			enemy->SetIdle();
		}

		if (m_fadeSpeed == 0)
		{
			m_fadeSpeed = 1; // フェードアウト開始
		}

		// プレイヤー攻撃ヒット音
		if (m_PlayerDieSeHandle != -1)
		{
			PlaySoundMem(m_PlayerDieSeHandle, DX_PLAYTYPE_BACK);
			ChangeVolumeSoundMem(200, m_PlayerDieSeHandle);
		}

		m_bg.Update();
		m_frameCount++;
		return;
	}

	if (m_enemyManager.IsAllEnemyDie())
	{
		m_isClear = true;	// クリア判定
		m_fadeSpeed = 1;

		if (m_fadeSpeed == 0)
		{
			m_fadeSpeed = 1;	// クリアフェードアウト
		}
		m_bg.Update();
		m_frameCount++;
		return;
	}

	m_OnHit = false;
	m_OnEnemyHit = false;

	// 当たり判定
	// プレイヤーの攻撃ヒットボックス
	float pAtkX, pAtkY, pAtkW, pAtkH;
	bool isPlayerAttacking = m_player.GetAttackHitBox(pAtkX, pAtkY, pAtkW, pAtkH);

	// プレイヤーの本体ヒットボックスを取得
	float pBodyX, pBodyY, pBodyW, pBodyH;
	m_player.HitBox(pBodyX, pBodyY, pBodyW, pBodyH);

	// 全ての敵と当たり判定チェック
	for (auto& enemy : m_enemyManager.GetEnemies())
	{
		// プレイヤー攻撃 -> 敵本体
		if (isPlayerAttacking)
		{
			float eBodyX, eBodyY, eBodyW, eBodyH;
			enemy->HitBox(eBodyX, eBodyY, eBodyW, eBodyH);

			if (!enemy->IsDead() && CheckAABB(pAtkX, pAtkY, pAtkW, pAtkH, eBodyX, eBodyY, eBodyW, eBodyH))
			{
				m_OnHit = true;
				enemy->OnDamage(playerX, m_player.GetAttackPower()); // 敵に1ダメージを与える

				// ★ 向きに合わせて先端側のX座標を計算
				float effectX = 0.0f;
				if (m_player.IsDirRight())
				{
					effectX = pAtkX + pAtkW * 0.7f; // 右向き時の手の先
				}
				else
				{
					effectX = pAtkX + pAtkW * 0.3f; // 左向き時の手の先
				}

				m_hitEffect.Play(effectX, eBodyY + eBodyH * 0.3f);

				// プレイヤー攻撃ヒット音
				if (m_PlayerhitSoundHandle != -1)
				{
					PlaySoundMem(m_PlayerhitSoundHandle, DX_PLAYTYPE_BACK);
					ChangeVolumeSoundMem(200, m_PlayerhitSoundHandle);
				}
				m_hitEffect.Play(pAtkX + pAtkW * 0.7f, eBodyY + eBodyH * 0.3f);

				// プレイヤー攻撃ヒット音
				if (m_PlayerhitSoundHandle != -1)
				{
					PlaySoundMem(m_PlayerhitSoundHandle, DX_PLAYTYPE_BACK);
					ChangeVolumeSoundMem(200, m_PlayerhitSoundHandle);
				}
			}
		}

		// 敵攻撃 -> プレイヤー本体
		float eAtkX, eAtkY, eAtkW, eAtkH;
		if (enemy->GetAttackHitBox(eAtkX, eAtkY, eAtkW, eAtkH))
		{
			if (CheckAABB(eAtkX, eAtkY, eAtkW, eAtkH, pBodyX, pBodyY, pBodyW, pBodyH))
			{
				m_OnEnemyHit = true;
				m_player.OnDamage(); // プレイヤーにダメージを与える

				// エネミー攻撃ヒット音
				if (m_EnemyhitSoundHandle != -1)
				{
					PlaySoundMem(m_EnemyhitSoundHandle, DX_PLAYTYPE_BACK);
					ChangeVolumeSoundMem(200, m_EnemyhitSoundHandle);
				}
			}
		}
		// フェード処理
		m_fadeFrame += m_fadeSpeed;;
		if (m_fadeFrame < 0)	m_fadeFrame = 0;
		if (m_fadeFrame > kFadeFrame)
		{
			m_fadeFrame = kFadeFrame;
			m_isEnd = true;
		}
	}
	// タイマー
	if (!m_isClear && !m_player.IsDead())
	{
		m_playTimeFrame++;
	}

	m_frameCount++;
	m_bg.Update();
}
// ゲームオーバーやクリア条件を満たしたらm_isEndをtrueにする
		// if (player.IsDead())
		// {
		//	m_isEnd = true;
		// }

void SceneMain::Draw()
{
	m_bg.Draw();

	int boxWidth = 260;
	int boxHeight = 100;

	int x1 = 0;
	int y1 = Game::kScreenHeight - boxHeight;
	int x2 = boxWidth;
	int y2 = Game::kScreenHeight;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

	// 2. 四角形を描画（黒や白など好みの色で）
	DrawBox(x1, y1, x2, y2, GetColor(0, 0, 0), TRUE);

	// 3. 通常の描画モードに戻す（
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawFormatStringToHandle(5, 620, GetColor(255, 255, 255), m_manualFontHandle, "十字キーorスティック…移動");
	DrawFormatStringToHandle(5, 650, GetColor(255, 255, 255), m_manualFontHandle, "Xボタン…弱攻撃(連続攻撃)");
	DrawFormatStringToHandle(5, 680, GetColor(255, 255, 255), m_manualFontHandle, "Yボタン…強攻撃(単発攻撃)");

	m_enemyManager.Draw();
	m_player.Draw();
	m_hitEffect.Draw();

	// HP表示
	int hp = m_player.GetHp();
	int maxHp = m_player.GetMaxHp();

	// UIサイズ
	int startX = 200;
	int startY = 30;
	int  blockWidth = 12;	// ブロック1個の横幅
	int barHeight = 24;		// ブロックの高さ
	int blockSpacing = 3;	// ブロック同士の感覚

	unsigned int cWhite = GetColor(255, 0, 0);	// 体力背景
	unsigned int cBlack = GetColor(0, 0, 0);
	unsigned int cRed = GetColor(230, 30, 30);	// 体力ゲージ

	// 体力ゲージの部分
	for (int i = 0; i < maxHp;i++)
	{
		int x1 = startX + i * (blockWidth + blockSpacing);
		int y1 = startY;
		int x2 = x1 + blockWidth;
		int y2 = y1 + barHeight;

		DrawBox(x1 - 1, y1 - 1, x2 + 1, y2 + 1, cBlack, TRUE);

		if (i < hp)
		{
			// のこりHP (赤)
			DrawBox(x1, y1, x2, y2, cWhite, TRUE);
		}
		else
		{
			// 被ダメ(黒)
			DrawBox(x1, y1, x2, y2, cBlack, TRUE);
		}
	}

	// フレーム数を秒単位に変換
	int totalSeconds = m_playTimeFrame / 60;
	int min = totalSeconds / 60;
	int sec = totalSeconds % 60;
	int millis = (m_playTimeFrame % 60) * 100 / 60;

	int TimerX = 900;
	int TimerY = 20;

	// タイマー描画
	DrawFormatStringToHandle(TimerX, TimerY, GetColor(255, 255, 255), m_fontHandle, "Time %02d:%02d.%02d", min, sec, millis);

	// Player文字
	DrawFormatStringToHandle(120, 30, GetColor(255, 255, 255), m_playerFontHandle, "Player");

	// フェード
	float frameRate = static_cast<float>(m_fadeFrame) / static_cast<float>(kFadeFrame);	// 浮動小数点数の計算のするためキャスト

	// 0.0~1.0 の割合を 0~255 の間の値に変換する
	int alpha = static_cast<int>(255 * frameRate);
	// 半透明で表示を開始
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), true);
	// 半透明で表示を終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	DrawString(0, 0, "SceneMain", GetColor(0, 255, 0));
	DrawFormatString(0, 16, GetColor(0, 255, 0), "FRAME:%d", m_frameCount);
	DrawFormatString(0, 80, GetColor(0, 255, 0), "ENEMYKILLCOUNT:%d/%d", m_enemyManager.GetSpawnCount(), m_enemyManager.GetTotalCount());

	// プレイヤーの攻撃が敵に当たった場合
	if (m_OnHit)
	{
		DrawString(0, 100, "PLAYER ATTACK HIT", GetColor(255, 255, 0));
	}

	// 敵の攻撃がプレイヤーに当たった場合
	if (m_OnEnemyHit)
	{
		DrawString(0, 120, "ENEMY ATTACK HIT!!", GetColor(255, 0, 0));
	}
#endif
}