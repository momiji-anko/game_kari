#include"pch.h"
#include"DeviceResources.h"
#include"Player.h"
#include<Keyboard.h>
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

const float Player::COLLISION_LINE_LENGTH=1.0f;

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
	m_sphere{ position,0.1f }
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
}

void Player::Update(const DX::StepTimer& timer)
{
	PlayerMove(timer);
	
	CollisionAreaUpdate();

	GameContext::GetInstance().SetPlayerPosition(GetPosition());
}

void Player::Render(const Camera* camera)
{
	//�f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	

	CalculateWorldMatrix();

	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

}

void Player::Finalize()
{
}

void Player::Reset()
{
}

void Player::PlayerMove(const DX::StepTimer& timer)
{
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

	//�ړ����Ă��邩
	bool IsMove = false;

	//90�x
	static const float NINETY_ANGLE = DirectX::XM_PI / 2.0f;

	//�E�L�[�������Ă����ꍇ�E�Ɉړ����E������
	if (keyState.IsKeyDown(DirectX::Keyboard::D))
	{
		rotation.y = -NINETY_ANGLE;

		IsMove = true;

	}
	//���L�[�������Ă����ꍇ�E�Ɉړ�����������
	else if (keyState.IsKeyDown(DirectX::Keyboard::A))
	{

		rotation.y = NINETY_ANGLE;

		IsMove = true;
	}

	//���L�[�������Ă����ꍇ�E�Ɉړ�����������
	if (keyState.IsKeyDown(DirectX::Keyboard::S))
	{
		rotation.y = NINETY_ANGLE * 2.0f;

		IsMove = true;
	}
	//�O�L�[�������Ă����ꍇ�E�Ɉړ����O������
	else if (keyState.IsKeyDown(DirectX::Keyboard::W))
	{
		rotation.y = 0;

		IsMove = true;
	}

	//���L�[�ƑO�L�[�������Ă����ꍇ���O������
	if ((keyState.IsKeyDown(DirectX::Keyboard::A)) && (keyState.IsKeyDown(DirectX::Keyboard::W)))
	{
		//45
		rotation.y = NINETY_ANGLE / 2.0f;
	}

	//���L�[�ƌ��L�[�������Ă����ꍇ����������
	if ((keyState.IsKeyDown(DirectX::Keyboard::A)) && (keyState.IsKeyDown(DirectX::Keyboard::S)))
	{
		//90+45
		rotation.y = NINETY_ANGLE + NINETY_ANGLE / 2.0f;
	}

	//�E�L�[�ƑO�L�[�������Ă����ꍇ�E�O������
	if ((keyState.IsKeyDown(DirectX::Keyboard::D)) && (keyState.IsKeyDown(DirectX::Keyboard::W)))
	{
		//-45
		rotation.y = -NINETY_ANGLE / 2.0f;
	}

	//�E�L�[�ƌ��L�[�������Ă����ꍇ�E��������
	if ((keyState.IsKeyDown(DirectX::Keyboard::D)) && (keyState.IsKeyDown(DirectX::Keyboard::S)))
	{
		//-(90+45)
		rotation.y = -(NINETY_ANGLE + NINETY_ANGLE / 2.0f);
	}


	//�ړ����Ă���ꍇ���f���^�C���𑝂₷
	if (IsMove)
	{
		float rot = rotation.y + -GameContext::GetInstance().GetCmeraAngleY() + NINETY_ANGLE;
		
		velocity.x = cos(rot) * MOVE_SPEED * elapsedTime;
		velocity.z = -sin(rot) * MOVE_SPEED * elapsedTime;

		rotation.y = rot;
	}
	//�ړ����Ă��Ȃ��ꍇ���f���^�C���͂O�ɂ���
	else
	{
		
	}

	

	
	DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero;
	//�X�e�[�W�ɓ������Ă���ꍇ�������Ȃ�
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon({ position + DirectX::SimpleMath::Vector3(0,1.5,0),position - DirectX::SimpleMath::Vector3(0,COLLISION_LINE_LENGTH,0) }, normal, pos))
	{
		//�x���V�e�B�x���O�ɂ���
		velocity.y = 0;
		
		DirectX::SimpleMath::Vector3 vel = velocity;

		vel.y = GRAVITY_FORCE * static_cast<float>(timer.GetElapsedSeconds());
		
		m_attackAABB->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);

		DirectX::SimpleMath::Vector3 slideVec = PolygonToLineSegmentCollision::SlideVecCalculation(normal, vel);
		
		 //�ړ�����
		position = pos + DirectX::SimpleMath::Vector3(0, COLLISION_LINE_LENGTH, 0) + slideVec;
		DirectX::SimpleMath::Vector3 area = DirectX::SimpleMath::Vector3(0.0);

		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(position.x, position.y - 1, position.z);


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
		DirectX::SimpleMath::Vector3 area = DirectX::SimpleMath::Vector3(0.5);

		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(position.x, position.y-1, position.z);
		m_attackAABB->SetData(pos - area, pos + area);
	}

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
	DirectX::SimpleMath::Vector3 area{0.5};
	aabb->SetData(position - area, position + area);

	Capsule* capsule = GetCapsule();
	capsule->a = DirectX::SimpleMath::Vector3(0.f, -1, 0.f) + position;
	capsule->b = DirectX::SimpleMath::Vector3(0.f, 1, 0.f) + position;

	m_sphere.centerPosition = GetPosition();

}
