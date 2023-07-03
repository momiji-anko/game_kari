/*
プレイシーン
*/
#include "pch.h"

#include "DeviceResources.h"
#include "../GameMain.h"
#include<windows.h>
#include "PlayScene.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"



/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
PlayScene::PlayScene()
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

	m_camera = std::make_unique<Camera>();

	//	コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	GameContext().GetInstance().SetCommonState(m_commonState.get());

	m_player = std::make_unique<Player>();
	
	m_player->Initialize(DirectX::SimpleMath::Vector3(0, 1, 0), DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(1, 1, 1), DirectX::SimpleMath::Vector3(0, 0, 0), ModelManager::GetInstance().LoadModel(L"Resources/Models/dice.cmo"), true);


	m_goal = std::make_unique<Goal>();
	m_goal->Initialize(DirectX::SimpleMath::Vector3(2, 0, 0), DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(1, 1, 1), DirectX::SimpleMath::Vector3(0, 0, DirectX::XM_PI/4.0f), ModelManager::GetInstance().LoadModel(L"Resources/Models/dice.cmo"), true);
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	m_player->Update(timer);
	m_goal->Update(timer);

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	m_player->Render(m_camera.get());

	if(!m_player->GetAABB()->DetectCollition(m_goal->GetAABB()))
	m_goal->Render(m_camera.get());


}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void PlayScene::Finalize()
{
}
