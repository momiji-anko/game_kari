#include"pch.h"
#include"ClearKey.h"

#include"Game/GameContext/GameContext.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Game/GameObject/Enemy/EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">���W</param>
/// <param name="velocity">�ړ���</param>
/// <param name="scale">�g�k</param>
/// <param name="rotation">�X�P�[��</param>
/// <param name="model">���f��</param>
/// <param name="active">�A�N�e�B�u</param>
ClearKey::ClearKey(
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
ClearKey::~ClearKey()
{
}

/// <summary>
/// ������	
/// </summary>
void ClearKey::Initialize()
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
void ClearKey::Update(const DX::StepTimer& timer)
{
	//�A�N�e�B�u�łȂ���Ώ������Ȃ�
	if (!IsActive())
		return;
	//�v���C���[�ɓ������Ă��邩
	bool isCollision = GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayer2Keys(GetAABB());

	//�������Ă����Ԃł���Δ�A�N�e�B�u�ɂȂ茮������Ă��邱�ƂɂȂ�
	SetActive(!isCollision);


	if (isCollision)
	{
		GameContext::GetInstance().GetEnemyManager() ->CreatePlayerTrackingEnemy();
	}

}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void ClearKey::Render(const Camera* camera)
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
void ClearKey::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void ClearKey::Reset()
{
}
