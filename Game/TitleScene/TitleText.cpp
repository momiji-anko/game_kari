#include"pch.h"
#include"TitleText.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/GameContext/GameContext.h"
//�^�C�g���̈ړ�����
const float TitleText::MOVE_TIME = 3.0f;
//�^�C�g���̈ړ��p�x
const float TitleText::MOVE_RORATION = 0.1f;
//�^�C�g���̏����p�x
const float TitleText::RORATION_ZERO = 0.0f;
//�^�C�g���̉�]���[�`���̃��Z�b�g
const int TitleText::ROTATION_RESET_ROUTINE = 2;
//�J�����̉�]���x
const float TitleText::CAMERA_ROT_SPEED = 0.001f;
//�A���t�@�̍ő�l
const float TitleText::MOVED_TEXTURE_ALPHA = 1.0f;
//�A���t�@�̍ŏ��l
const float TitleText::MOVEING_TEXTURE_ALPHA = 0.7f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
TitleText::TitleText()
	:
	m_titleTextureFilePath{ L"Resources/Textures/Title.png" },
	m_titileTexture{},
	m_titleTimer_s{ 0.0f },
	m_titleRoutine{ 0 },
	m_titlePosition{ DirectX::SimpleMath::Vector2(-1000.f,-1000.f),DirectX::SimpleMath::Vector2(-1000.f,-1000.f) },
	m_titleRotetion{ 0.0f },
	m_titleAlpha{ 0.0f }
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TitleText::~TitleText()
{
	m_titileTexture.Reset();
}

/// <summary>
/// ������
/// </summary>
void TitleText::Initialize()
{
	//�^�C�g���摜�̓ǂݍ���
	m_titileTexture = TextureManager::GetInstance().LoadTexture(m_titleTextureFilePath.c_str());
	//�^�C�g���A���t�@�l�̏�����
	m_titleAlpha = MOVEING_TEXTURE_ALPHA;

}


/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void TitleText::Update(const DX::StepTimer& timer)
{

	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();

	//�o�ߎ���
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//�^�C�g���^�C�}�[���o�ߎ��Ԃő���
	m_titleTimer_s += elapsedTime_s;

	//�^�C�g���̈ړ��O�̍��W
	DirectX::SimpleMath::Vector2 titleStartPosition = DirectX::SimpleMath::Vector2(static_cast<float>(size.right*2), static_cast<float>(size.bottom / 3));
	//�^�C�g���̈ړ���̍��W
	DirectX::SimpleMath::Vector2 titleMovedPosition = DirectX::SimpleMath::Vector2(static_cast<float>(size.right / 2), static_cast<float>(size.bottom / 3));


	switch (m_titleRoutine)
	{
	case 0:
		//���[�v�ňړ�
		m_titlePosition[0] = DirectX::SimpleMath::Vector2::Lerp(DirectX::SimpleMath::Vector2(-titleStartPosition.x, titleStartPosition.y), titleMovedPosition, easeOutCubic(m_titleTimer_s / MOVE_TIME));
		m_titlePosition[1] = DirectX::SimpleMath::Vector2::Lerp(titleStartPosition, titleMovedPosition, easeOutCubic(m_titleTimer_s / MOVE_TIME));
		break;
	case 1:
		//�^�C�}�[������
		m_titleTimer_s = 0;
		m_titleRoutine++;
		//�A���t�@�l���P�ɂ���
		m_titleAlpha = MOVED_TEXTURE_ALPHA;
	case 2:
		//�^�C�g�������̉�]
		m_titleRotetion = Lerp(RORATION_ZERO, MOVE_RORATION, m_titleTimer_s / MOVE_TIME);
		break;
	case 3:
		//�^�C�}�[������
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 4:

		//�^�C�g�������̉�]
		m_titleRotetion = Lerp(MOVE_RORATION, RORATION_ZERO, m_titleTimer_s / MOVE_TIME);

		break;
	case 5:
		//�^�C�}�[������
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 6:

		//�^�C�g�������̉�]
		m_titleRotetion = Lerp(RORATION_ZERO, -MOVE_RORATION, m_titleTimer_s / MOVE_TIME);

		break;
	case 7:
		m_titleTimer_s = 0;
		m_titleRoutine++;
	case 8:

		//�^�C�g�������̉�]
		m_titleRotetion = Lerp(-MOVE_RORATION, RORATION_ZERO, m_titleTimer_s / MOVE_TIME);

		break;
	case 9:
		m_titleTimer_s = 0;
		//��]���J��Ԃ�
		m_titleRoutine = ROTATION_RESET_ROUTINE;
	default:
		break;
	}

	//�ړ����I������玟�ɍs��
	if (m_titleTimer_s > MOVE_TIME)
	{
		m_titleTimer_s = MOVE_TIME;
		m_titleRoutine++;
	}


}


/// <summary>
/// �`��
/// </summary>
void TitleText::Draw()
{
	//�^�C�g����ʂ̃T�C�Y�擾
	DirectX::SimpleMath::Vector2 textureSize = TextureManager::GetInstance().GetTextureSize(m_titleTextureFilePath);


	//�^�C�g�������`��
	for (DirectX::SimpleMath::Vector2& position : m_titlePosition)
	{

		GameContext::GetInstance().GetSpriteBatch()->Draw(m_titileTexture.Get(), position, nullptr, DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_titleAlpha), m_titleRotetion, textureSize / 2.0f, 2.0f);
		
	}

}

/// <summary>
/// �I������
/// </summary>
void TitleText::Finalize()
{
}
