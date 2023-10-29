#include"pch.h"
#include"DeviceResources.h"
#include"Player.h"
#include<SimpleMath.h>
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/ModelManager.h"

//1秒間に進むマスの数
const float Player::MOVE_SPEED = 9.0f;
//1秒間に落ちるマスの数
const float Player::GRAVITY_FORCE = -1.4f;
//線分の長さ
const float Player::COLLISION_LINE_LENGTH=3.0f;

//ジャンプ力
const float Player::JUMP_FORCE = 0.50f;
//落ちた際の死亡する高さ
const float Player::FALL_DEAD_AREA = -50.0f;
//モデルタイムの最大数
const float Player::MAX_MODEL_TIME_S = 4.0f;
//モデルタイムの速度
const int Player::MODEL_TIME_SPEED = 10.0f;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">座標</param>
/// <param name="velocity">移動量</param>
/// <param name="scale">拡縮</param>
/// <param name="rotation">スケール</param>
/// <param name="model">モデル</param>
/// <param name="active">アクティブ</param>
Player::Player(
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Vector3& velocity,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Vector3& rotation,
	DirectX::Model* model,
	bool active
	)
	:
	Actor{ position, velocity,scale,rotation,model,active },
	m_sphere{ position,0.1f },
	m_isMoveKey{false},
	m_deathTime{0.0f}
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 初期化	
/// </summary>
void Player::Initialize()
{
	//攻撃判定作成
	m_attackAABB = std::make_unique<AABBFor3D>();
	//初期化
	m_attackAABB->Initialize();

	//AABBの当たり判定を設定
	m_attackAABB->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);
	//プレイヤーのAABB設定
	GameContext::GetInstance().GetCollisionManager()->SetPlayerAABB(GetAABB());
	//攻撃判定設定
	GameContext::GetInstance().GetCollisionManager()->SetPlayerAttackAABB(m_attackAABB.get());
	//プレイヤーの球の当たり判定設定
	GameContext::GetInstance().GetCollisionManager()->SetPlayerSphere(&m_sphere);
	//現在の座標設定
	GameContext::GetInstance().SetPlayerPosition(GetPosition());

	//SDKMESHの作成
	CreateSdkMesh();
	//死亡タイマー初期化
	m_deathTime = 0.0f;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void Player::Update(const DX::StepTimer& timer)
{
	//移動
	PlayerMove(timer);
	//当たり判定更新
	CollisionAreaUpdate();
	//現在の座標設定
	GameContext::GetInstance().SetPlayerPosition(GetPosition() + DirectX::SimpleMath::Vector3(0, COLLISION_LINE_LENGTH / 2.f, 0));

}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void Player::Render(const Camera* camera)
{
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
void Player::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void Player::Reset()
{
}

/// <summary>
/// 移動
/// </summary>
/// <param name="timer">タイマー</param>
void Player::PlayerMove(const DX::StepTimer& timer)
{
	//90度
	static const float NINETY_ANGLE = DirectX::XM_PI / 2.0f;

	//経過時間
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	//ベロシティ取得
	DirectX::SimpleMath::Vector3 velocity = GetVelocity();
	//座標取得
	DirectX::SimpleMath::Vector3 position = GetPosition();
	//角度取得
	DirectX::SimpleMath::Vector3 rotation = GetRotation().ToEuler();
	
	//ベロシティのＸとＺを０にする
	velocity.x = 0.0f;
	velocity.z = 0.0f;

	//押していない状態にする
	m_isMoveKey = false;

	//プレイヤーが向かう方向を決める
	rotation.y = PlayerMoveAngle(keyState);

	//移動している場合モデルタイムを増やす
	if (m_isMoveKey)
	{
		//９０度回転させる
		float rot = rotation.y + -GameContext::GetInstance().GetCmeraAngleY() + NINETY_ANGLE;
		
		//移動量計算
		velocity.x = cos(rot) * MOVE_SPEED * elapsedTime* (1 + keyState.IsKeyDown(DirectX::Keyboard::LeftShift));
		velocity.z = -sin(rot) * MOVE_SPEED * elapsedTime * (1 + keyState.IsKeyDown(DirectX::Keyboard::LeftShift));

		//法線
		DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;
		//ポリゴンと線分の当たった位置
		DirectX::SimpleMath::Vector3 polygonToLineHitPosition = DirectX::SimpleMath::Vector3::Zero;

		//壁に当たっているか
		bool isWallHit=GameContext::GetInstance().GetCollisionManager()->
			DetectCollisionCameraLine2Polygon(
				{	
					position+DirectX::SimpleMath::Vector3(0,1,0) ,
					position +DirectX::SimpleMath::Vector3(velocity.x,1.0f,velocity.z),
				}, 
				normal, 
				polygonToLineHitPosition);


		//壁と当たっているか
		if (isWallHit)
		{
			//スライドベクトル計算
			DirectX::SimpleMath::Vector3 slideVec = PolygonToLineSegmentCollision::SlideVecCalculation(normal, DirectX::SimpleMath::Vector3(velocity.x, 0.0f, velocity.z));
			//スライドベクトルを移動に代入
			velocity.x = slideVec.x;
			velocity.z = slideVec.z;
		}

		//回転
		rotation.y = rot - NINETY_ANGLE;

		//ダッシュ判定
		if (keyState.IsKeyDown(DirectX::Keyboard::LeftShift))
		{
			//アニメーションの更新
			SdkMeshUpdate(&m_animRunSdk, elapsedTime);
		}
		else
		{
			//アニメーションの更新
			SdkMeshUpdate(&m_animWalkSdk, elapsedTime);
		}
	}
	//移動していない場合モデルタイムは０にする
	else
	{
		//アニメーションの更新
		SdkMeshUpdate(&m_animIdleSdk, elapsedTime);
	}
	
	//法線
	DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;
	//ポリゴンと線分の当たった位置
	DirectX::SimpleMath::Vector3 polygonToLineHitPosition = DirectX::SimpleMath::Vector3::Zero;

	//地面と当たっているか
	bool isGroundHit = GameContext::GetInstance().GetCollisionManager()->
		DetectCollisionPlayerLine2Polygon(
			{
				position + DirectX::SimpleMath::Vector3(0,COLLISION_LINE_LENGTH,0),
				position
			},
			normal,
			polygonToLineHitPosition);

	//ステージに当たっている場合落下しない
	if (isGroundHit)
	{
		//ベロシティＹを０にする
		velocity.y = 0;
		
		DirectX::SimpleMath::Vector3 vel = velocity;

		vel.y = GRAVITY_FORCE * elapsedTime;

		DirectX::SimpleMath::Vector3 slideVec = PolygonToLineSegmentCollision::SlideVecCalculation(normal, vel);
		
		 //移動する
		position = polygonToLineHitPosition + slideVec;

		//ジャンプキーを押したらジャンプする
		if (keyState.IsKeyDown(DirectX::Keyboard::Space))
		{
			velocity.y += JUMP_FORCE;
		}
	}
	//ステージに当たっていない場合落下する
	else
	{
		//落下する
		velocity.y += GRAVITY_FORCE * static_cast<float>(timer.GetElapsedSeconds());
		//アニメーション更新
		SdkMeshUpdate(&m_animJumpSdk, elapsedTime);

	}

	//ゴールしているか
	bool isGoal = GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayer2Goal();
	if (isGoal)
	{
		//クリアにする
		GameContext::GetInstance().SetIsClear(isGoal);
		//非アクティブ状態に以降
		SetActive(false);
	}

	//敵に当たっているか
	if (	
		GameContext().GetInstance().GetCollisionManager()->DetectCollisionPlayer2Enemies()||
		GameContext().GetInstance().GetCollisionManager()->DetectCollisionPlayer2FallDeathAABB()
		)
	{
		//死亡していることにする
		GameContext::GetInstance().SetPlayerDeath(true);
		//クリアしていない
		GameContext::GetInstance().SetIsClear(false);

		//死亡アニメーション更新
		SdkMeshUpdate(&m_animDieSdk, elapsedTime);
		//死亡タイマーを進める
		m_deathTime += elapsedTime;
		//移動を０にする
		velocity = DirectX::SimpleMath::Vector3::Zero;

		//タイムが２秒以上になったら非アクティブ状態に以降
		if (m_deathTime >= 2)
		{
			SetActive(false);
		}
	}

	//攻撃当たり判定を更新
	AttackAreaUpdate(isGroundHit);

	//ベロシティの設定
	SetVelocity(velocity);
	//移動する
	SetPosition(position + velocity);
	//角度設定
	SetRotation(rotation);

}

/// <summary>
/// 当たり判定更新
/// </summary>
void Player::CollisionAreaUpdate()
{
	//ＡＡＢＢ取得
	AABBFor3D* aabb = GetAABB();
	//座標取得
	DirectX::SimpleMath::Vector3 position = GetPosition();
	position.y += COLLISION_LINE_LENGTH / 2.0f;
	//当たり判定の大きさ
	DirectX::SimpleMath::Vector3 area{0.5,1.5,0.5};
	//当たり判定設定
	aabb->SetData(position - area, position + area);

	//カプセル取得
	Capsule* capsule = GetCapsule();
	//カプセルの始点設定
	capsule->a = DirectX::SimpleMath::Vector3(0.f, -1, 0.f) + position;
	//カプセルの終点設定
	capsule->b = DirectX::SimpleMath::Vector3(0.f, 1, 0.f) + position;

	//球の中心座標設定
	m_sphere.centerPosition = GetPosition();

}

/// <summary>
/// SDKMESH更新
/// </summary>
/// <param name="sdkMeshAnimation">アニメーション</param>
/// <param name="elapsedTime">フレーム間時間</param>
void Player::SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime)
{
	//アニメーション更新
	sdkMeshAnimation->Update(elapsedTime);
	//モデルにSDKMESHをアタッチ
	size_t modelBone = GetModel()->bones.size();
	sdkMeshAnimation->Apply(*GetModel(), modelBone, m_animBone.get());

}

/// <summary>
/// 攻撃判定更新
/// </summary>
/// <param name="isGroundHit">着地しているか</param>
void Player::AttackAreaUpdate(bool isGroundHit)
{
	//着地しているか
	if (!isGroundHit)
	{
		//判定の大きさ
		DirectX::SimpleMath::Vector3 area = DirectX::SimpleMath::Vector3{ 0.5 };
		//現在の座標取得
		DirectX::SimpleMath::Vector3 position = GetPosition();
		//攻撃判定の中心座標計算
		DirectX::SimpleMath::Vector3 attckAreaCenterPosition = DirectX::SimpleMath::Vector3(position.x, position.y - 1, position.z);
		//判定を設定
		m_attackAABB->SetData(attckAreaCenterPosition - area, attckAreaCenterPosition + area);
	}
	else
	{
		//着地していない場合当たらないようにする
		m_attackAABB->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);
	}
}

/// <summary>
/// プレイヤーの移動方向
/// </summary>
/// <param name="keyState">キー</param>
/// <returns>方向</returns>
float Player::PlayerMoveAngle(DirectX::Keyboard::State& keyState)
{
	//90度
	static const float NINETY_ANGLE = DirectX::XM_PI / 2.0f;
	//プレイヤーの角度
	float playerMoveAngle = GetRotation().ToEuler().y;

	//右キーを押していた場合右に移動＆右を向く
	if (keyState.IsKeyDown(DirectX::Keyboard::D))
	{
		playerMoveAngle = -NINETY_ANGLE;

		m_isMoveKey = true;

	}
	//左キーを押していた場合右に移動＆左を向く
	else if (keyState.IsKeyDown(DirectX::Keyboard::A))
	{

		playerMoveAngle = NINETY_ANGLE;

		m_isMoveKey = true;
	}

	//後ろキーを押していた場合右に移動＆後ろを向く
	if (keyState.IsKeyDown(DirectX::Keyboard::S))
	{
		playerMoveAngle = NINETY_ANGLE * 2.0f;

		m_isMoveKey = true;
	}
	//前キーを押していた場合右に移動＆前を向く
	else if (keyState.IsKeyDown(DirectX::Keyboard::W))
	{
		playerMoveAngle = 0;

		m_isMoveKey = true;
	}

	//左キーと前キーを押していた場合左前を向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::A)) && (keyState.IsKeyDown(DirectX::Keyboard::W)))
	{
		//45
		playerMoveAngle = NINETY_ANGLE / 2.0f;
	}

	//左キーと後ろキーを押していた場合左後ろを向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::A)) && (keyState.IsKeyDown(DirectX::Keyboard::S)))
	{
		//90+45
		playerMoveAngle = NINETY_ANGLE + NINETY_ANGLE / 2.0f;
	}

	//右キーと前キーを押していた場合右前を向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::D)) && (keyState.IsKeyDown(DirectX::Keyboard::W)))
	{
		//-45
		playerMoveAngle = -NINETY_ANGLE / 2.0f;
	}

	//右キーと後ろキーを押していた場合右後ろを向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::D)) && (keyState.IsKeyDown(DirectX::Keyboard::S)))
	{
		//-(90+45)
		playerMoveAngle = -(NINETY_ANGLE + NINETY_ANGLE / 2.0f);
	}

	//角度を返す
	return playerMoveAngle;
}

/// <summary>
/// SDKMESH作成
/// </summary>
void Player::CreateSdkMesh()
{
	//ボーン設定
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

	//モデルをアニメーションにバインド
	m_animWalkSdk.Bind(*GetModel());
	m_animIdleSdk.Bind(*GetModel());
	m_animJumpSdk.Bind(*GetModel());
	m_animDieSdk.Bind(*GetModel());
	m_animRunSdk.Bind(*GetModel());
}

