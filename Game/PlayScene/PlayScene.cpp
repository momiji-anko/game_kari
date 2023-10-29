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
#include"Game/GameObject/Key/KeyManager.h"
#include"Game/GameObject/Stage/StageManager.h"
#include"Game/GameObject/Enemy/EnemyManager.h"
#include"Game/GameObject/Obstacle/ObstacleManager.h"
#include<SpriteBatch.h>

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

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	//�X�v���C�g�o�b�`���Q�[���R���e�L�X�g�ɃZ�b�g
	GameContext::GetInstance().SetSpriteBatch(m_spriteBatch.get());
	
	m_camera = std::make_unique<PlayerCamera>();
	m_camera->Initialize();


	//	�R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	m_collisionManager = std::make_unique<CollisionManager>();

	GameContext().GetInstance().SetCollisionManager(m_collisionManager.get());
	GameContext().GetInstance().SetCommonState(m_commonState.get());

	//�v���C���[�쐬
	
	std::unique_ptr<Actor> player = std::make_unique<Player>(DirectX::SimpleMath::Vector3(0, 3, 0), DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0.02f, 0.02f, 0.02f), DirectX::SimpleMath::Vector3(0, 0, 0), ModelManager::GetInstance().LoadSdkmeshModel(L"Walking.sdkmesh"), true);
	player->Initialize();
	m_actor = player.get();

	//�V�[���O���t�쐬
	m_sceneGraph = std::make_unique<SceneGraph>();
	m_sceneGraph->Initialize();
	//�V�[���O���t�Ƀv���C���[���A�^�b�`����
	m_sceneGraph->AttachNode(std::move(player));

	//�������S�G���A�쐬
	m_fallDeathArea = std::make_unique<AABBFor3D>();
	m_fallDeathArea->Initialize();
	m_fallDeathArea->SetData(DirectX::SimpleMath::Vector3(-10000.f,-10000.0f,-10000.f), DirectX::SimpleMath::Vector3(10000.f,-50.f,10000.f));
	GameContext::GetInstance().GetCollisionManager()->SetfallDeathAABB(m_fallDeathArea.get());
	GameContext::GetInstance().SetPlayerDeath(false);

	//�X�e�[�W�}�l�[�W���[
	std::unique_ptr<Actor> stageManager = std::make_unique<StageManager>(0);
	stageManager->Initialize();
	//�V�[���O���t�ɃX�e�[�W�}�l�[�W���[���A�^�b�`����
	m_sceneGraph->AttachNode(std::move(stageManager));

	//�G�}�l�[�W���[
	std::unique_ptr<Actor> enemyManager = std::make_unique<EnemyManager>(0);
	enemyManager->Initialize();
	//�V�[���O���t�ɓG�}�l�[�W���[���A�^�b�`����
	m_sceneGraph->AttachNode(std::move(enemyManager));

	//��Q���}�l�[�W���[�쐬
	std::unique_ptr<Actor> obstacleManager = std::make_unique<ObstacleManager>(0);
	obstacleManager->Initialize();
	//�V�[���O���t�ɏ�Q���}�l�[�W���[���A�^�b�`����
	m_sceneGraph->AttachNode(std::move(obstacleManager));

	//�L�[�}�l�[�W���[�쐬
	std::unique_ptr<Actor> keyManager = std::make_unique<KeyManager>(0);
	keyManager->Initialize();
	//�V�[���O���t�ɃL�[�}�l�[�W���[���A�^�b�`����
	m_sceneGraph->AttachNode(std::move(keyManager));

	//�t�F�[�h�̍쐬
	m_fade = std::make_unique<Fade>();
	//shader�쐬
	m_fade->Create();
	//������
	m_fade->Initialize(DirectX::SimpleMath::Vector3::Zero);
	//�ŏ��Ƀt�F�[�h�C������
	m_fade->FadeIn();
}

/*--------------------------------------------------
�X�V
--------------------------------------------------*/
void PlayScene::Update(const DX::StepTimer& timer)
{
	//�t�F�[�h�X�V
	m_fade->Update(timer);

	//�t�F�[�h�C�����Ă��Ȃ��ꍇ���v���C���[���A�N�e�B�u�ł���ꍇ����ȍ~�������Ȃ�
	if (!m_fade->ISOpen() && m_actor->IsActive())
		return ;

	//�V�[���O���t�X�V
	m_sceneGraph->Update(timer);

	//�J�����X�V
	m_camera->Update();

	//�v���C���[�����S���Ă���ꍇ���U���g�V�[���Ɉȍ~
	if (!m_actor->IsActive())
	{
		//�t�F�[�h�A�E�g����
		m_fade->FadeOut();


		//�t�F�[�h�A�E�g���I������烊�U���g�ֈȍ~
		if(m_fade->ISClose())
			m_parent->ChengeScene(m_parent->GetResultScene());
	}
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();


	//�V�[���O���t�`��
	m_sceneGraph->Render(m_camera.get());


	//�t�F�[�h�`��
	m_fade->Render();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void PlayScene::Finalize()
{
}
