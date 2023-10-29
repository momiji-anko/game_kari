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
#include<SpriteBatch.h>
#include"Game/Shader/Fade.h"

class GridFloor;
class PlayerCamera;
class GameMain;


class PlayScene : public IScene
{

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
private:
	//カメラ
	std::unique_ptr<PlayerCamera> m_camera;

	//プレイヤー
	Actor* m_actor;

	//親
	GameMain* m_parent;

	//フェード
	std::unique_ptr<Fade> m_fade;
	//コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	//シーングラフ
	std::unique_ptr<SceneGraph> m_sceneGraph;
	//コリジョンマネージャー
	std::unique_ptr<CollisionManager> m_collisionManager;


	//落下死亡エリア
	std::unique_ptr<AABBFor3D> m_fallDeathArea;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;


};