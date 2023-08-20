#include"pch.h"

#include "Enemy.h"
#include"DeviceResources.h"

#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"

const float Enemy::ENEMY_SPEHERE_RADIUS=50.0f;

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
	m_sphere{position,ENEMY_SPEHERE_RADIUS }
{


}

Enemy::~Enemy()
{
	
}

void Enemy::Initialize()
{
	GameContext::GetInstance().GetCollisionManager()->AddEnemiesAABB(GetAABB());
}

void Enemy::Update(const DX::StepTimer& timer)
{
	float speed = 3.0f;
	float line = 1.5f;

	m_sphere.centerPosition = GetPosition();

	DirectX::SimpleMath::Vector3 nowPosition = GetPosition();

	bool ishit = false;

	//当たっていた
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerSphere2EnemySphere(&m_sphere))
	{
		DirectX::SimpleMath::Vector3 playerVel = GameContext::GetInstance().GetPlayerPosition() - GetPosition();
		
		playerVel.Normalize();
		playerVel *= speed * timer.GetElapsedSeconds();
		playerVel.y = 0;

		

		DirectX::SimpleMath::Vector3 normal;
		DirectX::SimpleMath::Vector3 pos;

		if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon(
			{ 
				nowPosition + DirectX::SimpleMath::Vector3(playerVel.x,1.5,0),
				nowPosition - DirectX::SimpleMath::Vector3(-playerVel.x,line,0)
			},
			normal,
			pos))
		{
			DirectX::SimpleMath::Vector3 vel = GetVelocity();

			//移動する
			nowPosition = pos + DirectX::SimpleMath::Vector3(0, line, 0);

			ishit = true;
		
		}

		normal = DirectX::SimpleMath::Vector3::Zero;
		pos = DirectX::SimpleMath::Vector3::Zero;

		if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon(
			{
				nowPosition + DirectX::SimpleMath::Vector3(0,1.5,playerVel.z),
				nowPosition - DirectX::SimpleMath::Vector3(0,line,-playerVel.z)
			},
			normal,
			pos))
		{

			//移動する
			nowPosition = pos + DirectX::SimpleMath::Vector3(0, line, 0);

			ishit = true;
		}

	}
	DirectX::SimpleMath::Vector3 velocity = GetVelocity();
	if (!ishit)
	{
		//落下する
		velocity.y += -1.4f * static_cast<float>(timer.GetElapsedSeconds());
	}
	else
	{
		velocity.y = 0;
	}

	SetVelocity(velocity);

	SetPosition(nowPosition+velocity);

	AABBFor3D* aabb = GetAABB();
	DirectX::SimpleMath::Vector3 position = GetPosition();
	DirectX::SimpleMath::Vector3 area{ 0.5f,1.0f,0.5f };
	aabb->SetData(position - area, position + area);

	m_sphere.centerPosition = GetPosition();
}

void Enemy::Render(const Camera* camera)
{
	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	CalculateWorldMatrix();

	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());


}

void Enemy::Finalize()
{
}

void Enemy::Reset()
{
}
