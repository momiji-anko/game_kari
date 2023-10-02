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


using namespace DirectX;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
TitleScene::TitleScene(GameMain* parent)
	:
	m_parent{ parent },
	m_stageSelect{},
	m_cameraAngle{ 0 }
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

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	// �e�N�X�`���̓ǂݍ���
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/TridentLogo.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);

	GameContext::GetInstance().SetSpriteBath(m_spriteBatch.get());

	m_stageSelect = std::make_unique<StageSelect>();
	m_stageSelect->Initialize();

	m_camera = std::make_unique<Camera>();
	


}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
void TitleScene::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	// �}�E�X���͏����擾����
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();


	if (m_previousNumber != m_stageSelect->GetSelectStageNum())
	{
		m_previousNumber = m_stageSelect->GetSelectStageNum();

		m_stageManager.reset(new StageManager(m_previousNumber));
		m_stageManager->Initialize();
	}

	m_stageManager->Update(timer);

	m_cameraAngle += 0.1f;

	if (m_stageSelect->Update(timer))
	{
		m_parent->ChengeScene(m_parent->GetPlayScene());
	}

}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void TitleScene::Draw()
{
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	SimpleMath::Vector2 pos(640 - 128, 360 - 128);

	m_spriteBatch->Draw(m_texture.Get(), pos);

	m_spriteFont->DrawString(m_spriteBatch.get(), L"Title Scene", DirectX::XMFLOAT2(10, 10));

	m_stageSelect->Draw();


	// �r���[�s��
	DirectX::SimpleMath::Matrix view;
	// �v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix projection;
	// �J�������W
	DirectX::SimpleMath::Vector3 eye = { cos(m_cameraAngle) * 18,10,-sin(m_cameraAngle) * 18 };
	// �����_
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3::Zero;
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;
	// �r���[�s��v�Z
	view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
	m_camera->SetViewMatrix(view);

	m_stageManager->Render(m_camera.get());
	m_spriteBatch->End();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void TitleScene::Finalize()
{
}
