/*
���[�h���
*/

#include "pch.h"

#include "LoadingScreen.h"

#include "DeviceResources.h"
#include "../IScene.h"

#include <WICTextureLoader.h>
#include"Libraries/MyLibraries/TextureManager.h"
#include<WICTextureLoader.h>

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
LoadingScreen::LoadingScreen()
	:
	m_texPosition{ 0.0f,0.0f }
{
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
LoadingScreen::~LoadingScreen()
{
	if (m_loadingThread.joinable())
	{
		m_loadingThread.join();
	}
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void LoadingScreen::Initialize(IScene* targetScene)
{
	// �ΏۃV�[���̃��\�[�X�̓ǂݍ���
	LoadSceneResources(targetScene);


	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");
	

	

	//�e�N�X�`���̓ǂݍ���
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/nowLoading.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);



	// �e�N�X�`���̓ǂݍ���
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/black.png",
		nullptr,
		m_blackTexture.ReleaseAndGetAddressOf()
	);

	RECT rect = pDR->GetOutputSize();

	m_texPosition = DirectX::SimpleMath::Vector2(static_cast<float>(rect.right), static_cast<float>(rect.bottom) - 120.0f);

	m_texturePositions.resize(10);

	for (int i=0 ; i<m_texturePositions.size();i++)
	{
		m_texturePositions[i] = DirectX::SimpleMath::Vector2(m_texPosition.x - (i * 690.0f), m_texPosition.y);
	}
}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
void LoadingScreen::Update(const DX::StepTimer& timer)
{
	UNREFERENCED_PARAMETER(timer);

	for (DirectX::SimpleMath::Vector2& texPos : m_texturePositions)
	{
		texPos.x -= 5.0f;

		if (texPos.x < -780.0f)
		{
			texPos.x = m_texPosition.x;
		}
	}
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void LoadingScreen::Draw()
{
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	m_spriteFont->DrawString(m_spriteBatch.get(), L"Load Screen", DirectX::XMFLOAT2(10, 10));
	DirectX::SimpleMath::Vector2 blackTexture(0, 0);
	m_spriteBatch->Draw(m_blackTexture.Get(), blackTexture, nullptr, DirectX::SimpleMath::Vector4::One, 0.0f, DirectX::SimpleMath::Vector2::Zero);
	DirectX::SimpleMath::Vector2 pos(640 - 128, 360 - 128);
	

	for (DirectX::SimpleMath::Vector2& texPos : m_texturePositions)
	{
		m_spriteBatch->Draw(m_texture.Get(), texPos);
	}

	m_spriteBatch->End();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void LoadingScreen::Finalize()
{
}

/*--------------------------------------------------
	// �ǂݍ��ݒ����ǂ����̎擾
--------------------------------------------------*/
bool LoadingScreen::IsLoading()
{
 return m_isLoading;
}


/*--------------------------------------------------
�V�[���̃��\�[�X�̓ǂݍ���
--------------------------------------------------*/
void LoadingScreen::LoadSceneResources(IScene* targetScene)
{
	if (m_loadingThread.joinable())
	{
		m_loadingThread.join();
	}
	m_isLoading = true;
	m_loadingThread = std::thread(
		[=]
		{
			targetScene->LoadResources();
			m_isLoading = false;
		});
}
