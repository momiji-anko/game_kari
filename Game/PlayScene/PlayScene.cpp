/*
�v���C�V�[��
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



/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
PlayScene::PlayScene(GameMain* parent)
	:
	m_parent(parent)
{
}


/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
PlayScene::~PlayScene()
{
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void PlayScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_camera = std::make_unique<PlayerCamera>();
	m_camera->Initialize();

	//	�R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	m_collisionManager = std::make_unique<CollisionManager>();

	GameContext().GetInstance().SetCollisionManager(m_collisionManager.get());
	GameContext().GetInstance().SetCommonState(m_commonState.get());

	m_player = std::make_unique<Player>(DirectX::SimpleMath::Vector3(0, 3, 0), DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(1, 1, 1), DirectX::SimpleMath::Vector3(DirectX::XM_PI / 4.0f, 0, 0), ModelManager::GetInstance().LoadModel(L"dice.cmo"), true);
	
	m_player->Initialize();


	m_goal = std::make_unique<Goal>(DirectX::SimpleMath::Vector3(-157.779f, 4.13443f, 65.634f), DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(1, 1, 1 ), DirectX::SimpleMath::Vector3(DirectX::XM_PI/4.0f, 0, 0), ModelManager::GetInstance().LoadModel(L"dice.cmo"), true);
	m_goal->Initialize();


	m_sceneGraph = std::make_unique<SceneGraph>();
	m_sceneGraph->Initialize();
	m_sceneGraph->AttachNode(std::move(m_player));
	//m_sceneGraph->AttachNode(std::move(m_goal));

	

}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
void PlayScene::Update(const DX::StepTimer& timer)
{
	m_camera->Update();

	m_sceneGraph->Update(timer);


	if(GameContext().GetInstance().GetCollisionManager()->DetectCollisionPlayer2Goal()|| GameContext().GetInstance().GetCollisionManager()->DetectCollisionPlayer2Enemies())
	m_parent->ChengeScene(m_parent->GetTitleScene());

	return ;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	m_sceneGraph->Render(m_camera.get());


}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void PlayScene::Finalize()
{
}
