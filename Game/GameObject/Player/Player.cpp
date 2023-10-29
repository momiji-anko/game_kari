#include"pch.h"
#include"DeviceResources.h"
#include"Player.h"
#include<SimpleMath.h>
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/ModelManager.h"

//1�b�Ԃɐi�ރ}�X�̐�
const float Player::MOVE_SPEED = 9.0f;
//1�b�Ԃɗ�����}�X�̐�
const float Player::GRAVITY_FORCE = -1.4f;
//�����̒���
const float Player::COLLISION_LINE_LENGTH=3.0f;

//�W�����v��
const float Player::JUMP_FORCE = 0.50f;
//�������ۂ̎��S���鍂��
const float Player::FALL_DEAD_AREA = -50.0f;
//���f���^�C���̍ő吔
const float Player::MAX_MODEL_TIME_S = 4.0f;
//���f���^�C���̑��x
const int Player::MODEL_TIME_SPEED = 10.0f;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="position">���W</param>
/// <param name="velocity">�ړ���</param>
/// <param name="scale">�g�k</param>
/// <param name="rotation">�X�P�[��</param>
/// <param name="model">���f��</param>
/// <param name="active">�A�N�e�B�u</param>
Player::Player(
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& velocity,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Vector3& rotation,
	DirectX::Model* model,
	bool active
	)
	:
	Actor{ position, velocity,scale,rotation,model,active },
	m_sphere{ position,0.1f },
	m_isMoveKey{false},
	m_deathTime{0.0f}
{
	
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
}

/// <summary>
/// ������	
/// </summary>
void Player::Initialize()
{
	//�U������쐬
	m_attackAABB = std::make_unique<AABBFor3D>();
	//������
	m_attackAABB->Initialize();

	//AABB�̓����蔻���ݒ�
	m_attackAABB->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);
	//�v���C���[��AABB�ݒ�
	GameContext::GetInstance().GetCollisionManager()->SetPlayerAABB(GetAABB());
	//�U������ݒ�
	GameContext::GetInstance().GetCollisionManager()->SetPlayerAttackAABB(m_attackAABB.get());
	//�v���C���[�̋��̓����蔻��ݒ�
	GameContext::GetInstance().GetCollisionManager()->SetPlayerSphere(&m_sphere);
	//���݂̍��W�ݒ�
	GameContext::GetInstance().SetPlayerPosition(GetPosition());

	//SDKMESH�̍쐬
	CreateSdkMesh();
	//���S�^�C�}�[������
	m_deathTime = 0.0f;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Player::Update(const DX::StepTimer& timer)
{
	//�ړ�
	PlayerMove(timer);
	//�����蔻��X�V
	CollisionAreaUpdate();
	//���݂̍��W�ݒ�
	GameContext::GetInstance().SetPlayerPosition(GetPosition() + DirectX::SimpleMath::Vector3(0, COLLISION_LINE_LENGTH / 2.f, 0));

}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void Player::Render(const Camera* camera)
{
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
void Player::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void Player::Reset()
{
}

/// <summary>
/// �ړ�
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Player::PlayerMove(const DX::StepTimer& timer)
{
	//90�x
	static const float NINETY_ANGLE = DirectX::XM_PI / 2.0f;

	//�o�ߎ���
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//�x���V�e�B�擾
	DirectX::SimpleMath::Vector3 velocity = GetVelocity();
	//���W�擾
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//�p�x�擾
	DirectX::SimpleMath::Vector3 rotation = GetRotation().ToEuler();
	
	//�x���V�e�B�̂w�Ƃy���O�ɂ���
	velocity.x = 0.0f;
	velocity.z = 0.0f;

	//�����Ă��Ȃ���Ԃɂ���
	m_isMoveKey = false;

	//�v���C���[�����������������߂�
	rotation.y = PlayerMoveAngle(keyState);

	//�ړ����Ă���ꍇ���f���^�C���𑝂₷
	if (m_isMoveKey)
	{
		//�X�O�x��]������
		float rot = rotation.y + -GameContext::GetInstance().GetCmeraAngleY() + NINETY_ANGLE;
		
		//�ړ��ʌv�Z
		velocity.x = cos(rot) * MOVE_SPEED * elapsedTime* (1 + keyState.IsKeyDown(DirectX::Keyboard::LeftShift));
		velocity.z = -sin(rot) * MOVE_SPEED * elapsedTime * (1 + keyState.IsKeyDown(DirectX::Keyboard::LeftShift));

		//�@��
		DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;
		//�|���S���Ɛ����̓��������ʒu
		DirectX::SimpleMath::Vector3 polygonToLineHitPosition = DirectX::SimpleMath::Vector3::Zero;

		//�ǂɓ������Ă��邩
		bool isWallHit=GameContext::GetInstance().GetCollisionManager()->
			DetectCollisionCameraLine2Polygon(
				{	
					position+DirectX::SimpleMath::Vector3(0,1,0) ,
					position +DirectX::SimpleMath::Vector3(velocity.x,1.0f,velocity.z),
				}, 
				normal, 
				polygonToLineHitPosition);


		//�ǂƓ������Ă��邩
		if (isWallHit)
		{
			//�X���C�h�x�N�g���v�Z
			DirectX::SimpleMath::Vector3 slideVec = PolygonToLineSegmentCollision::SlideVecCalculation(normal, DirectX::SimpleMath::Vector3(velocity.x, 0.0f, velocity.z));
			//�X���C�h�x�N�g�����ړ��ɑ��
			velocity.x = slideVec.x;
			velocity.z = slideVec.z;
		}

		//��]
		rotation.y = rot - NINETY_ANGLE;

		//�_�b�V������
		if (keyState.IsKeyDown(DirectX::Keyboard::LeftShift))
		{
			//�A�j���[�V�����̍X�V
			SdkMeshUpdate(&m_animRunSdk, elapsedTime);
		}
		else
		{
			//�A�j���[�V�����̍X�V
			SdkMeshUpdate(&m_animWalkSdk, elapsedTime);
		}
	}
	//�ړ����Ă��Ȃ��ꍇ���f���^�C���͂O�ɂ���
	else
	{
		//�A�j���[�V�����̍X�V
		SdkMeshUpdate(&m_animIdleSdk, elapsedTime);
	}
	
	//�@��
	DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;
	//�|���S���Ɛ����̓��������ʒu
	DirectX::SimpleMath::Vector3 polygonToLineHitPosition = DirectX::SimpleMath::Vector3::Zero;

	//�n�ʂƓ������Ă��邩
	bool isGroundHit = GameContext::GetInstance().GetCollisionManager()->
		DetectCollisionPlayerLine2Polygon(
			{
				position + DirectX::SimpleMath::Vector3(0,COLLISION_LINE_LENGTH,0),
				position
			},
			normal,
			polygonToLineHitPosition);

	//�X�e�[�W�ɓ������Ă���ꍇ�������Ȃ�
	if (isGroundHit)
	{
		//�x���V�e�B�x���O�ɂ���
		velocity.y = 0;
		
		DirectX::SimpleMath::Vector3 vel = velocity;

		vel.y = GRAVITY_FORCE * elapsedTime;

		DirectX::SimpleMath::Vector3 slideVec = PolygonToLineSegmentCollision::SlideVecCalculation(normal, vel);
		
		 //�ړ�����
		position = polygonToLineHitPosition + slideVec;

		//�W�����v�L�[����������W�����v����
		if (keyState.IsKeyDown(DirectX::Keyboard::Space))
		{
			velocity.y += JUMP_FORCE;
		}
	}
	//�X�e�[�W�ɓ������Ă��Ȃ��ꍇ��������
	else
	{
		//��������
		velocity.y += GRAVITY_FORCE * static_cast<float>(timer.GetElapsedSeconds());
		//�A�j���[�V�����X�V
		SdkMeshUpdate(&m_animJumpSdk, elapsedTime);

	}

	//�S�[�����Ă��邩
	bool isGoal = GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayer2Goal();
	if (isGoal)
	{
		//�N���A�ɂ���
		GameContext::GetInstance().SetIsClear(isGoal);
		//��A�N�e�B�u��ԂɈȍ~
		SetActive(false);
	}

	//�G�ɓ������Ă��邩
	if (	
		GameContext().GetInstance().GetCollisionManager()->DetectCollisionPlayer2Enemies()||
		GameContext().GetInstance().GetCollisionManager()->DetectCollisionPlayer2FallDeathAABB()
		)
	{
		//���S���Ă��邱�Ƃɂ���
		GameContext::GetInstance().SetPlayerDeath(true);
		//�N���A���Ă��Ȃ�
		GameContext::GetInstance().SetIsClear(false);

		//���S�A�j���[�V�����X�V
		SdkMeshUpdate(&m_animDieSdk, elapsedTime);
		//���S�^�C�}�[��i�߂�
		m_deathTime += elapsedTime;
		//�ړ����O�ɂ���
		velocity = DirectX::SimpleMath::Vector3::Zero;

		//�^�C�����Q�b�ȏ�ɂȂ������A�N�e�B�u��ԂɈȍ~
		if (m_deathTime >= 2)
		{
			SetActive(false);
		}
	}

	//�U�������蔻����X�V
	AttackAreaUpdate(isGroundHit);

	//�x���V�e�B�̐ݒ�
	SetVelocity(velocity);
	//�ړ�����
	SetPosition(position + velocity);
	//�p�x�ݒ�
	SetRotation(rotation);

}

/// <summary>
/// �����蔻��X�V
/// </summary>
void Player::CollisionAreaUpdate()
{
	//�`�`�a�a�擾
	AABBFor3D* aabb = GetAABB();
	//���W�擾
	DirectX::SimpleMath::Vector3 position = GetPosition();
	position.y += COLLISION_LINE_LENGTH / 2.0f;
	//�����蔻��̑傫��
	DirectX::SimpleMath::Vector3 area{0.5,1.5,0.5};
	//�����蔻��ݒ�
	aabb->SetData(position - area, position + area);

	//�J�v�Z���擾
	Capsule* capsule = GetCapsule();
	//�J�v�Z���̎n�_�ݒ�
	capsule->a = DirectX::SimpleMath::Vector3(0.f, -1, 0.f) + position;
	//�J�v�Z���̏I�_�ݒ�
	capsule->b = DirectX::SimpleMath::Vector3(0.f, 1, 0.f) + position;

	//���̒��S���W�ݒ�
	m_sphere.centerPosition = GetPosition();

}

/// <summary>
/// SDKMESH�X�V
/// </summary>
/// <param name="sdkMeshAnimation">�A�j���[�V����</param>
/// <param name="elapsedTime">�t���[���Ԏ���</param>
void Player::SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime)
{
	//�A�j���[�V�����X�V
	sdkMeshAnimation->Update(elapsedTime);
	//���f����SDKMESH���A�^�b�`
	size_t modelBone = GetModel()->bones.size();
	sdkMeshAnimation->Apply(*GetModel(), modelBone, m_animBone.get());

}

/// <summary>
/// �U������X�V
/// </summary>
/// <param name="isGroundHit">���n���Ă��邩</param>
void Player::AttackAreaUpdate(bool isGroundHit)
{
	//���n���Ă��邩
	if (!isGroundHit)
	{
		//����̑傫��
		DirectX::SimpleMath::Vector3 area = DirectX::SimpleMath::Vector3{ 0.5 };
		//���݂̍��W�擾
		DirectX::SimpleMath::Vector3 position = GetPosition();
		//�U������̒��S���W�v�Z
		DirectX::SimpleMath::Vector3 attckAreaCenterPosition = DirectX::SimpleMath::Vector3(position.x, position.y - 1, position.z);
		//�����ݒ�
		m_attackAABB->SetData(attckAreaCenterPosition - area, attckAreaCenterPosition + area);
	}
	else
	{
		//���n���Ă��Ȃ��ꍇ������Ȃ��悤�ɂ���
		m_attackAABB->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);
	}
}

/// <summary>
/// �v���C���[�̈ړ�����
/// </summary>
/// <param name="keyState">�L�[</param>
/// <returns>����</returns>
float Player::PlayerMoveAngle(DirectX::Keyboard::State& keyState)
{
	//90�x
	static const float NINETY_ANGLE = DirectX::XM_PI / 2.0f;
	//�v���C���[�̊p�x
	float playerMoveAngle = GetRotation().ToEuler().y;

	//�E�L�[�������Ă����ꍇ�E�Ɉړ����E������
	if (keyState.IsKeyDown(DirectX::Keyboard::D))
	{
		playerMoveAngle = -NINETY_ANGLE;

		m_isMoveKey = true;

	}
	//���L�[�������Ă����ꍇ�E�Ɉړ�����������
	else if (keyState.IsKeyDown(DirectX::Keyboard::A))
	{

		playerMoveAngle = NINETY_ANGLE;

		m_isMoveKey = true;
	}

	//���L�[�������Ă����ꍇ�E�Ɉړ�����������
	if (keyState.IsKeyDown(DirectX::Keyboard::S))
	{
		playerMoveAngle = NINETY_ANGLE * 2.0f;

		m_isMoveKey = true;
	}
	//�O�L�[�������Ă����ꍇ�E�Ɉړ����O������
	else if (keyState.IsKeyDown(DirectX::Keyboard::W))
	{
		playerMoveAngle = 0;

		m_isMoveKey = true;
	}

	//���L�[�ƑO�L�[�������Ă����ꍇ���O������
	if ((keyState.IsKeyDown(DirectX::Keyboard::A)) && (keyState.IsKeyDown(DirectX::Keyboard::W)))
	{
		//45
		playerMoveAngle = NINETY_ANGLE / 2.0f;
	}

	//���L�[�ƌ��L�[�������Ă����ꍇ����������
	if ((keyState.IsKeyDown(DirectX::Keyboard::A)) && (keyState.IsKeyDown(DirectX::Keyboard::S)))
	{
		//90+45
		playerMoveAngle = NINETY_ANGLE + NINETY_ANGLE / 2.0f;
	}

	//�E�L�[�ƑO�L�[�������Ă����ꍇ�E�O������
	if ((keyState.IsKeyDown(DirectX::Keyboard::D)) && (keyState.IsKeyDown(DirectX::Keyboard::W)))
	{
		//-45
		playerMoveAngle = -NINETY_ANGLE / 2.0f;
	}

	//�E�L�[�ƌ��L�[�������Ă����ꍇ�E��������
	if ((keyState.IsKeyDown(DirectX::Keyboard::D)) && (keyState.IsKeyDown(DirectX::Keyboard::S)))
	{
		//-(90+45)
		playerMoveAngle = -(NINETY_ANGLE + NINETY_ANGLE / 2.0f);
	}

	//�p�x��Ԃ�
	return playerMoveAngle;
}

/// <summary>
/// SDKMESH�쐬
/// </summary>
void Player::CreateSdkMesh()
{
	//�{�[���ݒ�
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

	//���f�����A�j���[�V�����Ƀo�C���h
	m_animWalkSdk.Bind(*GetModel());
	m_animIdleSdk.Bind(*GetModel());
	m_animJumpSdk.Bind(*GetModel());
	m_animDieSdk.Bind(*GetModel());
	m_animRunSdk.Bind(*GetModel());
}

