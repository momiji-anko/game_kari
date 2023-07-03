
#include"pch.h"
#include "Actor.h"
#include"DeviceResources.h"
#include"Game/GameContext/GameContext.h"
#include"Game/Shader/ShadowMap.h"

Actor::Actor()
	:
	m_position(DirectX::SimpleMath::Vector3::Zero),
	m_velocity(DirectX::SimpleMath::Vector3::Zero),
	m_model(nullptr),
	m_active(true),
	m_AABBObject(nullptr),
	m_capsule(nullptr),
	m_rotation{},
	m_world(DirectX::SimpleMath::Matrix::Identity)
{
	CreateAABB();
	CreateCapsule();

}

/// <summary>
/// ������	
/// </summary>
/// <param name="position">���W</param>
/// <param name="velocity">�ړ���</param>
/// <param name="scale">�g�k</param>
/// <param name="rotation">��]</param>
/// <param name="model">���f��</param>
/// <param name="active">�A�N�e�B�u</param>
void Actor::Initialize(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model, bool active)
{
	//�p�����[�^��������
	//���W
	m_position = position;
	//�ړ���
	m_velocity = velocity;
	//�g�k
	m_scale = scale;
	//��]
	m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.z, rotation.x);
	//���f��
	m_model = model;
	//�A�N�e�B�u
	m_active = active;


}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Actor::Update(const DX::StepTimer& timer)
{
	//�x������
	UNREFERENCED_PARAMETER(timer);
	
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void Actor::Render(const Camera* camera)
{
	//�x������
	UNREFERENCED_PARAMETER(camera);
	
}

/// <summary>
/// �I������
/// </summary>
void Actor::Finalize()
{

}

/// <summary>
/// ���[���h�s��v�Z
/// </summary>
void Actor::CalculateWorldMatrix()
{
	//���[���h�s��ɒP�ʍs�������
	m_world = DirectX::SimpleMath::Matrix::Identity;
	//�ړ�
	DirectX::SimpleMath::Matrix translation = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	//��]
	DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation);
	//�g�k
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	//���[���h�s��̌v�Z
	m_world *= scale * rotation * translation;

}

/// <summary>
/// ���Z�b�g
/// </summary>
void Actor::Reset()
{
}

/// <summary>
/// �e����
/// </summary>
/// <param name="shadow">�V���h�E�}�b�v�̐��|�C���^</param>
/// <param name="view">�r���[�s��</param>
/// <param name="projection">�v���W�F�N�V�����s��</param>
void Actor::CreateShadow(ShadowMap* shadow, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//�f�o�C�X���\�[�X�擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//���f��������Ήe�𐶐�����
	if (GetModel() != nullptr)
	{
		//���[���h�s����v�Z����
		CalculateWorldMatrix();

		//�e����
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), view, projection, false, [&]()
			{
				shadow->DrawShadowMap(context);
			}
		);
	}

}

/// <summary>
/// AABB�̍쐬
/// </summary>
void Actor::CreateAABB()
{
	//AABB�����蔻��쐬
	m_AABBObject = std::make_unique<AABBFor3D>();
	//AABB������
	m_AABBObject->Initialize();

	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	//AABB�̓����蔻���ݒ�
	m_AABBObject->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);


}

/// <summary>
/// �J�v�Z���̐���
/// </summary>
void Actor::CreateCapsule()
{
	//�J�v�Z���̍쐬
	m_capsule = std::make_unique<Capsule>();
	//���a�ݒ�
	m_capsule->r = 1.0f;
}

