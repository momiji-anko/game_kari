
#include"pch.h"
#include "Actor.h"
#include"DeviceResources.h"
#include"Game/GameContext/GameContext.h"
#include"Game/Shader/ShadowMap.h"

Actor::Actor()
	:
	m_position(DirectX::SimpleMath::Vector3::Zero),
	m_velocity(DirectX::SimpleMath::Vector3::Zero),
	m_model(nullptr),
	m_active(true),
	m_AABBObject(nullptr),
	m_capsule(nullptr),
	m_rotation{},
	m_world(DirectX::SimpleMath::Matrix::Identity)
{
	CreateAABB();
	CreateCapsule();

}

/// <summary>
/// 初期化	
/// </summary>
/// <param name="position">座標</param>
/// <param name="velocity">移動量</param>
/// <param name="scale">拡縮</param>
/// <param name="rotation">回転</param>
/// <param name="model">モデル</param>
/// <param name="active">アクティブ</param>
void Actor::Initialize(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model, bool active)
{
	//パラメータを初期化
	//座標
	m_position = position;
	//移動量
	m_velocity = velocity;
	//拡縮
	m_scale = scale;
	//回転
	m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.z, rotation.x);
	//モデル
	m_model = model;
	//アクティブ
	m_active = active;


}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Actor::Update(const DX::StepTimer& timer)
{
	//警告避け
	UNREFERENCED_PARAMETER(timer);
	
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void Actor::Render(const Camera* camera)
{
	//警告避け
	UNREFERENCED_PARAMETER(camera);
	
}

/// <summary>
/// 終了処理
/// </summary>
void Actor::Finalize()
{

}

/// <summary>
/// ワールド行列計算
/// </summary>
void Actor::CalculateWorldMatrix()
{
	//ワールド行列に単位行列を入れる
	m_world = DirectX::SimpleMath::Matrix::Identity;
	//移動
	DirectX::SimpleMath::Matrix translation = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	//回転
	DirectX::SimpleMath::Matrix rotation = DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation);
	//拡縮
	DirectX::SimpleMath::Matrix scale = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	//ワールド行列の計算
	m_world *= scale * rotation * translation;

}

/// <summary>
/// リセット
/// </summary>
void Actor::Reset()
{
}

/// <summary>
/// 影生成
/// </summary>
/// <param name="shadow">シャドウマップの生ポインタ</param>
/// <param name="view">ビュー行列</param>
/// <param name="projection">プロジェクション行列</param>
void Actor::CreateShadow(ShadowMap* shadow, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキスト取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//モデルがあれば影を生成する
	if (GetModel() != nullptr)
	{
		//ワールド行列を計算する
		CalculateWorldMatrix();

		//影生成
		GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), view, projection, false, [&]()
			{
				shadow->DrawShadowMap(context);
			}
		);
	}

}

/// <summary>
/// AABBの作成
/// </summary>
void Actor::CreateAABB()
{
	//AABB当たり判定作成
	m_AABBObject = std::make_unique<AABBFor3D>();
	//AABB初期化
	m_AABBObject->Initialize();

	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	//AABBの当たり判定を設定
	m_AABBObject->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);


}

/// <summary>
/// カプセルの生成
/// </summary>
void Actor::CreateCapsule()
{
	//カプセルの作成
	m_capsule = std::make_unique<Capsule>();
	//半径設定
	m_capsule->r = 1.0f;
}

