/*
プレイシーン
*/
#pragma once

#include "../IScene.h"
#include "Game/GameMain.h"
#include"Game/GameObject/Player/Player.h"
#include<CommonStates.h>
#include"Game/GameObject/Goal/Goal.h"
#include"Game/SceneGraph/SceneGraph.h"
#include"Game/GameObject/Stage/StageManager.h"
#include"Game/GameObject/Collider/CollisionManager/CollisionManager.h"

class GridFloor;
class PlayerCamera;
class GameMain;


class PlayScene : public IScene
{
private:

	std::unique_ptr<PlayerCamera> m_camera;

	std::unique_ptr<Actor> m_player;
	Actor* m_actor;
	std::unique_ptr<Actor> m_goal;

	std::unique_ptr<SceneGraph> m_sceneGraph;

	std::unique_ptr<StageManager> m_stageManager;

	std::unique_ptr<DirectX::CommonStates> m_commonState;

	GameMain* m_parent;

	std::unique_ptr<CollisionManager> m_collisionManager;

	std::unique_ptr<AABBFor3D> m_area;

public:

	// コンストラクタ
	PlayScene(GameMain* parent);

	// デストラクタ
	~PlayScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;
};