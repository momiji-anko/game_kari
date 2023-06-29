
#include "Actor.h"

#include"pch.h"

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
}

/// <summary>
/// ������	
/// </summary>
/// <param name="position">���W</param>
/// <param name="velcity">�ړ���</param>
/// <param name="scale">�g�k</param>
/// <param name="rotataion">�X�P�[��</param>
/// <param name="model">���f��</param>
/// <param name="active">�A�N�e�B�u</param>
void Actor::Initialize(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velcity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotataion, DirectX::Model* model, bool active)
{
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Actor::Update(const DX::StepTimer& timer)
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
}
