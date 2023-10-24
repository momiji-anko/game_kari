#pragma once
#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"
#include<Keyboard.h>

class PlayerTrackingEnemy :public Actor
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
	/// <param name="second">秒数</param>
	PlayerTrackingEnemy(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		DirectX::Model* model,
		float second
	);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerTrackingEnemy();

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
	void CreateSdkMesh();

	void SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime);

	void Move(const DX::StepTimer& timer);

private:
	//定数=====================================================
	//	1秒間に進むマスの数
	static const float  MOVE_SPEED;
	//	1秒間に落ちるマスの数
	static const float GRAVITY_FORCE;
	//	ジャンプ力
	static const float JUMP_FORCE;

private:

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

	bool m_isMove;

	std::vector < DirectX::SimpleMath::Vector3 > m_playerPositions;
	int m_nowIndex;
	int m_index;
};