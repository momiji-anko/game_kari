#pragma once 
#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"

class Enemy :public Actor
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
	Enemy(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		DirectX::Model* model,
		bool active,
		std::vector<DirectX::SimpleMath::Vector3>& wanderPosition
	);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

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
	/// 
	/// </summary>
	/// <param name="velocity"></param>
	/// <param name="position"></param>
	/// <returns></returns>
	DirectX::SimpleMath::Vector3 Move(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position);

	/// <summary>
	/// 
	/// </summary>
	void CollisionAreaUpdate();

private:
	static const float ENEMY_SPEHERE_RADIUS;

	static const float MOVE_SPEED;

	static const float ENEMY_HEGHT_COLLISITION_LINE;
private:

	DX::AnimationSDKMESH  m_animSdk;
	DX::AnimationSDKMESH  m_animIdleSdk;

	//アニメーションスケルトン
	DirectX::ModelBone::TransformArray m_animBone;

	//スケルトン
	DirectX::ModelBone::TransformArray m_bone;

	Sphere m_sphere;

	std::vector<DirectX::SimpleMath::Vector3 >m_wanderPosition;

	DirectX::SimpleMath::Vector3 m_startPosition;

	int m_wanderNumber;

	bool m_groundHit;
};