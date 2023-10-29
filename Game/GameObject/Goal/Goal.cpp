#include"pch.h"
#include"Goal.h"
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
Goal::Goal(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model, bool active)
	:
	Actor(position, velocity, scale, rotation, model, active)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Goal::~Goal()
{
}

/// <summary>
/// 初期化	
/// </summary>
void Goal::Initialize()
{
	//コリジョンマネージャーが存在していればゴールの当たり判定を消す
	if(GameContext::GetInstance().GetCollisionManager()!=nullptr)
		GameContext::GetInstance().GetCollisionManager()->SetGoalAABB(nullptr);

	//当たり判定を設定
	//AABB当たり判定取得
	AABBFor3D* aabb = GetAABB();
	//座標取得
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//当たり判定エリアの大きさ
	DirectX::SimpleMath::Vector3 area{ 1.f };
	//エリアの設定
	aabb->SetData(position - area, position + area);

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Goal::Update(const DX::StepTimer& timer)
{
	//鍵をすべての取っているかつコリジョンマネージャーが存在している場合ゴールの当たり判定を追加する
	if(GameContext::GetInstance().GetIsAllGetKey() && GameContext::GetInstance().GetCollisionManager() != nullptr)
		GameContext::GetInstance().GetCollisionManager()->SetGoalAABB(GetAABB());

	
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void Goal::Render(const Camera* camera)
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
void Goal::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void Goal::Reset()
{
}
