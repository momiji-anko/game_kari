#include"pch.h"

#include "Enemy.h"
#include"DeviceResources.h"

#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
#include<Effects.h>


const float Enemy::ENEMY_SPEHERE_RADIUS = 20.0f;
const float Enemy:: ENEMY_HEGHT_COLLISITION_LINE = 3.f;
const float Enemy::MOVE_SPEED = 3.0f;

Enemy::Enemy(
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& velocity, 
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Vector3& rotation, 
	DirectX::Model* model,
	bool active,
	std::vector<DirectX::SimpleMath::Vector3>& wanderPosition
	)
	:
	Actor(position, velocity, scale, rotation, model, active),
	m_sphere{position,ENEMY_SPEHERE_RADIUS },
	m_wanderPosition{wanderPosition},
	m_startPosition{position},
	m_wanderNumber{0},
	m_groundHit{false}
{

}

Enemy::~Enemy()
{
	
}

void Enemy::Initialize()
{
	GameContext::GetInstance().GetCollisionManager()->AddEnemiesAABB(GetAABB());


	size_t nbones = GetModel()->bones.size();

	m_animBone = DirectX::ModelBone::MakeArray(nbones);
	GetModel()->CopyAbsoluteBoneTransformsTo(nbones, m_animBone.get());

	for (size_t j = 0; j < nbones; ++j)
	{
		m_animBone[j] = DirectX::XMMatrixMultiply(GetModel()->invBindPoseMatrices[j], m_animBone[j]);
	}

	//アニメーション読み込み
	DX::ThrowIfFailed(m_animSdk.Load(L"Resources/Models/EnemyWalking.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animIdleSdk.Load(L"Resources/Models/NeutralIdle.sdkmesh_anim"));


	GetModel()->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto skin = dynamic_cast<DirectX::SkinnedEffect*>(effect);
			if (skin)
			{
				skin->SetPerPixelLighting(true);
			}
		});

	m_animSdk.Bind(*GetModel());
	m_animIdleSdk.Bind(*GetModel());

}

void Enemy::Update(const DX::StepTimer& timer)
{
	if (!IsActive()||GameContext::GetInstance().IsPlayerDeath())
		return;

	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	m_groundHit = false;

	m_sphere.centerPosition = GetPosition();

	DirectX::SimpleMath::Vector3 nowPosition = GetPosition();


	//アニメーションの更新
	m_animSdk.Update(elapsedTime_s);

	size_t nbones = GetModel()->bones.size();
	m_animSdk.Apply(*GetModel(), nbones, m_animBone.get());

	DirectX::SimpleMath::Vector3 moveVelocity = GameContext::GetInstance().GetPlayerPosition() - GetPosition();
	
	moveVelocity.y = 0;
	moveVelocity.Normalize();
	moveVelocity *= MOVE_SPEED * elapsedTime_s;

	//当たっていた
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerSphere2EnemySphere(&m_sphere))
	{

		moveVelocity = Move(moveVelocity, nowPosition);
	}
	else
	{
		DirectX::SimpleMath::Vector3 normal;
		DirectX::SimpleMath::Vector3 pos;

		m_groundHit = GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon
		(
			{
				nowPosition + DirectX::SimpleMath::Vector3(0,1.5,0),
				nowPosition - DirectX::SimpleMath::Vector3(0,1.5,0)
			},
			normal,
			pos
		);
		
		DirectX::SimpleMath::Vector3 startPositionVelocity = m_startPosition - nowPosition;
		startPositionVelocity.y = 0;

		if (startPositionVelocity.Length() > MOVE_SPEED * elapsedTime_s)
		{
			startPositionVelocity.Normalize();
			startPositionVelocity *= MOVE_SPEED * elapsedTime_s;
			
			
			moveVelocity = Move(startPositionVelocity, nowPosition);
		}
		else
		{
			//アニメーションの更新
			m_animIdleSdk.Update(elapsedTime_s);

			size_t nbones = GetModel()->bones.size();
			m_animIdleSdk.Apply(*GetModel(), nbones, m_animBone.get());

			moveVelocity = DirectX::SimpleMath::Vector3::Zero;
		}
	}

	DirectX::SimpleMath::Vector3 jumpVelocity = GetVelocity();
	jumpVelocity.x = 0;
	jumpVelocity.z = 0;

	if (!m_groundHit)
	{
		
		//落下する
		jumpVelocity.y += -1.4f * static_cast<float>(timer.GetElapsedSeconds());
	}
	else
	{
		jumpVelocity.y = 0;
	}

	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::FromToRotation(DirectX::SimpleMath::Vector3::Forward, moveVelocity + DirectX::SimpleMath::Vector3(0, -moveVelocity.y, 0));

	SetRotation(rotation);

	SetVelocity(moveVelocity+ jumpVelocity);

	SetPosition(nowPosition+GetVelocity());

	CollisionAreaUpdate();

	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerAttackAABB2Enemies(GetAABB()))
	{
		DirectX::SimpleMath::Vector3 vel=GameContext::GetInstance().GetPlayerPosition()-GetPosition();
		DirectX::SimpleMath::Vector3 v = DirectX::SimpleMath::Vector3::UnitY;

		SetActive(false);

		AABBFor3D* aabb = GetAABB();
		DirectX::SimpleMath::Vector3 position = GetPosition();
		DirectX::SimpleMath::Vector3 area{10000 };
		aabb->SetData(area, area);

	}
}

void Enemy::Render(const Camera* camera)
{
	if (!IsActive())
		return;

	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	GetAABB()->Draw(DirectX::SimpleMath::Matrix::Identity, camera->GetViewMatrix(), camera->GetProjectionMatrix(), DirectX::SimpleMath::Color(0, 1, 1, 1));

	

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

void Enemy::Finalize()
{
}

void Enemy::Reset()
{
}

DirectX::SimpleMath::Vector3 Enemy::Move(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position)
{
	DirectX::SimpleMath::Vector3 normal;
	DirectX::SimpleMath::Vector3 pos;

	DirectX::SimpleMath::Vector3 moveVelocity;

	float line = 1.5f;

	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon(
		{
			position + DirectX::SimpleMath::Vector3(velocity.x,line*2,0),
			position - DirectX::SimpleMath::Vector3(-velocity.x,0,0)
		},
		normal,
		pos))
	{
		DirectX::SimpleMath::Vector3 veloc = (pos);

		moveVelocity.x = (veloc-position).x;
		moveVelocity.y = (veloc - position).y;

		m_groundHit = true;
	}

	normal = DirectX::SimpleMath::Vector3::Zero;
	pos = DirectX::SimpleMath::Vector3::Zero;

	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon(
		{
			position + DirectX::SimpleMath::Vector3(0,line * 2,velocity.z),
			position - DirectX::SimpleMath::Vector3(0,0,-velocity.z)
		},
		normal,
		pos))
	{
		DirectX::SimpleMath::Vector3 veloc = (pos );
		
		moveVelocity.z = (veloc - position).z;
		moveVelocity.y = (veloc - position).y;

		m_groundHit = true;
	}
	
	return moveVelocity;
}

void Enemy::CollisionAreaUpdate()
{
	AABBFor3D* aabb = GetAABB();
	DirectX::SimpleMath::Vector3 position = GetPosition();
	position.y += ENEMY_HEGHT_COLLISITION_LINE / 2.f;
	DirectX::SimpleMath::Vector3 area{ 0.5f,1.5f,0.5f };
	aabb->SetData(position - area, position + area);

	m_sphere.centerPosition = position;
}
