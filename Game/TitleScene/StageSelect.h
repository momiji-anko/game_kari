#pragma once
#include"StepTimer.h"
#include<SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include<Keyboard.h>
#include "../IScene.h"
#include "Game/GameMain.h"
#include"Game/GameObject/Stage/StageManager.h"

class StageSelect
{
public:
	/// <summary>
   /// 選択したステージ番号の取得
   /// </summary>
   /// <returns>選択したステージ番号</returns>
	int  GetSelectStageNum() { return m_selectStageNum; }

	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageSelect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageSelect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	/// <returns>次の選択画面に行くか</returns>
	bool  Update(const DX::StepTimer& timer);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ステージ選択のUI表示
	/// </summary>
	void StageSelectUIDraw();


private:

	//座標
	DirectX::SimpleMath::Vector2 m_texturePosition;

	//選択したステージの番号
	int m_selectStageNum;
	//ステージの最大数
	int m_stageMaxNum;


};