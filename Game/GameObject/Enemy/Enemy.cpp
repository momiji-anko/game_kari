#include"pch.h"

#include "Enemy.h"
#include"DeviceResources.h"

#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
#include<Effects.h>

//索敵範囲の大きさ
const float Enemy::ENEMY_SPEHERE_RADIUS = 20.0f;
//縦の線分の長さ
const float Enemy:: ENEMY_HEGHT_COLLISITION_LINE = 1.5f;
//移動速度
const float Enemy::MOVE_SPEED = 3.0f;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="velocity">移動量</param>
/// <param name="scale">拡縮</param>
/// <param name="rotation">スケール</param>
/// <param name="model">モデル</param>
/// <param name="active">アクティブ</param>
Enemy::Enemy(
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& velocity, 
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Vector3& rotation, 
	DirectX::Model* model,
	bool active	
)
	:
	Actor(position, velocity, scale, rotation, model, active),
	m_sphere{position,ENEMY_SPEHERE_RADIUS },
	m_startPosition{position},
	m_groundHit{false}
{

}
/// <summary>
/// デストラクタ
/// </summary>
Enemy::~Enemy()
{
	
}
/// <summary>
/// 初期化	
/// </summary>
void Enemy::Initialize()
{
	//コリジョンマネージャーに敵のAABB当たり判定を追加する
	GameContext::GetInstance().GetCollisionManager()->AddEnemiesAABB(GetAABB());

	//sdkmeshの作成
	CreateSdkMesh();
	//当たり判定更新
	CollisionAreaUpdate();

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Enemy::Update(const DX::StepTimer& timer)
{
	//非アクティブ状態かプレイヤーが死んでいる場合更新しない
	if (!IsActive()||GameContext::GetInstance().IsPlayerDeath())
		return;

	//フレーム間秒数
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	//毎フレーム地面についてない状態にする
	m_groundHit = false;
	
	//現在の座標取得
	DirectX::SimpleMath::Vector3 nowPosition = GetPosition();

	//アニメーションの更新
	SdkMeshUpdate(&m_animSdk, elapsedTime_s);

	//プレイヤーへの移動量
	DirectX::SimpleMath::Vector3 moveVelocity = GameContext::GetInstance().GetPlayerPosition() - nowPosition;
	
	//ｙを０にする
	moveVelocity.y = 0;
	//正規化
	moveVelocity.Normalize();
	//スピードを掛ける
	moveVelocity *= MOVE_SPEED * elapsedTime_s;

	//プレイヤーが索敵範囲にいる
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerSphere2EnemySphere(&m_sphere))
	{
		//移動できる量
		moveVelocity = Move(moveVelocity, nowPosition);
	}
	else//範囲にいない場合元の場所に変える
	{
		//法線
		DirectX::SimpleMath::Vector3 normal;
		//ポリゴンと線分の貫通点
		DirectX::SimpleMath::Vector3 polygonToLineHitPosition;

		//地面と当たっているか
		m_groundHit = GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon
		(
			{
				nowPosition + DirectX::SimpleMath::Vector3(0,ENEMY_HEGHT_COLLISITION_LINE,0),
				nowPosition - DirectX::SimpleMath::Vector3(0,ENEMY_HEGHT_COLLISITION_LINE,0)
			},
			normal,
			polygonToLineHitPosition
		);
		
		//元の場所に変える用移動量
		DirectX::SimpleMath::Vector3 startPositionVelocity = m_startPosition - nowPosition;
		startPositionVelocity.y = 0;

		//元の場所に
		if (startPositionVelocity.Length() > MOVE_SPEED * elapsedTime_s)
		{
			startPositionVelocity.Normalize();
			startPositionVelocity *= MOVE_SPEED * elapsedTime_s;
			
			
			moveVelocity = Move(startPositionVelocity, nowPosition);
		}
		else
		{
			//アニメーションの更新
			SdkMeshUpdate(&m_animIdleSdk, elapsedTime_s);

			//移動量を０にする
			moveVelocity = DirectX::SimpleMath::Vector3::Zero;
		}
	}
	//ジャンプ力取得
	DirectX::SimpleMath::Vector3 jumpVelocity = GetVelocity();
	jumpVelocity.x = 0;
	jumpVelocity.z = 0;

	//地面についていない場合落下する
	if (!m_groundHit)
	{
		
		//落下する
		jumpVelocity.y += -1.4f * static_cast<float>(timer.GetElapsedSeconds());
	}
	else
	{
		jumpVelocity.y = 0;
	}

	//回転
	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::FromToRotation(DirectX::SimpleMath::Vector3::Forward, moveVelocity + DirectX::SimpleMath::Vector3(0, -moveVelocity.y, 0));
	//回転設定
	SetRotation(rotation);
	//移動量設定
	SetVelocity(moveVelocity+ jumpVelocity);
	//座標更新
	SetPosition(nowPosition+GetVelocity());
	//当たり判定更新
	CollisionAreaUpdate();
	//プレイヤーの攻撃範囲に入っているか
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerAttackAABB2Enemies(GetAABB()))
	{
		//死亡する
		SetActive(false);
		//当たり判定をプレイヤーが当たらない場所に置く
		AABBFor3D* aabb = GetAABB();
		DirectX::SimpleMath::Vector3 position = GetPosition();
		DirectX::SimpleMath::Vector3 area{10000 };
		aabb->SetData(area, area);

	}
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void Enemy::Render(const Camera* camera)
{
	//死亡している場合処理しない
	if (!IsActive())
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
void Enemy::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void Enemy::Reset()
{
}

/// <summary>
/// 移動できるか判定
/// </summary>
/// <param name="velocity">移動量</param>
/// <param name="position">現在の座標</param>
/// <returns>移動量</returns>
DirectX::SimpleMath::Vector3 Enemy::Move(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position)
{
	//法線
	DirectX::SimpleMath::Vector3 normal;
	//ポリゴンと線分の当たった場所
	DirectX::SimpleMath::Vector3 polygonToLineHitPosition;
	//移動量
	DirectX::SimpleMath::Vector3 moveVelocity;

	//X軸で移動できるか確認
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon(
		{
			position + DirectX::SimpleMath::Vector3(velocity.x,ENEMY_HEGHT_COLLISITION_LINE *2,0),
			position - DirectX::SimpleMath::Vector3(-velocity.x,0,0)
		},
		normal,
		polygonToLineHitPosition))
	{
		//移動後の座標
		DirectX::SimpleMath::Vector3 movedPosition = (polygonToLineHitPosition);
		//移動できる量を計算
		moveVelocity.x = (movedPosition - position).x;
		moveVelocity.y = (movedPosition - position).y;
		//地面を当たっている
		m_groundHit = true;
	}
	//法線初期化
	normal = DirectX::SimpleMath::Vector3::Zero;
	//ポリゴンと線分の当たった場所初期化
	polygonToLineHitPosition = DirectX::SimpleMath::Vector3::Zero;

	//Z軸で移動できるか確認
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon(
		{
			position + DirectX::SimpleMath::Vector3(0,ENEMY_HEGHT_COLLISITION_LINE * 2,velocity.z),
			position - DirectX::SimpleMath::Vector3(0,0,-velocity.z)
		},
		normal,
		polygonToLineHitPosition))
	{
		//移動後の座標
		DirectX::SimpleMath::Vector3 movedPosition = (polygonToLineHitPosition);
		//移動できる量を計算		
		moveVelocity.z = (movedPosition - position).z;
		moveVelocity.y = (movedPosition - position).y;
		//地面を当たっている
		m_groundHit = true;
	}
	//移動できる量を返す
	return moveVelocity;
}

/// <summary>
/// 当たり判定更新
/// </summary>
void Enemy::CollisionAreaUpdate()
{
	//AABB取得
	AABBFor3D* aabb = GetAABB();
	//現在の座標取得
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//オブジェクトの中心点位置に移動
	position.y += ENEMY_HEGHT_COLLISITION_LINE / 2.f;
	//当たり判定エリア
	DirectX::SimpleMath::Vector3 area{ 0.5f,1.5f,0.5f };
	//当たり判定更新
	aabb->SetData(position - area, position + area);

	//球の中心座標を更新
	m_sphere.centerPosition = position;
}

/// <summary>
/// sdkmeshの作成
/// </summary>
void Enemy::CreateSdkMesh()
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
	DX::ThrowIfFailed(m_animSdk.Load(L"Resources/Models/EnemyWalking.sdkmesh_anim"));
	DX::ThrowIfFailed(m_animIdleSdk.Load(L"Resources/Models/NeutralIdle.sdkmesh_anim"));

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
	m_animSdk.Bind(*GetModel());
	m_animIdleSdk.Bind(*GetModel());

}

/// <summary>
/// sdkmesh更新
/// </summary>
/// <param name="sdkMeshAnimation">アニメーション</param>
/// <param name="elapsedTime">フレーム間秒数</param>
void Enemy::SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime)
{
	//更新
	sdkMeshAnimation->Update(elapsedTime);

	//モデルにアタッチする
	size_t modelBone = GetModel()->bones.size();
	sdkMeshAnimation->Apply(*GetModel(), modelBone, m_animBone.get());
}