#include"pch.h"

#include"ObstacleSpawner.h"
#include"ObstacleManager.h"
#include"Obstacle.h"
#include"DeviceResources.h"

#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Libraries/MyLibraries/Camera.h"

const float ObstacleSpawner::OBSTACLE_SPAWN_COOL_TIME_S=5;


ObstacleSpawner::ObstacleSpawner(
	DirectX::SimpleMath::Vector3 position,
	ObstacleManager* obstacleManager,
	float area,
	float obstacleSpeed,
	DirectX::SimpleMath::Vector3 obstacleScale, 
	DirectX::SimpleMath::Vector3 obstacleRotation,
	DirectX::Model* model
	)
	:
	Actor{
		position,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::One,
		DirectX::SimpleMath::Vector3::Zero,
		model,
		true
	},
	m_area{ area },
	m_obstacleManager{ obstacleManager },
	m_obstacleSpawnCoolTime_s{ OBSTACLE_SPAWN_COOL_TIME_S },
	m_sphere{ position,area }, 
	m_obstacleSpeed{obstacleSpeed},
	m_obstaceleScale{obstacleScale},
	m_obstacleRotation{obstacleRotation}
{
	
}

ObstacleSpawner::~ObstacleSpawner()
{

}

void ObstacleSpawner::Initialize()
{
	m_fireShaderManager = std::make_unique<FireEffectManager>();
	m_fireShaderManager->Create();
	m_fireShaderManager->Initialize(2.0f, DirectX::SimpleMath::Vector3::Zero);
}

void ObstacleSpawner::Update(const DX::StepTimer& timer)
{
	if (!IsActive() || GameContext::GetInstance().IsPlayerDeath())
		return;

	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerSphere2EnemySphere(&m_sphere))
	{
		m_obstacleSpawnCoolTime_s -= timer.GetElapsedSeconds();
		if (m_obstacleSpawnCoolTime_s < 0)
		{
			CreateObstacle();
			m_obstacleSpawnCoolTime_s = OBSTACLE_SPAWN_COOL_TIME_S;
		}
	}

	FireUpdate(timer);
}

void ObstacleSpawner::Render(const Camera* camera)
{
	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	CalculateWorldMatrix();

	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

}

void ObstacleSpawner::Finalize()
{
	m_obstacleManager = nullptr;
}

void ObstacleSpawner::Reset()
{
}

void ObstacleSpawner::CreateObstacle()
{
	std::unique_ptr<Actor> obstacle;

	DirectX::SimpleMath::Vector3 playerPosition = GameContext::GetInstance().GetPlayerPosition();
	DirectX::SimpleMath::Vector3 velocity = playerPosition - GetPosition();

	velocity.Normalize();
	velocity *= m_obstacleSpeed;

	obstacle = std::make_unique<Obstacle>(
		GetPosition(),
		velocity,
		m_obstaceleScale,
		m_obstacleRotation,
		ModelManager::GetInstance().LoadCmoModel(L"dice.cmo"),
		true,
		m_obstacleManager,
		m_fireShaderManager.get()
		);

	obstacle->Initialize();

	m_obstacleManager->AddObstacle(std::move(obstacle));
}

void ObstacleSpawner::FireUpdate(const DX::StepTimer& timer)
{
	for (int i = 0; i < 5; i++)
	{
		m_fireShaderManager->Update(timer);
	}
}
