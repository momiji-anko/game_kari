#include"pch.h"
#include"DeviceResources.h"

#include"PlayerTrackingEnemy.h"
#include"Enemy.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/InputKeyTime.h"
#include"Game/GameObject/Player/Player.h"

#include<Effects.h>

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">���W</param>
/// <param name="velocity">���x</param>
/// <param name="scale">�g�嗦</param>
/// <param name="rotation">�p�x</param>
/// <param name="model">���f��</param>
/// <param name="second">���b�O�̃v���C���[�̓������R�s�[����b��</param>
PlayerTrackingEnemy::PlayerTrackingEnemy(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model,float second)
	:
	Actor{ position, velocity,scale,rotation,model,true },
	m_playerPositions{},
	m_playerRecordIndex{0},
	m_playerTrackingIndex{1},
	m_isMove{false}
{
	//�v���C���[�̍��W���L�^����z��
	m_playerPositions.resize(static_cast<int>(60 * second));
	//������
	for (DirectX::SimpleMath::Vector3& pos : m_playerPositions)
	{
		pos = DirectX::SimpleMath::Vector3::Zero;
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerTrackingEnemy::~PlayerTrackingEnemy()
{
}

/// <summary>
/// ������
/// </summary>
void PlayerTrackingEnemy::Initialize()
{
	//skdmesh�쐬
	CreateSdkMesh();
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void PlayerTrackingEnemy::Update(const DX::StepTimer& timer)
{
	if (!IsActive() || GameContext::GetInstance().IsPlayerDeath())
		return;

	//�t���[���ԕb��
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());
	//�v���C���[�̍��W�擾
	m_playerPositions[m_playerRecordIndex] = GameContext::GetInstance().GetPlayerPosition()-DirectX::SimpleMath::Vector3(0, Player::COLLISION_LINE_LENGTH /2.0f,0);
	//�O�̃C���f�b�N�X
	int previousIndex = m_playerRecordIndex -1;

	//0��菬������Δz��̗v�f�ő吔�ɂ���
	if (previousIndex < 0)
	{
		previousIndex = m_playerPositions.size() - 1;
	}

	//�O�̍��W�ƌ��݂̍��W�����łȂ���Ό��݂̃C���f�b�N�X�𑝂₷
	if (m_playerPositions[m_playerRecordIndex] != m_playerPositions[previousIndex])
	{
		m_playerRecordIndex++;
	}

	//�����Ă������ꍇ����
	if (m_isMove)
	{
		Move(timer);

		//�����蔻��X�V
		CollisionAreaUpdate();

		//�����Ă���ꍇ�����蔻���ǉ�
		GameContext::GetInstance().GetCollisionManager()->AddEnemiesAABB(GetAABB());
	}

	//���W�z��ɑS�ĂɋL�^�����瓮��
	if (m_playerRecordIndex >= m_playerPositions.size())
	{
		m_playerRecordIndex = 0;
		m_isMove = true;
	}
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void PlayerTrackingEnemy::Render(const Camera* camera)
{
	//�����Ȃ��ꍇ�͕`�悵�Ȃ�
	if (!m_isMove)
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
void PlayerTrackingEnemy::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void PlayerTrackingEnemy::Reset()
{
}

/// <summary>
/// sdkmesh�̍쐬
/// </summary>
void PlayerTrackingEnemy::CreateSdkMesh()
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
	DX::ThrowIfFailed(m_animWalkSdk.Load(L"Resources/Models/WalkWithBriefcase.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animIdleSdk.Load(L"Resources/Models/NeutralIdle.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animJumpSdk.Load(L"Resources/Models/FallingIdle.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animDieSdk.Load(L"Resources/Models/FallingBackDeath.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animRunSdk.Load(L"Resources/Models/FastRun.sdkmesh_anim"));

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
	m_animWalkSdk.Bind(*GetModel());
	m_animIdleSdk.Bind(*GetModel());
	m_animJumpSdk.Bind(*GetModel());
	m_animDieSdk.Bind(*GetModel());
	m_animRunSdk.Bind(*GetModel());
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void PlayerTrackingEnemy::Move(const DX::StepTimer& timer)
{
	//�t���[���Ԃ̕b��
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//�ړ�
	SetPosition(m_playerPositions[m_playerTrackingIndex]);

	//�v���C���[�ǂ�������p�C���f�b�N�X
	int previousPlayerTrackingIndex = m_playerTrackingIndex + 1;

	//�C���f�b�N�X���z��ȏ�ł���΂O�ɂ���
	if (previousPlayerTrackingIndex >= m_playerPositions.size())
	{
		previousPlayerTrackingIndex = 0;
	}

	//���̃v���C���[�̏ꏊ�ւ̈ړ���
	DirectX::SimpleMath::Vector3 playerTrackingvelocity = m_playerPositions[previousPlayerTrackingIndex] - m_playerPositions[m_playerTrackingIndex];

	//�����Ă������
	DirectX::SimpleMath::Vector3 angleVelocity = DirectX::SimpleMath::Vector3(playerTrackingvelocity.x, 0.0f, playerTrackingvelocity.z);
	//�p�x�v�Z
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::FromToRotation(DirectX::SimpleMath::Vector3::Forward, angleVelocity);

	//�C���f�b�N�X�𑝂₷
	m_playerTrackingIndex++;

	//�����Ă���A�j���[�V�����X�V
	SdkMeshUpdate(&m_animWalkSdk, elapsedTime_s);

	//���̃v���C���[�̏ꏊ�ւ̈ړ��ʂ�Y��0.1�ȏ�ł���΃W�����v���Ă���
	if (std::abs(playerTrackingvelocity.y) >= 0.1f)
	{
		//�W�����v�A�j���[�V�����X�V
		SdkMeshUpdate(&m_animJumpSdk, timer.GetElapsedSeconds());

	}
	//�ړ��ʂ�������΃_�b�V������
	else if (playerTrackingvelocity.Length() > (Player::MOVE_SPEED * 1.4f) * elapsedTime_s)
	{
		//�_�b�V���A�j���[�V�����X�V
		SdkMeshUpdate(&m_animRunSdk, timer.GetElapsedSeconds());
	}

	//��]
	SetRotation(rotation);

	//�C���f�b�N�X���z��ȏ�̏ꍇ�O�ɂ���
	if (m_playerTrackingIndex >= m_playerPositions.size())
	{
		m_playerTrackingIndex = 0;
	}
}

/// <summary>
/// �����蔻��X�V
/// </summary>
void PlayerTrackingEnemy::CollisionAreaUpdate()
{
	//AABB�擾
	AABBFor3D* aabb = GetAABB();
	//���݂̍��W�擾
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//�I�u�W�F�N�g�̒��S�_�ʒu�Ɉړ�
	position.y += Enemy::ENEMY_HEGHT_COLLISITION_LINE / 2.f;
	//�����蔻��G���A
	DirectX::SimpleMath::Vector3 area{ 0.5f,1.5f,0.5f };
	//�����蔻��X�V
	aabb->SetData(position - area, position + area);
}

/// <summary>
/// sdkmesh�X�V
/// </summary>
/// <param name="sdkMeshAnimation">�A�j���[�V����</param>
/// <param name="elapsedTime">�t���[���ԕb��</param>
void PlayerTrackingEnemy::SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime)
{
	//�X�V
	sdkMeshAnimation->Update(elapsedTime);

	//���f���ɃA�^�b�`����
	size_t modelBone = GetModel()->bones.size();
	sdkMeshAnimation->Apply(*GetModel(), modelBone, m_animBone.get());
}