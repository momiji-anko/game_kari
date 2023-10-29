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
	/// <param name="position">座標</param>
	/// <param name="obstacleManager">障害物マネージャー</param>
	/// <param name="area">範囲</param>
	/// <param name="obstacleSpeed">障害物の速度</param>
	/// <param name="obstacleScale">障害物の拡大率</param>
	/// <param name="obstacleRotation">障害物の角度</param>
	/// <param name="model">モデル</param>
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

	/// <summary>
	/// 障害物作成
	/// </summary>
	void CreateObstacle();

private:
	/// <summary>
	/// エフェクトの更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void FireUpdate(const DX::StepTimer& timer);

private:
	//障害物生成クールタイム
	static const float OBSTACLE_SPAWN_COOL_TIME_S;

private:
	//障害物生成範囲
	float m_area;
	//障害物マネージャー
	ObstacleManager* m_obstacleManager;
	//障害物生成クールタイム
	float m_obstacleSpawnCoolTime_s;
	//球の当たり判定
	Sphere m_sphere;
	//障害物拡大率
	DirectX::SimpleMath::Vector3 m_obstaceleScale;
	//障害物角度
	DirectX::SimpleMath::Vector3 m_obstacleRotation;
	//障害物のスピード
	float m_obstacleSpeed;

	//エフェクト
	std::unique_ptr<FireEffectManager> m_fireShaderManager;
};