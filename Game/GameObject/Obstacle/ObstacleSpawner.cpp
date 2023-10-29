#include"pch.h"

#include"ObstacleSpawner.h"
#include"ObstacleManager.h"
#include"Obstacle.h"
#include"DeviceResources.h"

#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Libraries/MyLibraries/Camera.h"

//障害物生成クールタイム
const float ObstacleSpawner::OBSTACLE_SPAWN_COOL_TIME_S=5;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="obstacleManager">障害物マネージャー</param>
/// <param name="area">範囲</param>
/// <param name="obstacleSpeed">障害物の速度</param>
/// <param name="obstacleScale">障害物の拡大率</param>
/// <param name="obstacleRotation">障害物の角度</param>
/// <param name="model">モデル</param>
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

/// <summary>
/// デストラクタ
/// </summary>
ObstacleSpawner::~ObstacleSpawner()
{

}

/// <summary>
/// 初期化	
/// </summary>
void ObstacleSpawner::Initialize()
{
	//エフェクト作成
	m_fireShaderManager = std::make_unique<FireEffectManager>();
	//シェーダー生成
	m_fireShaderManager->Create();
	//初期化
	m_fireShaderManager->Initialize(2.0f, DirectX::SimpleMath::Vector3::Zero);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void ObstacleSpawner::Update(const DX::StepTimer& timer)
{
	//アクティブ状態ではないORプレイヤーが死んでいない状態であれば更新しない
	if (!IsActive() || GameContext::GetInstance().IsPlayerDeath())
		return;

	//角度
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion(0,0,0,0);

	//プレイヤーが範囲内にいるか
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerSphere2EnemySphere(&m_sphere))
	{
		//プレイヤーへのベクトル
		DirectX::SimpleMath::Vector3 playerVel = GetPosition() - GameContext::GetInstance().GetPlayerPosition();
		//回転
		rotation = DirectX::SimpleMath::Quaternion::FromToRotation(DirectX::SimpleMath::Vector3::Forward, DirectX::SimpleMath::Vector3(playerVel));
		//クールタイムを減らす
		m_obstacleSpawnCoolTime_s -= timer.GetElapsedSeconds();
		//クールタイムが０になったら障害物のを生成する
		if (m_obstacleSpawnCoolTime_s < 0)
		{
			//障害物生成
			CreateObstacle();
			//クールタイムを
			m_obstacleSpawnCoolTime_s = OBSTACLE_SPAWN_COOL_TIME_S;
		}
	}
	//回転
	SetRotation(rotation);
	//シェーダー更新
	FireUpdate(timer);
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void ObstacleSpawner::Render(const Camera* camera)
{
	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//ワールド行列計算
	CalculateWorldMatrix();
	//モデル描画
	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

}

/// <summary>
/// 終了処理
/// </summary>
void ObstacleSpawner::Finalize()
{
	m_obstacleManager = nullptr;
}

/// <summary>
/// リセット
/// </summary>
void ObstacleSpawner::Reset()
{
}

/// <summary>
/// 障害物作成
/// </summary>
void ObstacleSpawner::CreateObstacle()
{
	//障害物
	std::unique_ptr<Actor> obstacle;
	//プレイヤー座標
	DirectX::SimpleMath::Vector3 playerPosition = GameContext::GetInstance().GetPlayerPosition();
	//移動方向
	DirectX::SimpleMath::Vector3 velocity = playerPosition - GetPosition();
	//正規化
	velocity.Normalize();
	//スピードを掛ける
	velocity *= m_obstacleSpeed;

	//生成
	obstacle = std::make_unique<Obstacle>(
		GetPosition(),
		velocity,
		m_obstaceleScale,
		m_obstacleRotation,
		ModelManager::GetInstance().LoadCmoModel(L"dice.cmo"),
		true,
		m_fireShaderManager.get()
		);

	//初期化
	obstacle->Initialize();

	//障害物追加
	m_obstacleManager->AddObstacle(std::move(obstacle));
}

/// <summary>
/// エフェクトの更新
/// </summary>
/// <param name="timer">タイマー</param>
void ObstacleSpawner::FireUpdate(const DX::StepTimer& timer)
{
	//ファイヤー更新回数
	int fireUpdateNumber = 5;

	//シェーダー更新
	//更新回数を増やしファイヤーっぽくしている
	for (int i = 0; i < fireUpdateNumber; i++)
	{
		m_fireShaderManager->Update(timer);
	}
}
