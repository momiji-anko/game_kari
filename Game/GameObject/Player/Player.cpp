#include"pch.h"
#include"Player.h"
#include<Keyboard.h>
#include<SimpleMath.h>
#include"Game/GameContext/GameContext.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/ModelManager.h"

//	1�b�Ԃɐi�ރ}�X�̐�
const float Player::MOVE_SPEED = 9.0f;
//	1�b�Ԃɗ�����}�X�̐�
const float Player::GRAVITY_FORCE = -1.4f;
//	�W�����v��
const float Player::JUMP_FORCE = 0.50f;

Player::Player()
	:
	Actor()
{
}

Player::~Player()
{
}

void Player::Initialize(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model, bool active)
{
	SetPosition(position);
	SetVelocity(velocity);
	SetScale(scale);
	SetRotation(rotation);
	SetModel(model);
	SetActive(active);


	m_testCollisitionModel = ModelManager::GetInstance().LoadModel(L"Resources/Models/dice.cmo");
}

void Player::Update(const DX::StepTimer& timer)
{
	PlayerMove(timer);
	CollisionAreaUpdate();
}

void Player::Render(const Camera* camera)
{
	//�f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	CalculateWorldMatrix();

	DirectX::BoundingBox testCollision = m_testCollisitionModel->meshes.at(0)->boundingBox;
	testCollision.Transform(testCollision, GetWorldMatrix());

	DirectX::BoundingBox Collision = GetModel()->meshes.at(0)->boundingBox;
	Collision.Transform(Collision, DirectX::SimpleMath::Matrix::CreateScale(1));


	if (Collision.Intersects(testCollision))
	{
		m_testCollisitionModel->Draw(context, *GameContext::GetInstance().GetCommonState(), DirectX::SimpleMath::Matrix::Identity, camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}
	


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
	if (keyState.IsKeyDown(DirectX::Keyboard::Right))
	{
		velocity.x += MOVE_SPEED * elapsedTime;

		rotation.y = -NINETY_ANGLE;

		IsMove = true;

	}
	//���L�[�������Ă����ꍇ�E�Ɉړ�����������
	else if (keyState.IsKeyDown(DirectX::Keyboard::Left))
	{
		velocity.x -= MOVE_SPEED * elapsedTime;

		rotation.y = NINETY_ANGLE;

		IsMove = true;
	}

	//���L�[�������Ă����ꍇ�E�Ɉړ�����������
	if (keyState.IsKeyDown(DirectX::Keyboard::Down))
	{
		velocity.z += MOVE_SPEED * elapsedTime;

		rotation.y = NINETY_ANGLE * 2.0f;

		IsMove = true;
	}
	//�O�L�[�������Ă����ꍇ�E�Ɉړ����O������
	else if (keyState.IsKeyDown(DirectX::Keyboard::Up))
	{
		velocity.z -= MOVE_SPEED * elapsedTime;

		rotation.y = 0;

		IsMove = true;
	}

	//���L�[�ƑO�L�[�������Ă����ꍇ���O������
	if ((keyState.IsKeyDown(DirectX::Keyboard::Left)) && (keyState.IsKeyDown(DirectX::Keyboard::Up)))
	{
		//45
		rotation.y = NINETY_ANGLE / 2.0f;
	}

	//���L�[�ƌ��L�[�������Ă����ꍇ����������
	if ((keyState.IsKeyDown(DirectX::Keyboard::Left)) && (keyState.IsKeyDown(DirectX::Keyboard::Down)))
	{
		//90+45
		rotation.y = NINETY_ANGLE + NINETY_ANGLE / 2.0f;
	}

	//�E�L�[�ƑO�L�[�������Ă����ꍇ�E�O������
	if ((keyState.IsKeyDown(DirectX::Keyboard::Right)) && (keyState.IsKeyDown(DirectX::Keyboard::Up)))
	{
		//-45
		rotation.y = -NINETY_ANGLE / 2.0f;
	}

	//�E�L�[�ƌ��L�[�������Ă����ꍇ�E��������
	if ((keyState.IsKeyDown(DirectX::Keyboard::Right)) && (keyState.IsKeyDown(DirectX::Keyboard::Down)))
	{
		//-(90+45)
		rotation.y = -(NINETY_ANGLE + NINETY_ANGLE / 2.0f);
	}


	//�ړ����Ă���ꍇ���f���^�C���𑝂₷
	if (IsMove)
	{
		

	
	}
	//�ړ����Ă��Ȃ��ꍇ���f���^�C���͂O�ɂ���
	else
	{
		
	}


	//�X�e�[�W�ɓ������Ă���ꍇ�������Ȃ�
	if (position.y<=0)
	{
		//�x���V�e�B�x���O�ɂ���
		velocity.y = 0;

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
	}

	//�x���V�e�B�̐ݒ�
	SetVelocity(velocity);
	//�ړ�����
	SetPosition(GetPosition() + velocity);
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

}
