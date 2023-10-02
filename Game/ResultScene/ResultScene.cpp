/*
�^�C�g���V�[��
*/

#include"pch.h"

#include<WICTextureLoader.h>
#include<SimpleMath.h>

#include"../GameMain.h"
#include"DeviceResources.h"

#include"ResultScene.h"
#include"Game/GameMain.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/GameContext/GameContext.h"

using namespace DirectX;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
ResultScene::ResultScene(GameMain* parent)
	:
	m_parent(parent)
{
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
ResultScene::~ResultScene()
{
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void ResultScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


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

}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
void ResultScene::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	// �}�E�X���͏����擾����
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	if (keyState.Z || mouseState.rightButton)
	{
		m_parent->ChengeScene(m_parent->GetTitleScene());
	}

}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void ResultScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	TextureManager& textureManager = TextureManager::GetInstance();

	std::wstring tex[] =
	{
		L"youdead.png",
		L"clear.png",
	};
	
	int isclear = GameContext::GetInstance().ISClear();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> clearTexture = textureManager.LoadTexture(tex[isclear]);
	DirectX::SimpleMath::Vector2 texSize = textureManager.GetTextureSize(tex[isclear]);


	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();
	DirectX::SimpleMath::Vector2 windowCenterPosition(size.right / 2.0f, size.bottom / 2.0f);

	SimpleMath::Vector2 pos(640 - 128, 360 - 128);

	m_spriteBatch->Draw(clearTexture.Get(), windowCenterPosition, nullptr, DirectX::Colors::White, 0.0f, texSize/2.0f);

	m_spriteFont->DrawString(m_spriteBatch.get(), L"Result Scene", DirectX::XMFLOAT2(10, 10));

	m_spriteBatch->End();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void ResultScene::Finalize()
{
}
