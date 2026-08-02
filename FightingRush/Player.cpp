#include "DxLib.h"
#include "Player.h"
#include "Game.h"

namespace
{
	// 攻撃アニメーション
	constexpr int kAttack1AnimNum = 6;	// 攻撃1【弱攻撃】
	constexpr int kAttack2AnimNum = 4;	// 攻撃2【弱攻撃】
	constexpr int kAttack3AnimNum = 6;	// 攻撃3【強攻撃】

	// プレイヤー
	constexpr int kDefaultPosX = 600;	// 初期X座標
	constexpr int kDefaultPosY = 240;	// 初期Y座標
	constexpr int kMoveSpeed = 5;	// 移動速度
	constexpr int kWidth = 128;	// キャラクターの横幅
	constexpr int kHeight = 128;	// キャラクターの縦幅

	// プレイヤー空白サイズ
	// 余白調整
	constexpr int kMarginLeft = 40;
	constexpr int kMarginRight = 50;
	constexpr int kMarginTop = 50;
	constexpr int kMarginBottom = -15;
}

Player::Player() :
	m_posX(kDefaultPosX),
	m_posY(kDefaultPosY),
	m_isMoving(false),
	m_isDirRight(true),
	m_isAttacking(false),
	m_isAttackInput(false),
	m_ComboInput(false),
	m_ComboInputTime(0),
	m_nextAttackType(0),
	m_attackFrame(0),
	m_frameCount(0),
	m_attackType(1)
{
	for (int i = 0; i < 7; i++)  m_idleHandle[i] = -1;
	for (int i = 0; i < 10; i++) m_runHandle[i] = -1;
	for (int i = 0; i < 6; i++) m_attack1Handle[i] = -1;
	for (int i = 0; i < 4; i++) m_attack2Handle[i] = -1;
	for (int i = 0; i < 6; i++) m_attack3Handle[i] = -1;
}

Player::~Player()
{
}

void Player::Init()
{
	m_posX = kDefaultPosX;
	m_posY = kDefaultPosY;
	m_isMoving = false;
	m_isDirRight = true;
	m_isAttacking = false;
	m_isAttackInput = false;
	m_ComboInput = false;
	m_ComboInputTime = 0;
	m_nextAttackType = 0;
	m_attackFrame = 0;
	m_frameCount = 0;
	m_attackType = 1;

	LoadDivGraph(
		"sozai/Player/Idle.png",	// ファイル名
		7,	// 総コマ数
		7,	// 横コマ数
		1,	// 縦コマ数
		kWidth,	// 1コマの幅
		kHeight,	// １コマの高さ
		m_idleHandle	// 保存配列
	);

	// 走り画像
	LoadDivGraph(
		"sozai/Player/run.png",	// ファイル名
		10,	// 総コマ数
		10,	// 横コマ数
		1,	// 縦コマ数
		kWidth,	// 1コマの幅
		kHeight,	// １コマの高さ
		m_runHandle	// 保存配列
	);

	// 攻撃1画像
	LoadDivGraph(
		"sozai/Player/Attack1.png",	// 素材ファイル名
		6,	// 総コマ数
		6,	// 横コマ数
		1,	// 縦コマ数
		kWidth,	// 1コマの幅
		kHeight,	// １コマの高さ
		m_attack1Handle	// 保存配列
	);
	// 攻撃1画像
	LoadDivGraph(
		"sozai/Player/Attack2.png",	// 素材ファイル名
		4,	// 総コマ数
		4,	// 横コマ数
		1,	// 縦コマ数
		kWidth,	// 1コマの幅
		kHeight,	// １コマの高さ
		m_attack2Handle	// 保存配列
	);
	// 攻撃3画像
	LoadDivGraph(
		"sozai/Player/Attack3.png",	// 素材ファイル名
		6,	// 総コマ数
		6,	// 横コマ数
		1,	// 縦コマ数
		kWidth,	// 1コマの幅
		kHeight,	// １コマの高さ
		m_attack3Handle	// 保存配列
	);

}

void Player::End()
{
	// 画像を一括で削除
	InitGraph();
}

void Player::Update()
{
	m_frameCount++;

	// コントローラー入力
	int padState = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	// 現在ボタンが押されているか
	bool isPushX = (padState & PAD_INPUT_3);
	bool isPushY = (padState & PAD_INPUT_4);
	bool isPushNow = isPushX || isPushY;

	// 攻撃ボタンが押されているか
	bool isTrigger = (!m_isAttackInput && isPushNow);
	m_isAttackInput = isPushNow;

	// 攻撃開始
	if (!m_isAttacking && isTrigger)
	{
		m_isAttacking = true;
		m_attackType = 1;
		m_attackFrame = 0;	// アニメーションタイマーをリセット
		m_ComboInput = false;

		// Yボタンなら【攻撃3】Xボタンなら【攻撃1、2】
		if (isPushY)
		{
			m_attackType = 3;	// 攻撃3【強攻撃】
		}
		else if (isPushX)
		{
			m_attackType = 1;	// 攻撃1【弱攻撃】
		}
	}
	else if (m_isAttacking && isTrigger)
	{
		if (m_attackFrame >= 10)
		{
			if (isPushY)
			{
				m_nextAttackType = 3;	// 攻撃3予約
			}
			else if (isPushX)
			{
				m_nextAttackType = 2;	// 攻撃2予約
			}
		}
		//	m_ComboInput = true;
	}

	// 移動中か判定
	m_isMoving = false;

	// 攻撃中でないときに移動可能
	if (!m_isAttacking)
	{

		// 入力に合わせて向きを変える
		if (padState & PAD_INPUT_LEFT)
		{
			// 左入力、向き
			m_posX -= kMoveSpeed;
			m_isMoving = true;
			m_isDirRight = false;
		}
		if (padState & PAD_INPUT_RIGHT)
		{
			// 右入力、向き
			m_posX += kMoveSpeed;
			m_isMoving = true;
			m_isDirRight = true;
		}
		if (padState & PAD_INPUT_UP)
		{
			m_posY -= kMoveSpeed;
			m_isMoving = true;
		}	// 上入力
		if (padState & PAD_INPUT_DOWN)
		{
			m_posY += kMoveSpeed;
			m_isMoving = true;
		}	// 下入力
	}

	// 画面外に出ない処理
	// 左端制限
	if (m_posX + kMarginLeft < 0)
	{
		m_posX = -kMarginLeft;
	}
	// 右端制限
	if (m_posX + kWidth - kMarginRight > Game::kScreenWidth)
	{
		m_posX = Game::kScreenWidth - kWidth + kMarginRight;
	}
	// 上端制限
	if (m_posY + kMarginTop < 0)
	{
		m_posY = -kMarginTop;
	}
	// 下端制限
	if (m_posY + kHeight - kMarginBottom > Game::kScreenHeight)
	{
		m_posY = Game::kScreenHeight - kHeight + kMarginBottom;
	}

	// 攻撃アニメーション切り替え処理
	if (m_isAttacking)
	{
		m_attackFrame++;
		int animSpeed = 5;

		// 【攻撃1】
		if (m_attackType == 1)
		{
			if (m_attackFrame >= kAttack1AnimNum * animSpeed)
			{
				if (m_nextAttackType == 2)
				{
					// 連打入力があれば【攻撃2】に移行
					m_attackType = 2;
					m_attackFrame = 0;
					m_nextAttackType = 0;
				}
				else if (m_nextAttackType == 3)
				{
					// 連打入力があれば【攻撃3】に移行
					m_attackType = 3;
					m_attackFrame = 0;
					m_nextAttackType = 0;
				}
				else
				{
					m_isAttacking = false;// アニメーションが終わったら攻撃終了
				}
			}
		}
		// 【攻撃2】
		else if (m_attackType == 2)
		{
			if (m_attackFrame >= kAttack2AnimNum * animSpeed)
			{
				if (m_nextAttackType == 3)
				{
					m_attackType = 3;
					m_attackFrame = 0;
					m_nextAttackType = 0;
				}
				else
				{
					m_isAttacking = false;
				}
			}
		}
		// 【攻撃3(強攻撃)】
		else if (m_attackType == 3)
		{
			if (m_attackFrame >= kAttack3AnimNum * animSpeed)
			{
				m_isAttacking = false;
				m_attackType = 1;
			}
		}
	}


}

void Player::Draw()
{
	int centerX = m_posX + kWidth / 2;
	int centerY = m_posY + kHeight / 2;
	float Size = 1.0;
	float Angle = 0.0;
	int turnFlag = m_isDirRight ? FALSE : TRUE;

	if (m_isAttacking)
	{
		int animIndex = m_attackFrame / 5;
		// 【攻撃1】
		if (m_attackType == 1)
		{
			if (animIndex >= kAttack1AnimNum)
			{
				animIndex = kAttack1AnimNum - 1;
			}
			DrawRotaGraph(centerX, centerY, Size, Angle, m_attack1Handle[animIndex], TRUE, turnFlag);
		}
		// 【攻撃2】
		if (m_attackType == 2)
		{
			if (animIndex >= kAttack2AnimNum)
			{
				animIndex = kAttack2AnimNum - 1;
			}
			DrawRotaGraph(centerX, centerY, Size, Angle, m_attack2Handle[animIndex], TRUE, turnFlag);
		}
		// 【攻撃3】
		else if (m_attackType == 3)
		{
			if (animIndex >= kAttack3AnimNum)
			{
				animIndex = kAttack3AnimNum - 1;
			}
			DrawRotaGraph(centerX, centerY, Size, Angle, m_attack3Handle[animIndex], TRUE, turnFlag);
		}

	}
	else if (m_isMoving)
	{
		int animIndex = (m_frameCount / 5) % 10;
		DrawRotaGraph(centerX, centerY, Size, Angle, m_runHandle[animIndex], TRUE, turnFlag);
	}
	else
	{
		int animIndex = (m_frameCount / 10) % 7;
		DrawRotaGraph(centerX, centerY, Size, Angle, m_idleHandle[animIndex], TRUE, turnFlag);
	}
	float offsetX = 0.0f;
	float offsetY = 0.0f;
	// デバッグ用表示
#ifdef _DEBUG
	DrawFormatString(0, 40, GetColor(255, 255, 255), "X:%d", m_posX);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "Y:%d", m_posY);
	DrawCircle(centerX,centerY, 5, GetColor(0, 0, 255), TRUE);
#endif
}
