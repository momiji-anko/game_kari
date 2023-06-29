#pragma once
#include"IActor.h"
#include"Game/GameObject/Collider/AABBFor3D/AABBFor3D.h"
#include"Game/GameObject/Collider/Capsule/Capsule.h"
#include<SimpleMath.h>

class Actor :public IActor
{
public:
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const DirectX::SimpleMath::Vector3& position)override final { m_position = position; }
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>現在の座標</returns>
	DirectX::SimpleMath::Vector3 GetPosition() const override final { return m_position; }

	/// <summary>
	/// 移動量の設定
	/// </summary>
	/// <param name="velcity">移動量</param>
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) override final { m_velocity = velocity; }
	/// <summary>
	/// 移動量の取得
	/// </summary>
	/// <returns>移動量</returns>
	DirectX::SimpleMath::Vector3 GetVelocity() const override final { return m_velocity; }

	/// <summary>
	/// モデルの設定
	/// </summary>
	/// <param name="model">モデルの生ポインタ</param>
	void SetModel(DirectX::Model* model) override final { m_model = model; }
	/// <summary>
	/// モデルの取得
	/// </summary>
	/// <returns>モデルの生ポインタ</returns>
	DirectX::Model* GetModel() const override final { return m_model; }

	/// <summary>
	/// アクティブの設定
	/// </summary>
	/// <param name="active">true = アクティブ、false = 非アクティブ</param>
	void SetActive(bool active)override final { m_active = active; }
	/// <summary>
	/// アクティブ状態の取得
	/// </summary>
	/// <returns>true = アクティブ、false = 非アクティブ</returns>
	bool GetActive() const override final { return m_active; }
	/// <summary>
	/// アクティブ状態の確認
	/// </summary>
	/// <returns>true = アクティブ、false = 非アクティブ</returns>
	bool IsActive() const override final { return m_active; }

	/// <summary>
	/// 当たり判定AABBの取得
	/// </summary>
	/// <param name="AABBobject">AABBのユニークポインタ</param>
	void SetAABB(std::unique_ptr<AABBFor3D>& AABBObject) override final { m_AABBObject = std::move(AABBObject); }
	/// <summary>
	/// 当たり判定AABBの設定
	/// </summary>
	/// <returns></returns>
	AABBFor3D* GetAABB() const override final { return m_AABBObject.get(); }

	/// <summary>
	/// 当たり判定カプセルの設定
	/// </summary>
	/// <param name="capsule">カプセルのユニークポインタ</param>
	void SetCapsule(std::unique_ptr<Capsule>& capsule) override final { m_capsule = std::move(capsule); }
	/// <summary>
	/// 当たり判定カプセルの取得
	/// </summary>
	/// <returns>カプセルの生ポインタ</returns>
	Capsule* GetCapsule() const override final { return m_capsule.get(); }

	/// <summary>
	/// 拡縮の設定
	/// </summary>
	/// <param name="scale">拡縮</param>
	void SetScale(const DirectX::SimpleMath::Vector3& scale) override final { m_scale = scale; }
	/// <summary>
	/// 拡縮の取得
	/// </summary>
	/// <returns>拡縮</returns>
	DirectX::SimpleMath::Vector3 GetScale() const override final { return m_scale; }

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation">角度(クォータニオン)</param>
	void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) override final { m_rotation = rotation; }
	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="rotation">角度(Vector3)</param>
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) override final { m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z); }
	/// <summary>
	/// 角度の取得
	/// </summary>
	/// <returns></returns>
	DirectX::SimpleMath::Quaternion GetRotation() const override final { return m_rotation; }

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Actor();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Actor() = default;

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
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotataion,
		DirectX::Model* model,
		bool active
	);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	virtual void Update(const DX::StepTimer& timer);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	virtual void Render(const Camera * camera);

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// ワールド行列計算
	/// </summary>
	void CalculateWorldMatrix();

	/// <summary>
	/// リセット
	/// </summary>
	virtual void Reset();

	/// <summary>
	/// 影生成
	/// </summary>
	/// <param name="shadow">シャドウマップの生ポインタ</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="projection">プロジェクション行列</param>
	virtual void CreateShadow(ShadowMap * shadow, const DirectX::SimpleMath::Matrix & view, const DirectX::SimpleMath::Matrix & projection);

private:
	//座標
	DirectX::SimpleMath::Vector3    m_position;
	//移動量
	DirectX::SimpleMath::Vector3    m_velocity;

	//モデルのポインター
	DirectX::Model* m_model;

	//アクティブ
	bool m_active;

	//AABB当たり判定
	std::unique_ptr<AABBFor3D> m_AABBObject;
	//カプセル当たり判定
	std::unique_ptr<Capsule>   m_capsule;

	//ワールド行列
	DirectX::SimpleMath::Matrix     m_world;
	//拡縮
	DirectX::SimpleMath::Vector3    m_scale;
	//角度
	DirectX::SimpleMath::Quaternion m_rotation;

};