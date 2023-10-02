//
// Scene.cpp
//
#include "pch.h"

#include "DeviceResources.h"
#include "GameMain.h"

#include "Libraries/MyLibraries/DebugCamera.h"
#include "Libraries/MyLibraries/GridFloor.h"

#include"Game/PlayScene/PlayScene.h"
#include"Game/TitleScene/TitleScene.h"
#include"Game/ResultScene/ResultScene.h"

// ���O��Ԃ̗��p
using namespace DirectX;

//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
GameMain::GameMain()
	:
	m_nextScene{ GAME_SCENE::TITLE },		// �����V�[���̐ݒ�
	m_pScene{ nullptr },
	m_playScane{},
	m_titleScane{},
	m_resultScene{}
{
}

//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
GameMain::~GameMain()
{
	Finalize();
}

//-------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------
void GameMain::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �L�[�{�[�h�֘A
	m_keybord = std::make_unique<DirectX::Keyboard>();

	// �}�E�X�֘A
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(pDR->GetHwnd());

	m_playScane = std::make_unique<PlayScene>(this);
	m_titleScane = std::make_unique<TitleScene>(this);
	m_resultScene = std::make_unique<ResultScene>(this);

	
	ChengeScene(m_playScane.get());
}

//-------------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------------
void GameMain::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = m_keybord->GetState();

	// ESC�L�[�ŏI��
	if (keyState.Escape)
	{
		PostQuitMessage(0);
	}


	// ���Ԃ�����΍X�V
	if (m_pScene != nullptr)
	{
		m_pScene->Update(timer);
	}
}

//-------------------------------------------------------------------
// �`�揈��
//-------------------------------------------------------------------
void GameMain::Render()
{
	// ���Ԃ�����Ε`��
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}
}

//-------------------------------------------------------------------
// ��n��
//-------------------------------------------------------------------
void GameMain::Finalize()
{


}

void GameMain::ChengeScene(IScene* scene)
{
	if (m_pScene != nullptr)
		m_pScene->Finalize();

	m_pScene = scene;

	m_pScene->Initialize();
}

IScene* GameMain::GetPlayScene()
{
	return m_playScane.get();
}

IScene* GameMain::GetTitleScene()
{
	return m_titleScane.get();
}

IScene* GameMain::GetResultScene()
{
	return m_resultScene.get();
}
