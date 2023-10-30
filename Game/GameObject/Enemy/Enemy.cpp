#include"pch.h"

#include "Enemy.h"
#include"DeviceResources.h"

#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
#include<Effects.h>

//���G�͈͂̑傫��
const float Enemy::ENEMY_SPEHERE_RADIUS = 20.0f;
//�c�̐����̒���
const float Enemy:: ENEMY_HEGHT_COLLISITION_LINE = 1.5f;
//�ړ����x
const float Enemy::MOVE_SPEED = 3.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">���W</param>
/// <param name="velocity">�ړ���</param>
/// <param name="scale">�g�k</param>
/// <param name="rotation">�X�P�[��</param>
/// <param name="model">���f��</param>
/// <param name="active">�A�N�e�B�u</param>
Enemy::Enemy(
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& velocity, 
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Vector3& rotation, 
	DirectX::Model* model,
	bool active	
)
	:
	Actor(position, velocity, scale, rotation, model, active),
	m_sphere{position,ENEMY_SPEHERE_RADIUS },
	m_startPosition{position},
	m_groundHit{false}
{

}
/// <summary>
/// �f�X�g���N�^
/// </summary>
Enemy::~Enemy()
{
	
}
/// <summary>
/// ������	
/// </summary>
void Enemy::Initialize()
{
	//�R���W�����}�l�[�W���[�ɓG��AABB�����蔻���ǉ�����
	GameContext::GetInstance().GetCollisionManager()->AddEnemiesAABB(GetAABB());

	//sdkmesh�̍쐬
	CreateSdkMesh();
	//�����蔻��X�V
	CollisionAreaUpdate();

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Enemy::Update(const DX::StepTimer& timer)
{
	//��A�N�e�B�u��Ԃ��v���C���[������ł���ꍇ�X�V���Ȃ�
	if (!IsActive()||GameContext::GetInstance().IsPlayerDeath())
		return;

	//�t���[���ԕb��
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//���t���[���n�ʂɂ��ĂȂ���Ԃɂ���
	m_groundHit = false;
	
	//���݂̍��W�擾
	DirectX::SimpleMath::Vector3 nowPosition = GetPosition();

	//�A�j���[�V�����̍X�V
	SdkMeshUpdate(&m_animSdk, elapsedTime_s);

	//�v���C���[�ւ̈ړ���
	DirectX::SimpleMath::Vector3 moveVelocity = GameContext::GetInstance().GetPlayerPosition() - nowPosition;
	
	//�����O�ɂ���
	moveVelocity.y = 0;
	//���K��
	moveVelocity.Normalize();
	//�X�s�[�h���|����
	moveVelocity *= MOVE_SPEED * elapsedTime_s;

	//�v���C���[�����G�͈͂ɂ���
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerSphere2EnemySphere(&m_sphere))
	{
		//�ړ��ł����
		moveVelocity = Move(moveVelocity, nowPosition);
	}
	else//�͈͂ɂ��Ȃ��ꍇ���̏ꏊ�ɕς���
	{
		//�@��
		DirectX::SimpleMath::Vector3 normal;
		//�|���S���Ɛ����̊ђʓ_
		DirectX::SimpleMath::Vector3 polygonToLineHitPosition;

		//�n�ʂƓ������Ă��邩
		m_groundHit = GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon
		(
			{
				nowPosition + DirectX::SimpleMath::Vector3(0,ENEMY_HEGHT_COLLISITION_LINE,0),
				nowPosition - DirectX::SimpleMath::Vector3(0,ENEMY_HEGHT_COLLISITION_LINE,0)
			},
			normal,
			polygonToLineHitPosition
		);
		
		//���̏ꏊ�ɕς���p�ړ���
		DirectX::SimpleMath::Vector3 startPositionVelocity = m_startPosition - nowPosition;
		startPositionVelocity.y = 0;

		//���̏ꏊ��
		if (startPositionVelocity.Length() > MOVE_SPEED * elapsedTime_s)
		{
			startPositionVelocity.Normalize();
			startPositionVelocity *= MOVE_SPEED * elapsedTime_s;
			
			
			moveVelocity = Move(startPositionVelocity, nowPosition);
		}
		else
		{
			//�A�j���[�V�����̍X�V
			SdkMeshUpdate(&m_animIdleSdk, elapsedTime_s);

			//�ړ��ʂ��O�ɂ���
			moveVelocity = DirectX::SimpleMath::Vector3::Zero;
		}
	}
	//�W�����v�͎擾
	DirectX::SimpleMath::Vector3 jumpVelocity = GetVelocity();
	jumpVelocity.x = 0;
	jumpVelocity.z = 0;

	//�n�ʂɂ��Ă��Ȃ��ꍇ��������
	if (!m_groundHit)
	{
		
		//��������
		jumpVelocity.y += -1.4f * static_cast<float>(timer.GetElapsedSeconds());
	}
	else
	{
		jumpVelocity.y = 0;
	}

	//��]
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::FromToRotation(DirectX::SimpleMath::Vector3::Forward, moveVelocity + DirectX::SimpleMath::Vector3(0, -moveVelocity.y, 0));
	//��]�ݒ�
	SetRotation(rotation);
	//�ړ��ʐݒ�
	SetVelocity(moveVelocity+ jumpVelocity);
	//���W�X�V
	SetPosition(nowPosition+GetVelocity());
	//�����蔻��X�V
	CollisionAreaUpdate();
	//�v���C���[�̍U���͈͂ɓ����Ă��邩
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerAttackAABB2Enemies(GetAABB()))
	{
		//���S����
		SetActive(false);
		//�����蔻����v���C���[��������Ȃ��ꏊ�ɒu��
		AABBFor3D* aabb = GetAABB();
		DirectX::SimpleMath::Vector3 position = GetPosition();
		DirectX::SimpleMath::Vector3 area{10000 };
		aabb->SetData(area, area);

	}
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void Enemy::Render(const Camera* camera)
{
	//���S���Ă���ꍇ�������Ȃ�
	if (!IsActive())
		return;

	//�f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//���[���h�s��v�Z
	CalculateWorldMatrix();

	//���f���`��
	GetModel()->DrawSkinned
	(
		context,
		*GameContext::GetInstance().GetCommonState(),
		GetModel()->bones.size(),
		m_animBone.get(),
		GetWorldMatrix(),
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix()
	);

}

/// <summary>
/// �I������
/// </summary>
void Enemy::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void Enemy::Reset()
{
}

/// <summary>
/// �ړ��ł��邩����
/// </summary>
/// <param name="velocity">�ړ���</param>
/// <param name="position">���݂̍��W</param>
/// <returns>�ړ���</returns>
DirectX::SimpleMath::Vector3 Enemy::Move(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position)
{
	//�@��
	DirectX::SimpleMath::Vector3 normal;
	//�|���S���Ɛ����̓��������ꏊ
	DirectX::SimpleMath::Vector3 polygonToLineHitPosition;
	//�ړ���
	DirectX::SimpleMath::Vector3 moveVelocity;

	//X���ňړ��ł��邩�m�F
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon(
		{
			position + DirectX::SimpleMath::Vector3(velocity.x,ENEMY_HEGHT_COLLISITION_LINE *2,0),
			position - DirectX::SimpleMath::Vector3(-velocity.x,0,0)
		},
		normal,
		polygonToLineHitPosition))
	{
		//�ړ���̍��W
		DirectX::SimpleMath::Vector3 movedPosition = (polygonToLineHitPosition);
		//�ړ��ł���ʂ��v�Z
		moveVelocity.x = (movedPosition - position).x;
		moveVelocity.y = (movedPosition - position).y;
		//�n�ʂ𓖂����Ă���
		m_groundHit = true;
	}
	//�@��������
	normal = DirectX::SimpleMath::Vector3::Zero;
	//�|���S���Ɛ����̓��������ꏊ������
	polygonToLineHitPosition = DirectX::SimpleMath::Vector3::Zero;

	//Z���ňړ��ł��邩�m�F
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon(
		{
			position + DirectX::SimpleMath::Vector3(0,ENEMY_HEGHT_COLLISITION_LINE * 2,velocity.z),
			position - DirectX::SimpleMath::Vector3(0,0,-velocity.z)
		},
		normal,
		polygonToLineHitPosition))
	{
		//�ړ���̍��W
		DirectX::SimpleMath::Vector3 movedPosition = (polygonToLineHitPosition);
		//�ړ��ł���ʂ��v�Z		
		moveVelocity.z = (movedPosition - position).z;
		moveVelocity.y = (movedPosition - position).y;
		//�n�ʂ𓖂����Ă���
		m_groundHit = true;
	}
	//�ړ��ł���ʂ�Ԃ�
	return moveVelocity;
}

/// <summary>
/// �����蔻��X�V
/// </summary>
void Enemy::CollisionAreaUpdate()
{
	//AABB�擾
	AABBFor3D* aabb = GetAABB();
	//���݂̍��W�擾
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//�I�u�W�F�N�g�̒��S�_�ʒu�Ɉړ�
	position.y += ENEMY_HEGHT_COLLISITION_LINE / 2.f;
	//�����蔻��G���A
	DirectX::SimpleMath::Vector3 area{ 0.5f,1.5f,0.5f };
	//�����蔻��X�V
	aabb->SetData(position - area, position + area);

	//���̒��S���W���X�V
	m_sphere.centerPosition = position;
}

/// <summary>
/// sdkmesh�̍쐬
/// </summary>
void Enemy::CreateSdkMesh()
{
	//���f���̃{�[���̐��擾
	size_t nbones = GetModel()->bones.size();
	m_animBone = DirectX::ModelBone::MakeArray(nbones);
	GetModel()->CopyAbsoluteBoneTransformsTo(nbones, m_animBone.get());
	for (size_t j = 0; j < nbones; ++j)
	{
		m_animBone[j] = DirectX::XMMatrixMultiply(GetModel()->invBindPoseMatrices[j], m_animBone[j]);
	}

	//�A�j���[�V�����ǂݍ���
	DX::ThrowIfFailed(m_animSdk.Load(L"Resources/Models/EnemyWalking.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animIdleSdk.Load(L"Resources/Models/NeutralIdle.sdkmesh_anim"));

	//�G�t�F�N�g�X�V
	GetModel()->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto skin = dynamic_cast<DirectX::SkinnedEffect*>(effect);
			if (skin)
			{
				skin->SetPerPixelLighting(true);
			}
		});

	//�A�j���[�V�����Ƀ��f����ݒ�
	m_animSdk.Bind(*GetModel());
	m_animIdleSdk.Bind(*GetModel());

}

/// <summary>
/// sdkmesh�X�V
/// </summary>
/// <param name="sdkMeshAnimation">�A�j���[�V����</param>
/// <param name="elapsedTime">�t���[���ԕb��</param>
void Enemy::SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime)
{
	//�X�V
	sdkMeshAnimation->Update(elapsedTime);

	//���f���ɃA�^�b�`����
	size_t modelBone = GetModel()->bones.size();
	sdkMeshAnimation->Apply(*GetModel(), modelBone, m_animBone.get());
}