#pragma once 
#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"
#include<Keyboard.h>


class Player :public Actor
{
public:
	static const float COLLISION_LINE_LENGTH;

	//	1秒間に落ちるマスの数
	static const float GRAVITY_FORCE;
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
	Player(
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
	~Player();

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
	void PlayerMove(const DX::StepTimer& timer);

	void CollisionAreaUpdate();

	void SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation,float elapsedTime);

	void AttackAreaUpdate(bool isGroundHit);

	float PlayerMoveAngle(DirectX::Keyboard::State& keyState);

	void CreateSdkMesh();

	void CreateShader();

private:
	//定数=====================================================
	//	1秒間に進むマスの数
	static const float  MOVE_SPEED;
	//	1秒間に落ちるマスの数
	static const float GRAVITY_FORCE;
	//	ジャンプ力
	static const float JUMP_FORCE;	
	//落ちた際の死亡する高さ
	static const float FALL_DEAD_AREA;
	//モデルタイムの最大数
	static const float MAX_MODEL_TIME_S;
	//モデルタイムの速度
	static const int MODEL_TIME_SPEED;


private:
	Sphere m_sphere;

	std::unique_ptr<AABBFor3D> m_attackAABB;

	//アニメーション
	DX::AnimationSDKMESH  m_animWalkSdk;
	DX::AnimationSDKMESH  m_animIdleSdk;
	DX::AnimationSDKMESH  m_animJumpSdk;
	DX::AnimationSDKMESH  m_animDieSdk;
	DX::AnimationSDKMESH  m_animRunSdk;

	//アニメーションスケルトン
	DirectX::ModelBone::TransformArray m_animBone;

	//スケルトン
	DirectX::ModelBone::TransformArray m_bone;


	//移動ボタンを押したか
	bool m_isMoveKey;

	float m_deathTime;

};