#include"pch.h"
#include"DeviceResources.h"

#include"PlayerTrackingEnemy.h"
#include"Enemy.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/InputKeyTime.h"
#include"Game/GameObject/Player/Player.h"

#include<Effects.h>

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="velocity">速度</param>
/// <param name="scale">拡大率</param>
/// <param name="rotation">角度</param>
/// <param name="model">モデル</param>
/// <param name="second">何秒前のプレイヤーの動きをコピーする秒数</param>
PlayerTrackingEnemy::PlayerTrackingEnemy(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model,float second)
	:
	Actor{ position, velocity,scale,rotation,model,true },
	m_playerPositions{},
	m_playerRecordIndex{0},
	m_playerTrackingIndex{1},
	m_isMove{false}
{
	//プレイヤーの座標を記録する配列
	m_playerPositions.resize(static_cast<int>(60 * second));
	//初期化
	for (DirectX::SimpleMath::Vector3& pos : m_playerPositions)
	{
		pos = DirectX::SimpleMath::Vector3::Zero;
	}
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerTrackingEnemy::~PlayerTrackingEnemy()
{
}

/// <summary>
/// 初期化
/// </summary>
void PlayerTrackingEnemy::Initialize()
{
	//skdmesh作成
	CreateSdkMesh();
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void PlayerTrackingEnemy::Update(const DX::StepTimer& timer)
{
	if (!IsActive() || GameContext::GetInstance().IsPlayerDeath())
		return;

	//フレーム間秒数
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());
	//プレイヤーの座標取得
	m_playerPositions[m_playerRecordIndex] = GameContext::GetInstance().GetPlayerPosition()-DirectX::SimpleMath::Vector3(0, Player::COLLISION_LINE_LENGTH /2.0f,0);
	//前のインデックス
	int previousIndex = m_playerRecordIndex -1;

	//0より小さければ配列の要素最大数にする
	if (previousIndex < 0)
	{
		previousIndex = m_playerPositions.size() - 1;
	}

	//前の座標と現在の座標同じでなければ現在のインデックスを増やす
	if (m_playerPositions[m_playerRecordIndex] != m_playerPositions[previousIndex])
	{
		m_playerRecordIndex++;
	}

	//動いていいか場合動く
	if (m_isMove)
	{
		Move(timer);

		//当たり判定更新
		CollisionAreaUpdate();

		//動いている場合当たり判定を追加
		GameContext::GetInstance().GetCollisionManager()->AddEnemiesAABB(GetAABB());
	}

	//座標配列に全てに記録したら動く
	if (m_playerRecordIndex >= m_playerPositions.size())
	{
		m_playerRecordIndex = 0;
		m_isMove = true;
	}
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void PlayerTrackingEnemy::Render(const Camera* camera)
{
	//動かない場合は描画しない
	if (!m_isMove)
		return;

	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//ワールド行列計算
	CalculateWorldMatrix();

	//モデル描画
	GetModel()->DrawSkinned
	(
		context,
		*GameContext::GetInstance().GetCommonState(),
		GetModel()->bones.size(),
		m_animBone.get(),
		GetWorldMatrix(),
		camera->GetViewMatrix(),
		camera->GetProjectionMatrix()
	);
}

/// <summary>
/// 終了処理
/// </summary>
void PlayerTrackingEnemy::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void PlayerTrackingEnemy::Reset()
{
}

/// <summary>
/// sdkmeshの作成
/// </summary>
void PlayerTrackingEnemy::CreateSdkMesh()
{
	//モデルのボーンの数取得
	size_t nbones = GetModel()->bones.size();
	m_animBone = DirectX::ModelBone::MakeArray(nbones);
	GetModel()->CopyAbsoluteBoneTransformsTo(nbones, m_animBone.get());

	for (size_t j = 0; j < nbones; ++j)
	{
		m_animBone[j] = DirectX::XMMatrixMultiply(GetModel()->invBindPoseMatrices[j], m_animBone[j]);
	}

	//アニメーション読み込み
	DX::ThrowIfFailed(m_animWalkSdk.Load(L"Resources/Models/WalkWithBriefcase.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animIdleSdk.Load(L"Resources/Models/NeutralIdle.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animJumpSdk.Load(L"Resources/Models/FallingIdle.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animDieSdk.Load(L"Resources/Models/FallingBackDeath.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animRunSdk.Load(L"Resources/Models/FastRun.sdkmesh_anim"));

	//エフェクト更新
	GetModel()->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto skin = dynamic_cast<DirectX::SkinnedEffect*>(effect);
			if (skin)
			{
				skin->SetPerPixelLighting(true);
			}
		});

	//アニメーションにモデルを設定
	m_animWalkSdk.Bind(*GetModel());
	m_animIdleSdk.Bind(*GetModel());
	m_animJumpSdk.Bind(*GetModel());
	m_animDieSdk.Bind(*GetModel());
	m_animRunSdk.Bind(*GetModel());
}

/// <summary>
/// 移動する
/// </summary>
/// <param name="timer">タイマー</param>
void PlayerTrackingEnemy::Move(const DX::StepTimer& timer)
{
	//フレーム間の秒数
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//移動
	SetPosition(m_playerPositions[m_playerTrackingIndex]);

	//プレイヤー追いかける用インデックス
	int previousPlayerTrackingIndex = m_playerTrackingIndex + 1;

	//インデックスが配列以上であれば０にする
	if (previousPlayerTrackingIndex >= m_playerPositions.size())
	{
		previousPlayerTrackingIndex = 0;
	}

	//次のプレイヤーの場所への移動量
	DirectX::SimpleMath::Vector3 playerTrackingvelocity = m_playerPositions[previousPlayerTrackingIndex] - m_playerPositions[m_playerTrackingIndex];

	//向いている方向
	DirectX::SimpleMath::Vector3 angleVelocity = DirectX::SimpleMath::Vector3(playerTrackingvelocity.x, 0.0f, playerTrackingvelocity.z);
	//角度計算
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::FromToRotation(DirectX::SimpleMath::Vector3::Forward, angleVelocity);

	//インデックスを増やす
	m_playerTrackingIndex++;

	//歩いているアニメーション更新
	SdkMeshUpdate(&m_animWalkSdk, elapsedTime_s);

	//次のプレイヤーの場所への移動量のYが0.1以上であればジャンプしている
	if (std::abs(playerTrackingvelocity.y) >= 0.1f)
	{
		//ジャンプアニメーション更新
		SdkMeshUpdate(&m_animJumpSdk, timer.GetElapsedSeconds());

	}
	//移動量が多きればダッシュする
	else if (playerTrackingvelocity.Length() > (Player::MOVE_SPEED * 1.4f) * elapsedTime_s)
	{
		//ダッシュアニメーション更新
		SdkMeshUpdate(&m_animRunSdk, timer.GetElapsedSeconds());
	}

	//回転
	SetRotation(rotation);

	//インデックスが配列以上の場合０にする
	if (m_playerTrackingIndex >= m_playerPositions.size())
	{
		m_playerTrackingIndex = 0;
	}
}

/// <summary>
/// 当たり判定更新
/// </summary>
void PlayerTrackingEnemy::CollisionAreaUpdate()
{
	//AABB取得
	AABBFor3D* aabb = GetAABB();
	//現在の座標取得
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//オブジェクトの中心点位置に移動
	position.y += Enemy::ENEMY_HEGHT_COLLISITION_LINE / 2.f;
	//当たり判定エリア
	DirectX::SimpleMath::Vector3 area{ 0.5f,1.5f,0.5f };
	//当たり判定更新
	aabb->SetData(position - area, position + area);
}

/// <summary>
/// sdkmesh更新
/// </summary>
/// <param name="sdkMeshAnimation">アニメーション</param>
/// <param name="elapsedTime">フレーム間秒数</param>
void PlayerTrackingEnemy::SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime)
{
	//更新
	sdkMeshAnimation->Update(elapsedTime);

	//モデルにアタッチする
	size_t modelBone = GetModel()->bones.size();
	sdkMeshAnimation->Apply(*GetModel(), modelBone, m_animBone.get());
}