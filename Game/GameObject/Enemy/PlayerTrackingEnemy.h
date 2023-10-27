#pragma once

#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"
#include<Keyboard.h>

/// <summary>
/// プレイヤーを追いかける敵
/// </summary>
class PlayerTrackingEnemy :public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="scale">拡大率</param>
	/// <param name="rotation">角度</param>
	/// <param name="model">モデル</param>
	/// <param name="second">何秒前のプレイヤーの動きをコピーする秒数</param>
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
	
	/// <summary>
	/// 移動する
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Move(const DX::StepTimer& timer);

	/// <summary>
	/// 当たり判定更新
	/// </summary>
	void CollisionAreaUpdate();

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
	
	//動いていいか
	bool m_isMove;
	
	//プレイヤーの座標を記録する配列
	std::vector < DirectX::SimpleMath::Vector3 > m_playerPositions;
	//プレイヤーの座標を記録するインデックス
	int m_playerRecordIndex;
	//プレイヤー追跡用インデックス
	int m_playerTrackingIndex;
};