#include"pch.h"

#include"Obstacle.h"
#include"ObstacleManager.h"
#include"DeviceResources.h"

#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"

Obstacle::Obstacle(
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& velocity,
	const DirectX::SimpleMath::Vector3& scale, 
	const DirectX::SimpleMath::Vector3& rotation, 
	DirectX::Model* model, 
	bool active,
	ObstacleManager* obstacleManager,
	FireEffectManager* fireManager
)
	:
	Actor{ position, velocity, scale, rotation, model, active },
	m_obstacleManager{ obstacleManager },
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
	//�f�o�C�X���\�[�X�̎擾
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	CalculateWorldMatrix();

	GetAABB()->Draw(DirectX::SimpleMath::Matrix::Identity, camera->GetViewMatrix(), camera->GetProjectionMatrix(), DirectX::SimpleMath::Vector4(1,0,0,1));
	
	//GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

	//�G�t�F�N�g�\������
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