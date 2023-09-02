#include"pch.h"
#include"DeviceResources.h"
#include"Player.h"
#include<Keyboard.h>
#include<SimpleMath.h>
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/ModelManager.h"

//	1秒間に進むマスの数
const float Player::MOVE_SPEED = 9.0f;
//	1秒間に落ちるマスの数
const float Player::GRAVITY_FORCE = -1.4f;
//	ジャンプ力
const float Player::JUMP_FORCE = 0.50f;

const float Player::COLLISION_LINE_LENGTH=1.0f;

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
	m_sphere{ position,0.1f }
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	m_attackAABB = std::make_unique<AABBFor3D>();
	m_attackAABB->Initialize();
	DirectX::SimpleMath::Vector3 length = { 0.5f,0.5f,0.5f };

	//AABBの当たり判定を設定
	m_attackAABB->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);

	GameContext::GetInstance().GetCollisionManager()->SetPlayerAABB(GetAABB());
	GameContext::GetInstance().GetCollisionManager()->SetPlayerAttackAABB(m_attackAABB.get());
	GameContext::GetInstance().GetCollisionManager()->SetPlayerSphere(&m_sphere);
	GameContext::GetInstance().SetPlayerPosition(GetPosition());
}

void Player::Update(const DX::StepTimer& timer)
{
	PlayerMove(timer);
	
	CollisionAreaUpdate();

	GameContext::GetInstance().SetPlayerPosition(GetPosition());
}

void Player::Render(const Camera* camera)
{
	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();
	

	CalculateWorldMatrix();

	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

}

void Player::Finalize()
{
}

void Player::Reset()
{
}

void Player::PlayerMove(const DX::StepTimer& timer)
{
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

	//移動しているか
	bool IsMove = false;

	//90度
	static const float NINETY_ANGLE = DirectX::XM_PI / 2.0f;

	//右キーを押していた場合右に移動＆右を向く
	if (keyState.IsKeyDown(DirectX::Keyboard::D))
	{
		rotation.y = -NINETY_ANGLE;

		IsMove = true;

	}
	//左キーを押していた場合右に移動＆左を向く
	else if (keyState.IsKeyDown(DirectX::Keyboard::A))
	{

		rotation.y = NINETY_ANGLE;

		IsMove = true;
	}

	//後ろキーを押していた場合右に移動＆後ろを向く
	if (keyState.IsKeyDown(DirectX::Keyboard::S))
	{
		rotation.y = NINETY_ANGLE * 2.0f;

		IsMove = true;
	}
	//前キーを押していた場合右に移動＆前を向く
	else if (keyState.IsKeyDown(DirectX::Keyboard::W))
	{
		rotation.y = 0;

		IsMove = true;
	}

	//左キーと前キーを押していた場合左前を向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::A)) && (keyState.IsKeyDown(DirectX::Keyboard::W)))
	{
		//45
		rotation.y = NINETY_ANGLE / 2.0f;
	}

	//左キーと後ろキーを押していた場合左後ろを向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::A)) && (keyState.IsKeyDown(DirectX::Keyboard::S)))
	{
		//90+45
		rotation.y = NINETY_ANGLE + NINETY_ANGLE / 2.0f;
	}

	//右キーと前キーを押していた場合右前を向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::D)) && (keyState.IsKeyDown(DirectX::Keyboard::W)))
	{
		//-45
		rotation.y = -NINETY_ANGLE / 2.0f;
	}

	//右キーと後ろキーを押していた場合右後ろを向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::D)) && (keyState.IsKeyDown(DirectX::Keyboard::S)))
	{
		//-(90+45)
		rotation.y = -(NINETY_ANGLE + NINETY_ANGLE / 2.0f);
	}


	//移動している場合モデルタイムを増やす
	if (IsMove)
	{
		float rot = rotation.y + -GameContext::GetInstance().GetCmeraAngleY() + NINETY_ANGLE;
		
		velocity.x = cos(rot) * MOVE_SPEED * elapsedTime;
		velocity.z = -sin(rot) * MOVE_SPEED * elapsedTime;

		rotation.y = rot;
	}
	//移動していない場合モデルタイムは０にする
	else
	{
		
	}

	

	
	DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero;
	//ステージに当たっている場合落下しない
	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayerLine2Polygon({ position + DirectX::SimpleMath::Vector3(0,1.5,0),position - DirectX::SimpleMath::Vector3(0,COLLISION_LINE_LENGTH,0) }, normal, pos))
	{
		//ベロシティＹを０にする
		velocity.y = 0;
		
		DirectX::SimpleMath::Vector3 vel = velocity;

		vel.y = GRAVITY_FORCE * static_cast<float>(timer.GetElapsedSeconds());
		
		m_attackAABB->SetData(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero);

		DirectX::SimpleMath::Vector3 slideVec = PolygonToLineSegmentCollision::SlideVecCalculation(normal, vel);
		
		 //移動する
		position = pos + DirectX::SimpleMath::Vector3(0, COLLISION_LINE_LENGTH, 0) + slideVec;
		DirectX::SimpleMath::Vector3 area = DirectX::SimpleMath::Vector3(0.0);

		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(position.x, position.y - 1, position.z);


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
		DirectX::SimpleMath::Vector3 area = DirectX::SimpleMath::Vector3(0.5);

		DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3(position.x, position.y-1, position.z);
		m_attackAABB->SetData(pos - area, pos + area);
	}

	//ベロシティの設定
	SetVelocity(velocity);
	//移動する
	SetPosition(position + velocity);
	//角度設定
	SetRotation(rotation);

}

void Player::CollisionAreaUpdate()
{
	AABBFor3D* aabb = GetAABB();
	DirectX::SimpleMath::Vector3 position = GetPosition();
	DirectX::SimpleMath::Vector3 area{0.5};
	aabb->SetData(position - area, position + area);

	Capsule* capsule = GetCapsule();
	capsule->a = DirectX::SimpleMath::Vector3(0.f, -1, 0.f) + position;
	capsule->b = DirectX::SimpleMath::Vector3(0.f, 1, 0.f) + position;

	m_sphere.centerPosition = GetPosition();

}
