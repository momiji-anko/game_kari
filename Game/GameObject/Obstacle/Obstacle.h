#pragma once

#include"Game/GameObject/Actor/Actor.h"
#include"Game/Shader/FireEffectManager.h"

class ObstacleManager;
/// <summary>
/// 障害物
/// </summary>
class Obstacle:public Actor
{
public:

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
	Obstacle(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		DirectX::Model* model,
		bool active,
		FireEffectManager* fireManager
	);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Obstacle();

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

private:
	/// <summary>
	/// 当たり判定更新
	/// </summary>
	void CollisionAreaUpdate();

private:
	//障害物マネージャー

	float m_deleteTime;

	FireEffectManager* m_fireManager;
};