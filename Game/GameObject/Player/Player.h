#pragma once 
#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"
#include<Keyboard.h>

/// <summary>
/// プレイヤー
/// </summary>
class Player :public Actor
{
public:
	//線分の長さ
	static const float COLLISION_LINE_LENGTH;

	//1秒間に落ちるマスの数
	static const float GRAVITY_FORCE;

	//1秒間に進むマスの数
	static const float  MOVE_SPEED;

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

	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="timer">タイマー</param>
	void PlayerMove(const DX::StepTimer& timer);

	/// <summary>
	/// 当たり判定更新
	/// </summary>
	void CollisionAreaUpdate();

	/// <summary>
	/// SDKMESH更新
	/// </summary>
	/// <param name="sdkMeshAnimation">アニメーション</param>
	/// <param name="elapsedTime">フレーム間時間</param>
	void SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation,float elapsedTime);

	/// <summary>
	/// 攻撃判定更新
	/// </summary>
	/// <param name="isGroundHit">着地しているか</param>
	void AttackAreaUpdate(bool isGroundHit);

	/// <summary>
	/// プレイヤーの移動方向
	/// </summary>
	/// <param name="keyState">キー</param>
	/// <returns>方向</returns>
	float PlayerMoveAngle(DirectX::Keyboard::State& keyState);

	/// <summary>
	/// SDKMESH作成
	/// </summary>
	void CreateSdkMesh();


private:
	//定数=====================================================
	//	ジャンプ力
	static const float JUMP_FORCE;	
	//落ちた際の死亡する高さ
	static const float FALL_DEAD_AREA;
	//モデルタイムの最大数
	static const float MAX_MODEL_TIME_S;
	//モデルタイムの速度
	static const int MODEL_TIME_SPEED;


private:
	//球の当たり判定
	Sphere m_sphere;
	//攻撃範囲
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

	//死亡タイム
	float m_deathTime;

};