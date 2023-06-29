
#include "Actor.h"

#include"pch.h"

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
}

/// <summary>
/// 初期化	
/// </summary>
/// <param name="position">座標</param>
/// <param name="velcity">移動量</param>
/// <param name="scale">拡縮</param>
/// <param name="rotataion">スケール</param>
/// <param name="model">モデル</param>
/// <param name="active">アクティブ</param>
void Actor::Initialize(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velcity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotataion, DirectX::Model* model, bool active)
{
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Actor::Update(const DX::StepTimer& timer)
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
}
