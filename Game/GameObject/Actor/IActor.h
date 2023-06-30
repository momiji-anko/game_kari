#pragma once
#include<SimpleMath.h>
#include<Model.h>
#include"StepTimer.h"

//前方宣言
class Camera;
class ShadowMap;
class AABBFor3D;
class Capsule;

/// <summary>
/// アクターのインターフェースクラス
/// </summary>
class IActor
{
public:
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& position) = 0;
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>現在の座標</returns>
	virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;

	/// <summary>
	/// 移動量の設定
	/// </summary>
	/// <param name="velcity">移動量</param>
	virtual void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) = 0;
	/// <summary>
	/// 移動量の取得
	/// </summary>
	/// <returns>移動量</returns>
	virtual DirectX::SimpleMath::Vector3 GetVelocity() const = 0;

	/// <summary>
	/// モデルの設定
	/// </summary>
	/// <param name="model">モデルの生ポインタ</param>
	virtual void SetModel(DirectX::Model* model) = 0;
	/// <summary>
	/// モデルの取得
	/// </summary>
	/// <returns>モデルの生ポインタ</returns>
	virtual DirectX::Model* GetModel() const = 0;

	/// <summary>
	/// アクティブの設定
	/// </summary>
	/// <param name="active">true = アクティブ、false = 非アクティブ</param>
	virtual void SetActive(bool active) = 0;
	/// <summary>
	/// アクティブ状態の取得
	/// </summary>
	/// <returns>true = アクティブ、false = 非アクティブ</returns>
	virtual bool GetActive() const = 0;
	/// <summary>
	/// アクティブ状態の確認
	/// </summary>
	/// <returns>true = アクティブ、false = 非アクティブ</returns>
	virtual bool IsActive() const = 0;

	/// <summary>
	/// 当たり判定AABBの取得
	/// </summary>
	/// <param name="AABBobject">AABBのユニークポインタ</param>
	virtual void SetAABB(std::unique_ptr<AABBFor3D>& AABBObject) = 0;
	/// <summary>
	/// 当たり判定AABBの設定
	/// </summary>
	/// <returns></returns>
	virtual AABBFor3D* GetAABB() const = 0;

	/// <summary>
	/// 当たり判定カプセルの設定
	/// </summary>
	/// <param name="capsule">カプセルのユニークポインタ</param>
	virtual void SetCapsule(std::unique_ptr<Capsule>& capsule) = 0;
	/// <summary>
	/// 当たり判定カプセルの取得
	/// </summary>
	/// <returns>カプセルの生ポインタ</returns>
	virtual Capsule* GetCapsule() const = 0;

	/// <summary>
	/// ワールド行列の設定
	/// </summary>
	/// <param name="world">ワールド行列</param>
	virtual void SetWorldMatrix(const DirectX::SimpleMath::Matrix& world) = 0;
	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	virtual DirectX::SimpleMath::Matrix GetWorldMatrix()const = 0;

	/// <summary>
	/// 拡縮の設定
	/// </summary>
	/// <param name="scale">拡縮</param>
	virtual void SetScale(const DirectX::SimpleMath::Vector3& scale) = 0;
	/// <summary>
	/// 拡縮の取得
	/// </summary>
	/// <returns>拡縮</returns>
	virtual DirectX::SimpleMath::Vector3 GetScale() const = 0;

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation">角度(クォータニオン)</param>
	virtual void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) = 0;
	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation">角度(Vector3)</param>
	virtual void SetRotation(const DirectX::SimpleMath::Vector3& rotation) = 0;
	/// <summary>
	/// 角度の取得
	/// </summary>
	/// <returns></returns>
	virtual DirectX::SimpleMath::Quaternion GetRotation() const = 0;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IActor() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IActor() = default;

	/// <summary>
	/// 初期化	
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="velcity">移動量</param>
	/// <param name="scale">拡縮</param>
	/// <param name="rotataion">スケール</param>
	/// <param name="model">モデル</param>
	/// <param name="active">アクティブ</param>
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& velcity,
		const DirectX::SimpleMath::Vector3 & scale,
		const DirectX::SimpleMath::Vector3 & rotataion,
		DirectX::Model* model,
		bool active
	) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	virtual void Update(const DX::StepTimer& timer) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	virtual void Render(const Camera* camera) = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// ワールド行列計算
	/// </summary>
	virtual void CalculateWorldMatrix() = 0;

	/// <summary>
	/// リセット
	/// </summary>
	virtual void Reset() = 0;

	/// <summary>
	/// 影生成
	/// </summary>
	/// <param name="shadow">シャドウマップの生ポインタ</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="projection">プロジェクション行列</param>
	virtual void CreateShadow(ShadowMap* shadow, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection) = 0;

};