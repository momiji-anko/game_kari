#include"pch.h"

#include"Obstacle.h"
#include"ObstacleManager.h"
#include"DeviceResources.h"

#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="velocity">移動量</param>
/// <param name="scale">拡縮</param>
/// <param name="rotation">スケール</param>
/// <param name="model">モデル</param>
/// <param name="active">アクティブ</param>
/// <param name="fireManager">エフェクト</param>
Obstacle::Obstacle(
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& velocity,
	const DirectX::SimpleMath::Vector3& scale, 
	const DirectX::SimpleMath::Vector3& rotation, 
	DirectX::Model* model, 
	bool active,
	FireEffectManager* fireManager
)
	:
	Actor{ position, velocity, scale, rotation, model, active },
	m_deleteTime{3},
	m_fireManager{fireManager}
{
}

Obstacle::~Obstacle()
{
}

void Obstacle::Initialize()
{
	AABBFor3D* aabb = GetAABB();
	DirectX::SimpleMath::Vector3 position = GetPosition();
	DirectX::SimpleMath::Vector3 area{ 0.5f,1.0f,0.5f };
	aabb->SetData(position - area, position + area);

	GameContext::GetInstance().GetCollisionManager()->AddEnemiesAABB(GetAABB());
}

void Obstacle::Update(const DX::StepTimer& timer)
{
	if (!IsActive() || GameContext::GetInstance().IsPlayerDeath())
		return;

	DirectX::SimpleMath::Vector3 position = GetPosition();
	DirectX::SimpleMath::Vector3 velocity = GetVelocity();

	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());


	SetPosition(position + (velocity * elapsedTime));

	CollisionAreaUpdate();

	m_deleteTime -= elapsedTime;

	if (m_deleteTime < 0)
	{
		SetActive(false);
	}

}

void Obstacle::Render(const Camera* camera)
{
	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	CalculateWorldMatrix();

	GetAABB()->Draw(DirectX::SimpleMath::Matrix::Identity, camera->GetViewMatrix(), camera->GetProjectionMatrix(), DirectX::SimpleMath::Vector4(1,0,0,1));
	
	//GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

	//エフェクト表示する
	m_fireManager->SetRenderState(camera->GetEyePosition(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
	m_fireManager->SetOffsetPosition(GetPosition());
	m_fireManager->Render();
	
}

void Obstacle::Finalize()
{
}

void Obstacle::Reset()
{
}

void Obstacle::CollisionAreaUpdate()
{
	AABBFor3D* aabb = GetAABB();
	DirectX::SimpleMath::Vector3 area{ 0.5f,0.5f,0.5f };
	aabb->SetData(GetPosition() - area, GetPosition() + area);

}
