/*
�v���C�V�[��
*/
#include "pch.h"

#include "DeviceResources.h"
#include "../GameMain.h"
#include<windows.h>
#include "PlayScene.h"

// ���O��Ԃ̗��p
using namespace DirectX;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
PlayScene::PlayScene()
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
}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void PlayScene::Finalize()
{
}
