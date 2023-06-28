/*
プレイシーン
*/
#pragma once

#include "../IScene.h"
#include "Game/GameMain.h"

class GridFloor;
class Camera;

class PlayScene : public IScene
{
private:

public:

	// コンストラクタ
	PlayScene();

	// デストラクタ
	~PlayScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;
};