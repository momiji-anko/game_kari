#include"pch.h"
#include"Player.h"
#include<Keyboard.h>
#include<SimpleMath.h>
#include"Game/GameContext/GameContext.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/ModelManager.h"

//	1秒間に進むマスの数
const float Player::MOVE_SPEED = 9.0f;
//	1秒間に落ちるマスの数
const float Player::GRAVITY_FORCE = -1.4f;
//	ジャンプ力
const float Player::JUMP_FORCE = 0.50f;

Player::Player()
	:
	Actor()
{
}

Player::~Player()
{
}

void Player::Initialize(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model, bool active)
{
	SetPosition(position);
	SetVelocity(velocity);
	SetScale(scale);
	SetRotation(rotation);
	SetModel(model);
	SetActive(active);


	m_testCollisitionModel = ModelManager::GetInstance().LoadModel(L"Resources/Models/dice.cmo");
}

void Player::Update(const DX::StepTimer& timer)
{
	PlayerMove(timer);
	CollisionAreaUpdate();
}

void Player::Render(const Camera* camera)
{
	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	CalculateWorldMatrix();

	DirectX::BoundingBox testCollision = m_testCollisitionModel->meshes.at(0)->boundingBox;
	testCollision.Transform(testCollision, GetWorldMatrix());

	DirectX::BoundingBox Collision = GetModel()->meshes.at(0)->boundingBox;
	Collision.Transform(Collision, DirectX::SimpleMath::Matrix::CreateScale(1));


	if (Collision.Intersects(testCollision))
	{
		m_testCollisitionModel->Draw(context, *GameContext::GetInstance().GetCommonState(), DirectX::SimpleMath::Matrix::Identity, camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}
	


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
	if (keyState.IsKeyDown(DirectX::Keyboard::Right))
	{
		velocity.x += MOVE_SPEED * elapsedTime;

		rotation.y = -NINETY_ANGLE;

		IsMove = true;

	}
	//左キーを押していた場合右に移動＆左を向く
	else if (keyState.IsKeyDown(DirectX::Keyboard::Left))
	{
		velocity.x -= MOVE_SPEED * elapsedTime;

		rotation.y = NINETY_ANGLE;

		IsMove = true;
	}

	//後ろキーを押していた場合右に移動＆後ろを向く
	if (keyState.IsKeyDown(DirectX::Keyboard::Down))
	{
		velocity.z += MOVE_SPEED * elapsedTime;

		rotation.y = NINETY_ANGLE * 2.0f;

		IsMove = true;
	}
	//前キーを押していた場合右に移動＆前を向く
	else if (keyState.IsKeyDown(DirectX::Keyboard::Up))
	{
		velocity.z -= MOVE_SPEED * elapsedTime;

		rotation.y = 0;

		IsMove = true;
	}

	//左キーと前キーを押していた場合左前を向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::Left)) && (keyState.IsKeyDown(DirectX::Keyboard::Up)))
	{
		//45
		rotation.y = NINETY_ANGLE / 2.0f;
	}

	//左キーと後ろキーを押していた場合左後ろを向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::Left)) && (keyState.IsKeyDown(DirectX::Keyboard::Down)))
	{
		//90+45
		rotation.y = NINETY_ANGLE + NINETY_ANGLE / 2.0f;
	}

	//右キーと前キーを押していた場合右前を向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::Right)) && (keyState.IsKeyDown(DirectX::Keyboard::Up)))
	{
		//-45
		rotation.y = -NINETY_ANGLE / 2.0f;
	}

	//右キーと後ろキーを押していた場合右後ろを向く
	if ((keyState.IsKeyDown(DirectX::Keyboard::Right)) && (keyState.IsKeyDown(DirectX::Keyboard::Down)))
	{
		//-(90+45)
		rotation.y = -(NINETY_ANGLE + NINETY_ANGLE / 2.0f);
	}


	//移動している場合モデルタイムを増やす
	if (IsMove)
	{
		

	
	}
	//移動していない場合モデルタイムは０にする
	else
	{
		
	}


	//ステージに当たっている場合落下しない
	if (position.y<=0)
	{
		//ベロシティＹを０にする
		velocity.y = 0;

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
	}

	//ベロシティの設定
	SetVelocity(velocity);
	//移動する
	SetPosition(GetPosition() + velocity);
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

}
