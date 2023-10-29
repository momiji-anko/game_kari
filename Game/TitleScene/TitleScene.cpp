/*
�^�C�g���V�[��
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"
#include"Game/GameContext/GameContext.h"
#include "TitleScene.h"
#include"Libraries/MyLibraries/Camera.h"



/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
TitleScene::TitleScene(GameMain* parent)
	:
	m_parent{parent},
	m_stageSelect{},
	m_cameraAngle{0},
	m_previousStageSelectNumber{0}
{
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
TitleScene::~TitleScene()
{
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void TitleScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//�L�[�{�[�h�X�e�[�g�g���b�J�[�擾
	m_keyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	GameContext::GetInstance().SetKeyboardStateTracker(m_keyboardStateTracker.get());

	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);
	GameContext::GetInstance().SetCommonState(m_commonState.get());

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	
	//GameContext�ɃX�v���C�g�o�b�`��o�^
	GameContext::GetInstance().SetSpriteBatch(m_spriteBatch.get());

	//�X�e�[�W�Z���N�g�쐬
	m_stageSelect = std::make_unique<StageSelect>();
	m_stageSelect->Initialize();

	//�J�����쐬
	m_camera = std::make_unique<Camera>();
	
	//�X�e�[�W�}�l�[�W���[�쐬
	m_stageManager = std::make_unique<StageManager>(m_previousStageSelectNumber);
	m_stageManager->Initialize();

	//�t�F�[�h�̍쐬
	m_fade = std::make_unique<Fade>();
	//shader�쐬
	m_fade->Create();
	//������
	m_fade->Initialize(DirectX::SimpleMath::Vector3::Zero);
	//�ŏ��Ƀt�F�[�h�C������
	m_fade->FadeIn();

	//�^�C�g�������쐬
	m_titleText = std::make_unique<TitleText>();
	m_titleText->Initialize();

}

/*--------------------------------------------------
�X�V
--------------------------------------------------*/
void TitleScene::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	// �}�E�X���͏����擾����
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	m_fade->Update(timer);

	//�t�F�[�h�A�E�g���Ă����Ԃł���΃v���C�V�[���ڍs
	if (m_fade->ISClose())
		m_parent->ChengeScene(m_parent->GetPlayScene());

	//�t�F�[�h�C�����Ă��Ȃ��ꍇ����ȍ~�������Ȃ�
	if (!m_fade->ISOpen())
		return;


	//�^�C�g�������̓���
	m_titleText->Update(timer);

	//�I���X�e�[�W���ς����ꍇ�A�w�i�̃X�e�[�W��ς���
	if (m_previousStageSelectNumber != m_stageSelect->GetSelectStageNum())
	{
		//�I�����Ă����X�e�[�W�̔ԍ����X�V
		m_previousStageSelectNumber = m_stageSelect->GetSelectStageNum();

		//�X�e�[�W�}�l�[�W���[�̍Đ���
		m_stageManager.reset(new StageManager(m_previousStageSelectNumber));
		m_stageManager->Initialize();
	}

	//�X�e�[�W�}�l�[�W���[�X�V
	m_stageManager->Update(timer);

	//�J��������]������
	m_cameraAngle += 0.001f;

	//�X�e�[�W���I�΂ꂽ��t�F�[�h�A�E�g����
	if (m_stageSelect->Update(timer))
	{
		m_fade->FadeOut();		
	}
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void TitleScene::Draw()
{
	//�摜�`��J�n
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//�X�e�[�W�I��`��
	m_stageSelect->Draw();

	// �r���[�s��
	DirectX::SimpleMath::Matrix view;
	// �v���W�F�N�V�����s��
	// �J�������W
	DirectX::SimpleMath::Vector3 eye = { cos(m_cameraAngle) * 180,100,-sin(m_cameraAngle) * 180 };
	// �����_
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3::Zero;
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;
	// �r���[�s��v�Z
	view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
	// �r���[�ݒ�
	m_camera->SetViewMatrix(view);
	
	//�X�e�[�W�}�l�[�W���[�`��
	m_stageManager->Render(m_camera.get());
	
	//�^�C�g�������`��
	m_titleText->Draw();


	//�摜�`��I��
	m_spriteBatch->End();

	//�t�F�[�h�`��
	m_fade->Render();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void TitleScene::Finalize()
{
}
