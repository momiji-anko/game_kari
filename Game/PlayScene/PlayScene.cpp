/*
プレイシーン
*/
#include "pch.h"

#include "DeviceResources.h"
#include "../GameMain.h"
#include<windows.h>
#include "PlayScene.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/PlayerCamera.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Game/GameObject/Key/KeyManager.h"
#include"Game/GameObject/Stage/StageManager.h"
#include"Game/GameObject/Enemy/EnemyManager.h"
#include"Game/GameObject/Obstacle/ObstacleManager.h"


/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
PlayScene::PlayScene(GameMain* parent)
	:
	m_parent(parent)
{
}


/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
PlayScene::~PlayScene()
{
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void PlayScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_camera = std::make_unique<PlayerCamera>();
	m_camera->Initialize();

	//	コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	m_collisionManager = std::make_unique<CollisionManager>();

	GameContext().GetInstance().SetCollisionManager(m_collisionManager.get());
	GameContext().GetInstance().SetCommonState(m_commonState.get());

	m_player = std::make_unique<Player>(DirectX::SimpleMath::Vector3(0, 3, 0), DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.02f, 0.02f, 0.02f), DirectX::SimpleMath::Vector3(0, 0, 0), ModelManager::GetInstance().LoadSdkmeshModel(L"Walking.sdkmesh"), true);
	
	m_player->Initialize();
	m_actor = m_player.get();
	m_sceneGraph = std::make_unique<SceneGraph>();
	m_sceneGraph->Initialize();

	m_sceneGraph->AttachNode(std::move(m_player));

	m_area = std::make_unique<AABBFor3D>();
	m_area->Initialize();
	m_area->SetData(DirectX::SimpleMath::Vector3(-10000.f,-100.0f,-10000.f), DirectX::SimpleMath::Vector3(10000.f,-50.f,10000.f));
	GameContext::GetInstance().GetCollisionManager()->SetfallDeathAABB(m_area.get());
	GameContext::GetInstance().SetPlayerDeath(false);

	std::unique_ptr<Actor> stageManager = std::make_unique<StageManager>(0);
	stageManager->Initialize();
	m_sceneGraph->AttachNode(std::move(stageManager));

	std::unique_ptr<Actor> enemyManager = std::make_unique<EnemyManager>(0);
	enemyManager->Initialize();
	m_sceneGraph->AttachNode(std::move(enemyManager));


	std::unique_ptr<Actor> obstacleManager = std::make_unique<ObstacleManager>(0);
	obstacleManager->Initialize();
	m_sceneGraph->AttachNode(std::move(obstacleManager));


	std::unique_ptr<Actor> keyManager = std::make_unique<KeyManager>(0);
	keyManager->Initialize();
	m_sceneGraph->AttachNode(std::move(keyManager));
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
void PlayScene::Update(const DX::StepTimer& timer)
{

	m_sceneGraph->Update(timer);

	m_camera->Update();

	if(!m_actor->IsActive())
	m_parent->ChengeScene(m_parent->GetResultScene());

	return ;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	m_sceneGraph->Render(m_camera.get());

}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void PlayScene::Finalize()
{
}
