#include"pch.h"
#include"Key.h"

#include"Game/GameContext/GameContext.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/Camera.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">���W</param>
/// <param name="velocity">�ړ���</param>
/// <param name="scale">�g�k</param>
/// <param name="rotation">�X�P�[��</param>
/// <param name="model">���f��</param>
/// <param name="active">�A�N�e�B�u</param>
Key::Key(
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& velocity,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Vector3& rotation,
	DirectX::Model* model,
	bool active
)
	:
	Actor{ position, velocity, scale, rotation, model, active }
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Key::~Key()
{
}

/// <summary>
/// ������	
/// </summary>
void Key::Initialize()
{
	//AABB�����蔻��̎擾
	AABBFor3D* aabb = GetAABB();
	//���݂̍��W�擾
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//�����蔻��̑傫��
	DirectX::SimpleMath::Vector3 area{ 0.5 };
	//�����蔻��ݒ�
	aabb->SetData(position - area, position + area);
	

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Key::Update(const DX::StepTimer& timer)
{
	//�A�N�e�B�u�łȂ���Ώ������Ȃ�
	if (!IsActive())
		return;

	SetActive(!GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayer2Keys(GetAABB()));

}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void Key::Render(const Camera* camera)
{
	//�A�N�e�B�u�łȂ���Ώ������Ȃ�
	if (!IsActive())
		return;

	//�f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	CalculateWorldMatrix();

	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
}

/// <summary>
/// �I������
/// </summary>
void Key::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void Key::Reset()
{
}
