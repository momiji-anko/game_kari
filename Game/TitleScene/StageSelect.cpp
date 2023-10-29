
#include"pch.h"
#include"StageSelect.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"DeviceResources.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/MemoryLeakDetector.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
StageSelect::StageSelect()
	:
	m_texturePosition{ DirectX::SimpleMath::Vector2::Zero },
	m_selectStageNum{ 0 },
	m_stageMaxNum{ 0 }
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
StageSelect::~StageSelect()
{
	
}

/// <summary>
/// ������
/// </summary>
void StageSelect::Initialize()
{

	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();

	//�e�N�X�`���}�l�[�W���[�̎擾
	TextureManager& textureManager = TextureManager::GetInstance();

	//�e�N�X�`���̓ǂݍ���
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageTextTexture = textureManager.LoadTexture(L"stageText.png");
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageNumTexture = textureManager.LoadTexture(L"stageNum.png");
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageArrowTexture = textureManager.LoadTexture(L"Arrow.png");


	//�X�e�[�W�摜��Y���W
	float stageSelectPosY = static_cast<float>(size.bottom / 2.0f + size.bottom / 4.0);
	//�X�e�[�W�摜��X���W
	float stagePositionX = static_cast<float>(size.right / 2.0f);
	//�X�e�[�W�摜�̍��W�ݒ�
	m_texturePosition = { stagePositionX,stageSelectPosY };

	//�t�@�C���}�l�[�W���[�̎擾
	FileLoadManager& fileLoadManager = FileLoadManager::GetInstance();
	//�t�@�C���̐��擾
	m_stageMaxNum = static_cast<int>(fileLoadManager.LoadFile(L"Resources/Stage/Json/").size());


}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
/// <returns>���̑I����ʂɍs����</returns>
bool StageSelect::Update(const DX::StepTimer& timer)
{
	UNREFERENCED_PARAMETER(timer);

	//�Q�[���R���e�L�X�g�擾
	GameContext& gameContext = GameContext::GetInstance();
	//�Q�[���R���e�L�X�g����L�[�{�[�h�X�e�[�g�g���b�J�[�̎擾
	DirectX::Keyboard::KeyboardStateTracker* keyboard = gameContext.GetKeyboardStateTracker();

	//�{�^������������
	bool isKeyPush = false;

	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	//�L�[�{�[�h�X�e�[�g�g���b�J�[�̍X�V
	keyboard->Update(keyState);


	//�X�e�[�W�I��
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		//��������true�ɂ���
		isKeyPush = true;

		//�X�e�[�W�̔ԍ������炷
		m_selectStageNum -= 1;

	}
	else if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		//��������true�ɂ���
		isKeyPush = true;

		//�X�e�[�W�̔ԍ��𑝂₷
		m_selectStageNum += 1;
	}


	//��������X�e�[�W�}�l�[�W���[�̃X�e�[�W��ς���
	if (isKeyPush)
	{
		//�͈̓`�F�b�N
		m_selectStageNum = std::min(std::max(0, m_selectStageNum), m_stageMaxNum - 1);
	}

	//�X�y�[�X�L�[�������Ǝ��̑I����ʂɍs���Ɉړ�
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{

		return true;
	}

	return false;
}

/// <summary>
/// �`��
/// </summary>
void StageSelect::Draw()
{
	//�X�e�[�W�I����UI�\��
	StageSelectUIDraw();
}

/// <summary>
/// �I������
/// </summary>
void StageSelect::Finalize()
{

}

/// <summary>
/// �X�e�[�W�I����UI�\��
/// </summary>
void StageSelect::StageSelectUIDraw()
{
	//�e�N�X�`���}�l�[�W���[�̎擾
	TextureManager& textureManager = TextureManager::GetInstance();

	//�e�N�X�`���̓ǂݍ���
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageTextTexture = textureManager.LoadTexture(L"stageText.png");
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageNumTexture = textureManager.LoadTexture(L"stageNum.png");
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageArrowTexture = textureManager.LoadTexture(L"Arrow.png");

	//�X�e�[�W�ԍ��e�N�X�`���̉摜�T�C�Y���擾
	DirectX::SimpleMath::Vector2 stageNumTextureSize = textureManager.GetTextureSize(L"stageNum.png");
	//�X�e�[�W�e�L�X�g�e�N�X�`���̉摜�T�C�Y���擾
	DirectX::SimpleMath::Vector2 stageTextTextureSize = textureManager.GetTextureSize(L"stageText.png");
	//�A���[�e�N�X�`���̉摜�T�C�Y���擾
	DirectX::SimpleMath::Vector2 stageArrowTextureSize = textureManager.GetTextureSize(L"Arrow.png");

	//�Q�[���R���e�L�X�g�擾
	GameContext& gameContext = GameContext::GetInstance();
	//�Q�[���R���e�L�X�g����X�v���C�g�o�b�`�擾
	DirectX::SpriteBatch* spriteBatch = gameContext.GetSpriteBatch();

	//�����̈�}�X�̃T�C�Y
	float stageNumSize = (stageNumTextureSize.x) / 10;

	//�؂���ʒu
	RECT texRect =
	{
		static_cast<LONG>(stageNumSize * m_selectStageNum + 1),
		static_cast <LONG>(0),
		static_cast<LONG>(stageNumSize * (m_selectStageNum + 1) - (stageNumSize / 5)),
		static_cast<LONG>(stageNumTextureSize.y)
	};

	//�g�嗦
	float textureScale = 1.5f;

	//�J���[
	DirectX::SimpleMath::Vector4 color = DirectX::Colors::Red;

	//�X�e�[�W�����̕`��
	spriteBatch->Draw(stageTextTexture.Get(), m_texturePosition, nullptr, color, 0.0f, stageTextTextureSize / 2.0f, textureScale);

	//�����̈ʒu
	DirectX::SimpleMath::Vector2 numPosition = m_texturePosition + DirectX::SimpleMath::Vector2(stageTextTextureSize.x + stageNumTextureSize.x / 10, 0);

	//�����̕`��
	spriteBatch->Draw(stageNumTexture.Get(), numPosition, &texRect, color, 0.0f, DirectX::SimpleMath::Vector2(stageNumSize, stageNumTextureSize.y / 2.0f), textureScale);

	//�I�����Ă���X�e�[�W�ԍ����P��荂����΍����\��
	if (m_selectStageNum > 0)
	{
		spriteBatch->Draw(stageArrowTexture.Get(), m_texturePosition - DirectX::SimpleMath::Vector2(stageArrowTextureSize.x + stageTextTextureSize.x, 0), nullptr, DirectX::Colors::White, DirectX::XM_PI, stageArrowTextureSize / 2, textureScale);
	}

	//�I�����Ă���X�e�[�W�ԍ����X�e�[�W�̍ő吔���Ⴍ�����
	if (m_selectStageNum < m_stageMaxNum - 1)
	{
		spriteBatch->Draw(stageArrowTexture.Get(), numPosition + DirectX::SimpleMath::Vector2(stageArrowTextureSize.x, 0), nullptr, DirectX::Colors::White, 0.0f, stageArrowTextureSize / 2, textureScale);
	}
}

