#pragma once 

#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Game/Shader/FireEffectManager.h"

class ObstacleManager;

class  ObstacleSpawner : public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObstacleSpawner(DirectX::SimpleMath::Vector3 position, ObstacleManager* obstacleManager, float area,float obstacleSpeed,DirectX::SimpleMath::Vector3 obstacleScale,DirectX::SimpleMath::Vector3 obstacleRotation,DirectX::Model* model);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ObstacleSpawner();

	/// <summary>
	/// 初期化	
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void Render(const Camera* camera)override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset()override;

	void CreateObstacle();

private:
	void FireUpdate(const DX::StepTimer& timer);

private:
	static const float OBSTACLE_SPAWN_COOL_TIME_S;

private:
	float m_area;

	ObstacleManager* m_obstacleManager;

	float m_obstacleSpawnCoolTime_s;

	Sphere m_sphere;

	DirectX::SimpleMath::Vector3 m_obstaceleScale;
	DirectX::SimpleMath::Vector3 m_obstacleRotation;
	float m_obstacleSpeed;


	std::unique_ptr<FireEffectManager> m_fireShaderManager;
};