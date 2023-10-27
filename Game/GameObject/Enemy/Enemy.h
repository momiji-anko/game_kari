#pragma once 
#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"

/// <summary>
/// 敵
/// </summary>
class Enemy :public Actor
{
public:
	//索敵範囲の大きさ
	static const float ENEMY_SPEHERE_RADIUS;
	//移動速度
	static const float MOVE_SPEED;
	//縦の線分の長さ
	static const float ENEMY_HEGHT_COLLISITION_LINE;
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
		bool active
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
	/// 移動できるか判定
	/// </summary>
	/// <param name="velocity">移動量</param>
	/// <param name="position">現在の座標</param>
	/// <returns>移動量</returns>
	DirectX::SimpleMath::Vector3 Move(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position);

	/// <summary>
	/// 当たり判定更新
	/// </summary>
	void CollisionAreaUpdate();

	/// <summary>
	/// sdkmeshの作成
	/// </summary>
	void CreateSdkMesh();
	
	/// <summary>
	/// sdkmesh更新
	/// </summary>
	/// <param name="sdkMeshAnimation">アニメーション</param>
	/// <param name="elapsedTime">フレーム間秒数</param>
	void SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime);



private:
	//アニメーション
	DX::AnimationSDKMESH  m_animSdk;
	DX::AnimationSDKMESH  m_animIdleSdk;

	//アニメーションスケルトン
	DirectX::ModelBone::TransformArray m_animBone;

	//スケルトン
	DirectX::ModelBone::TransformArray m_bone;

	//球の当たり判定
	Sphere m_sphere;

	//初期位置
	DirectX::SimpleMath::Vector3 m_startPosition;

	//地面と当たっているか
	bool m_groundHit;
};