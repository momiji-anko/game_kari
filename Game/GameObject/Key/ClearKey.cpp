#include"pch.h"
#include"ClearKey.h"

#include"Game/GameContext/GameContext.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Game/GameObject/Enemy/EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="velocity">移動量</param>
/// <param name="scale">拡縮</param>
/// <param name="rotation">スケール</param>
/// <param name="model">モデル</param>
/// <param name="active">アクティブ</param>
ClearKey::ClearKey(
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& velocity,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Vector3& rotation,
	DirectX::Model* model,
	bool active
)
	:
	Actor{ position, velocity, scale, rotation, model, active }
{
}

/// <summary>
/// デストラクタ
/// </summary>
ClearKey::~ClearKey()
{
}

/// <summary>
/// 初期化	
/// </summary>
void ClearKey::Initialize()
{
	//AABB当たり判定の取得
	AABBFor3D* aabb = GetAABB();
	//現在の座標取得
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//当たり判定の大きさ
	DirectX::SimpleMath::Vector3 area{ 0.5 };
	//当たり判定設定
	aabb->SetData(position - area, position + area);
	

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void ClearKey::Update(const DX::StepTimer& timer)
{
	//アクティブでなければ処理しない
	if (!IsActive())
		return;
	//プレイヤーに当たっているか
	bool isCollision = GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayer2Keys(GetAABB());

	//当たっている状態であれば非アクティブになり鍵を取っていることになる
	SetActive(!isCollision);


	if (isCollision)
	{
		GameContext::GetInstance().GetEnemyManager() ->CreatePlayerTrackingEnemy();
	}

}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void ClearKey::Render(const Camera* camera)
{
	//アクティブでなければ処理しない
	if (!IsActive())
		return;

	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	CalculateWorldMatrix();

	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());
}

/// <summary>
/// 終了処理
/// </summary>
void ClearKey::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void ClearKey::Reset()
{
}
