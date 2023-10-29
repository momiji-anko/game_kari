#include"pch.h"
#include "Stage.h"
#include"Game/GameContext/GameContext.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/Camera.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="velocity">移動量</param>
/// <param name="scale">拡縮</param>
/// <param name="rotation">スケール</param>
/// <param name="model">モデル</param>
/// <param name="active">アクティブ</param>
Stage::Stage(
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& velocity,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Vector3& rotation,
	DirectX::Model* model,
	bool active
	)
	:
	Actor(position, velocity, scale, rotation, model, active)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Stage::~Stage()
{
}

/// <summary>
/// 初期化	
/// </summary>
void Stage::Initialize()
{
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Stage::Update(const DX::StepTimer& timer)
{


}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void Stage::Render(const Camera* camera)
{
	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	//ワールド行列計算
	CalculateWorldMatrix();
	//モデル描画
	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
}

/// <summary>
/// 終了処理
/// </summary>
void Stage::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void Stage::Reset()
{
}
