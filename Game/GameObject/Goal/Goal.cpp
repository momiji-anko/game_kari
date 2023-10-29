#include"pch.h"
#include"Goal.h"
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
Goal::Goal(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model, bool active)
	:
	Actor(position, velocity, scale, rotation, model, active)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Goal::~Goal()
{
}

/// <summary>
/// ������	
/// </summary>
void Goal::Initialize()
{
	//�R���W�����}�l�[�W���[�����݂��Ă���΃S�[���̓����蔻�������
	if(GameContext::GetInstance().GetCollisionManager()!=nullptr)
		GameContext::GetInstance().GetCollisionManager()->SetGoalAABB(nullptr);

	//�����蔻���ݒ�
	//AABB�����蔻��擾
	AABBFor3D* aabb = GetAABB();
	//���W�擾
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//�����蔻��G���A�̑傫��
	DirectX::SimpleMath::Vector3 area{ 1.f };
	//�G���A�̐ݒ�
	aabb->SetData(position - area, position + area);

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Goal::Update(const DX::StepTimer& timer)
{
	//�������ׂĂ̎���Ă��邩�R���W�����}�l�[�W���[�����݂��Ă���ꍇ�S�[���̓����蔻���ǉ�����
	if(GameContext::GetInstance().GetIsAllGetKey() && GameContext::GetInstance().GetCollisionManager() != nullptr)
		GameContext::GetInstance().GetCollisionManager()->SetGoalAABB(GetAABB());

	
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void Goal::Render(const Camera* camera)
{

	//�f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//���[���h�s��v�Z
	CalculateWorldMatrix();

	//���f���`��
	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

}

/// <summary>
/// �I������
/// </summary>
void Goal::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void Goal::Reset()
{
}
