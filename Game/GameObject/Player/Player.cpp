#include"pch.h"
#include"DeviceResources.h"
#include"Player.h"
#include<SimpleMath.h>
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/ModelManager.h"

//	1�b�Ԃɐi�ރ}�X�̐�
const float Player::MOVE_SPEED = 9.0f;
//	1�b�Ԃɗ�����}�X�̐�
const float Player::GRAVITY_FORCE = -1.4f;
//	�W�����v��
const float Player::JUMP_FORCE = 0.50f;

const float Player::COLLISION_LINE_LENGTH=3.0f;

//�������ۂ̎��S���鍂��
const float Player::FALL_DEAD_AREA = -50.0f;
//���f���^�C���̍ő吔
const float Player::MAX_MODEL_TIME_S = 4.0f;
//���f���^�C���̑��x
const int Player::MODEL_TIME_SPEED = 10.0f;


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

Player::~Player()
{
}

void Player::Initialize()
{
	m_attackAABB = std::make_unique<AABBFor3D>();
	m_attackAABB->Initialize();
	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	//AABB�̓����蔻���ݒ�
	m_attackAABB->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);

	GameContext::GetInstance().GetCollisionManager()->SetPlayerAABB(GetAABB());
	GameContext::GetInstance().GetCollisionManager()->SetPlayerAttackAABB(m_attackAABB.get());
	GameContext::GetInstance().GetCollisionManager()->SetPlayerSphere(&m_sphere);
	GameContext::GetInstance().SetPlayerPosition(GetPosition());

	//SDKMESH�̍쐬
	CreateSdkMesh();

	m_deathTime = 0.0f;
}

void Player::Update(const DX::StepTimer& timer)
{
	PlayerMove(timer);
	
	CollisionAreaUpdate();

	GameContext::GetInstance().SetPlayerPosition(GetPosition() + DirectX::SimpleMath::Vector3(0, COLLISION_LINE_LENGTH / 2.f, 0));

}

void Player::Render(const Camera* camera)
{
	//�f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	

	CalculateWorldMatrix();

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

void Player::Finalize()
{
}

void Player::Reset()
{
}

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
		float rot = rotation.y + -GameContext::GetInstance().GetCmeraAngleY() + NINETY_ANGLE;
		
		velocity.x = cos(rot) * MOVE_SPEED * elapsedTime* (1 + keyState.IsKeyDown(DirectX::Keyboard::LeftShift));
		velocity.z = -sin(rot) * MOVE_SPEED * elapsedTime * (1 + keyState.IsKeyDown(DirectX::Keyboard::LeftShift));

		DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero;

		bool isWallHit=GameContext::GetInstance().GetCollisionManager()->
			DetectCollisionCameraLine2Polygon(
				{	
					position+DirectX::SimpleMath::Vector3(0,1,0) ,
					position +DirectX::SimpleMath::Vector3(velocity.x,1.0f,velocity.z),
				}, 
				normal, 
				pos);


		if (isWallHit)
		{
			DirectX::SimpleMath::Vector3 slideVec = PolygonToLineSegmentCollision::SlideVecCalculation(normal, DirectX::SimpleMath::Vector3(velocity.x, 0.0f, velocity.z));
						
			velocity.x = slideVec.x;
			velocity.z = slideVec.z;
		}

		rotation.y = rot - NINETY_ANGLE;

		//�A�j���[�V�����̍X�V
		if (keyState.IsKeyDown(DirectX::Keyboard::LeftShift))
		{
			SdkMeshUpdate(&m_animRunSdk, elapsedTime);
		}
		else
		{
			SdkMeshUpdate(&m_animWalkSdk, elapsedTime);
		}
	}
	//�ړ����Ă��Ȃ��ꍇ���f���^�C���͂O�ɂ���
	else
	{
		SdkMeshUpdate(&m_animIdleSdk, elapsedTime);
	}
	
	
	DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero;

	bool isGroundHit = GameContext::GetInstance().GetCollisionManager()->
		DetectCollisionPlayerLine2Polygon(
			{
				position + DirectX::SimpleMath::Vector3(0,COLLISION_LINE_LENGTH,0),
				position
			},
			normal,
			pos);

	//�X�e�[�W�ɓ������Ă���ꍇ�������Ȃ�
	if (isGroundHit)
	{
		//�x���V�e�B�x���O�ɂ���
		velocity.y = 0;
		
		DirectX::SimpleMath::Vector3 vel = velocity;

		vel.y = GRAVITY_FORCE * elapsedTime;

		DirectX::SimpleMath::Vector3 slideVec = PolygonToLineSegmentCollision::SlideVecCalculation(normal, vel);
		
		 //�ړ�����
		position = pos + slideVec;

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

		SdkMeshUpdate(&m_animJumpSdk, elapsedTime);

	}


	bool isGoal = GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayer2Goal();
	if (isGoal)
	{
		GameContext::GetInstance().SetIsClear(isGoal);
		SetActive(false);
	}

	if (	
		GameContext().GetInstance().GetCollisionManager()->DetectCollisionPlayer2Enemies()||
		GameContext().GetInstance().GetCollisionManager()->DetectCollisionPlayer2FallDeathAABB()
		)
	{

		GameContext::GetInstance().SetPlayerDeath(true);
		GameContext::GetInstance().SetIsClear(false);


		SdkMeshUpdate(&m_animDieSdk, elapsedTime);
		
		m_deathTime += elapsedTime;

		velocity = DirectX::SimpleMath::Vector3::Zero;
		
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

void Player::CollisionAreaUpdate()
{
	AABBFor3D* aabb = GetAABB();
	DirectX::SimpleMath::Vector3 position = GetPosition();
	position.y += COLLISION_LINE_LENGTH / 2.0f;
	DirectX::SimpleMath::Vector3 area{0.5,1.5,0.5};
	aabb->SetData(position - area, position + area);

	Capsule* capsule = GetCapsule();
	capsule->a = DirectX::SimpleMath::Vector3(0.f, -1, 0.f) + position;
	capsule->b = DirectX::SimpleMath::Vector3(0.f, 1, 0.f) + position;

	m_sphere.centerPosition = GetPosition();

}

void Player::SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime)
{
	sdkMeshAnimation->Update(elapsedTime);
	
	size_t modelBone = GetModel()->bones.size();
	sdkMeshAnimation->Apply(*GetModel(), modelBone, m_animBone.get());

}

void Player::AttackAreaUpdate(bool isGroundHit)
{
	if (!isGroundHit)
	{
		DirectX::SimpleMath::Vector3 area = DirectX::SimpleMath::Vector3{ 0.5 };
		DirectX::SimpleMath::Vector3 position = GetPosition();

		DirectX::SimpleMath::Vector3 attckAreaCenterPosition = DirectX::SimpleMath::Vector3(position.x, position.y - 1, position.z);

		m_attackAABB->SetData(attckAreaCenterPosition - area, attckAreaCenterPosition + area);
	}
	else
	{
		m_attackAABB->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);
	}
}

float Player::PlayerMoveAngle(DirectX::Keyboard::State& keyState)
{
	//90�x
	static const float NINETY_ANGLE = DirectX::XM_PI / 2.0f;

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

	return playerMoveAngle;
}

void Player::CreateSdkMesh()
{
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

	
	GetModel()->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto skin = dynamic_cast<DirectX::SkinnedEffect*>(effect);
			if (skin)
			{
				skin->SetPerPixelLighting(true);
			}
		});

	m_animWalkSdk.Bind(*GetModel());
	m_animIdleSdk.Bind(*GetModel());
	m_animJumpSdk.Bind(*GetModel());
	m_animDieSdk.Bind(*GetModel());
	m_animRunSdk.Bind(*GetModel());
}

void Player::CreateShader()
{


}
