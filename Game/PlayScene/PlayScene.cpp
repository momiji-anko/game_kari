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
#include<SpriteBatch.h>

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

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	//スプライトバッチをゲームコンテキストにセット
	GameContext::GetInstance().SetSpriteBatch(m_spriteBatch.get());
	
	m_camera = std::make_unique<PlayerCamera>();
	m_camera->Initialize();


	//	コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	m_collisionManager = std::make_unique<CollisionManager>();

	GameContext().GetInstance().SetCollisionManager(m_collisionManager.get());
	GameContext().GetInstance().SetCommonState(m_commonState.get());

	//プレイヤー作成
	
	std::unique_ptr<Actor> player = std::make_unique<Player>(DirectX::SimpleMath::Vector3(0, 3, 0), DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.02f, 0.02f, 0.02f), DirectX::SimpleMath::Vector3(0, 0, 0), ModelManager::GetInstance().LoadSdkmeshModel(L"Walking.sdkmesh"), true);
	player->Initialize();
	m_actor = player.get();

	//シーングラフ作成
	m_sceneGraph = std::make_unique<SceneGraph>();
	m_sceneGraph->Initialize();
	//シーングラフにプレイヤーをアタッチする
	m_sceneGraph->AttachNode(std::move(player));

	//落下死亡エリア作成
	m_fallDeathArea = std::make_unique<AABBFor3D>();
	m_fallDeathArea->Initialize();
	m_fallDeathArea->SetData(DirectX::SimpleMath::Vector3(-10000.f,-10000.0f,-10000.f), DirectX::SimpleMath::Vector3(10000.f,-50.f,10000.f));
	GameContext::GetInstance().GetCollisionManager()->SetfallDeathAABB(m_fallDeathArea.get());
	GameContext::GetInstance().SetPlayerDeath(false);

	//ステージマネージャー
	std::unique_ptr<Actor> stageManager = std::make_unique<StageManager>(0);
	stageManager->Initialize();
	//シーングラフにステージマネージャーをアタッチする
	m_sceneGraph->AttachNode(std::move(stageManager));

	//敵マネージャー
	std::unique_ptr<Actor> enemyManager = std::make_unique<EnemyManager>(0);
	enemyManager->Initialize();
	//シーングラフに敵マネージャーをアタッチする
	m_sceneGraph->AttachNode(std::move(enemyManager));

	//障害物マネージャー作成
	std::unique_ptr<Actor> obstacleManager = std::make_unique<ObstacleManager>(0);
	obstacleManager->Initialize();
	//シーングラフに障害物マネージャーをアタッチする
	m_sceneGraph->AttachNode(std::move(obstacleManager));

	//キーマネージャー作成
	std::unique_ptr<Actor> keyManager = std::make_unique<KeyManager>(0);
	keyManager->Initialize();
	//シーングラフにキーマネージャーをアタッチする
	m_sceneGraph->AttachNode(std::move(keyManager));

	//フェードの作成
	m_fade = std::make_unique<Fade>();
	//shader作成
	m_fade->Create();
	//初期化
	m_fade->Initialize(DirectX::SimpleMath::Vector3::Zero);
	//最初にフェードインする
	m_fade->FadeIn();
}

/*--------------------------------------------------
更新
--------------------------------------------------*/
void PlayScene::Update(const DX::StepTimer& timer)
{
	//フェード更新
	m_fade->Update(timer);

	//フェードインしていない場合かつプレイヤーがアクティブである場合これ以降処理しない
	if (!m_fade->ISOpen() && m_actor->IsActive())
		return ;

	//シーングラフ更新
	m_sceneGraph->Update(timer);

	//カメラ更新
	m_camera->Update();

	//プレイヤーが死亡している場合リザルトシーンに以降
	if (!m_actor->IsActive())
	{
		//フェードアウトする
		m_fade->FadeOut();


		//フェードアウトし終わったらリザルトへ以降
		if(m_fade->ISClose())
			m_parent->ChengeScene(m_parent->GetResultScene());
	}
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();


	//シーングラフ描画
	m_sceneGraph->Render(m_camera.get());


	//フェード描画
	m_fade->Render();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void PlayScene::Finalize()
{
}
