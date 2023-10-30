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


//�A���t�@�̍ő�l
const float ResultScene::ALPHA_MAX_NUM = 1.0f;
//�A���t�@�̍ŏ��l
const float ResultScene::ALPHA_MIN_NUM = 0.0f;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
ResultScene::ResultScene(GameMain* parent)
	:
	m_parent{parent},
	m_commonState{},
	m_fade{},
	m_spriteBatch{},
	m_spriteFont{},
	m_alpha{1.0f},
	m_alphaSpeed{-0.01f}
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
void ResultScene::Update(const DX::StepTimer& timer)
{
	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	// �}�E�X���͏����擾����
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	//�t�F�[�h�X�V
	m_fade->Update(timer);

	//�A���t�@���A���t�@�X�s�[�h�ő���
	m_alpha += m_alphaSpeed;

	//�A���t�@���O���P�ɂȂ�����A���t�@�X�s�[�h�𔽓]������
	if (m_alpha < ALPHA_MIN_NUM || m_alpha > ALPHA_MAX_NUM)
	{
		//�A���t�@�X�s�[�h�𔽓]������
		m_alphaSpeed *= -1.0f;
	}

	//�t�F�[�h�A�E�g���邩
	if (m_fade->ISOpen() && keyState.Space)
	{
		m_fade->FadeOut();		
	}

	//�t�F�[�h�A�E�g���Ă����Ԃł���΃^�C�g���V�[���Ɉڍs
	if (m_fade->ISClose())
		m_parent->ChengeScene(m_parent->GetTitleScene());

}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void ResultScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	
	//�摜�`��J�n
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());
	//�e�N�X�`���}�l�[�W���[�擾
	TextureManager& textureManager = TextureManager::GetInstance();
	//�摜�̃t�@�C����
	std::wstring tex[] =
	{
		L"youdead.png",
		L"clear.png",
	};
	//�N���A���Ă��邩�擾
	int isclear = GameContext::GetInstance().ISClear();
	//�摜��ǂݍ���
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> clearTexture = textureManager.LoadTexture(tex[isclear]);
	//�摜�T�C�Y�̎擾
	DirectX::SimpleMath::Vector2 texSize = textureManager.GetTextureSize(tex[isclear]);


	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();
	DirectX::SimpleMath::Vector2 windowCenterPosition(size.right / 2.0f, size.bottom / 2.0f);
	
	//�摜�`��
	m_spriteBatch->Draw(clearTexture.Get(), windowCenterPosition, nullptr, DirectX::Colors::White, 0.0f, texSize/2.0f,2.0f);
	
	//PUSH_SPASE_KEY�̕\�����W
	DirectX::SimpleMath::Vector2 pushSpeseKeyPos = DirectX::SimpleMath::Vector2::Zero;
	//PUSH_SPASE_KEY�̐F
	DirectX::SimpleMath::Vector4 pushColor{ 1.0f,1.0f,1.0f,m_alpha };
	//PUSH_SPASE_KEY�̕\��
	m_spriteBatch->Draw(textureManager.LoadTexture(L"PushSpaceKey.png").Get(), pushSpeseKeyPos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	//�摜�`��I��
	m_spriteBatch->End();

	//�t�F�[�h�`��
	m_fade->Render();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void ResultScene::Finalize()
{
}
