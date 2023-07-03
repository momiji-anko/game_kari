/*
プレイシーン
*/
#pragma once

#include "../IScene.h"
#include "Game/GameMain.h"
#include"Game/GameObject/Player/Player.h"
#include<CommonStates.h>
#include"Game/GameObject/Goal/Goal.h"

class GridFloor;
class Camera;

class PlayScene : public IScene
{
private:

	std::unique_ptr<Camera> m_camera;

	std::unique_ptr<Player> m_player;

	std::unique_ptr<Goal> m_goal;

	std::unique_ptr<DirectX::CommonStates> m_commonState;

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