#include"pch.h"
#include"DeviceResources.h"

#include"PlayerTrackingEnemy.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/Camera.h"
#include"Libraries/MyLibraries/InputKeyTime.h"
#include"Game/GameObject/Player/Player.h"
#include<Effects.h>

PlayerTrackingEnemy::PlayerTrackingEnemy(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model,float second)
	:
	Actor{ position, velocity,scale,rotation,model,true },
	m_playerPositions{},
	m_nowIndex{0},
	m_index{1},
	m_isMove{false}
{
	m_playerPositions.resize(60 * second);

	for (DirectX::SimpleMath::Vector3& pos : m_playerPositions)
	{
		pos = DirectX::SimpleMath::Vector3::Zero;
	}
}

PlayerTrackingEnemy::~PlayerTrackingEnemy()
{
}

void PlayerTrackingEnemy::Initialize()
{

	CreateSdkMesh();

	
}

void PlayerTrackingEnemy::Update(const DX::StepTimer& timer)
{
	
	m_playerPositions[m_nowIndex] = GameContext::GetInstance().GetPlayerPosition()-DirectX::SimpleMath::Vector3(0, 3.0f /2.0f,0);
	
	m_nowIndex++;

	

	if (m_isMove)
	{
		 SetPosition(m_playerPositions[m_index]);

		 m_index++;

		 int nextIndex = m_index ;

		 if (nextIndex >= m_playerPositions.size())
		 {
			 nextIndex = 0;
		 }

		 DirectX::SimpleMath::Vector3 nextvelocity = m_playerPositions[nextIndex] - m_playerPositions[m_index - 1];
		 nextvelocity.y = 0;
		 DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::FromToRotation(DirectX::SimpleMath::Vector3::Forward, nextvelocity);

		 SetRotation(rotation);


		 if (m_index >= m_playerPositions.size())
		 { 
			 m_index = 0;
		 }
	}

	if (m_nowIndex >= m_playerPositions.size())
	{
		m_nowIndex = 0;
		m_isMove = true;
	}
	SdkMeshUpdate(&m_animWalkSdk, timer.GetElapsedSeconds());

}

void PlayerTrackingEnemy::Render(const Camera* camera)
{
	if (!IsActive())
		return;


	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	CalculateWorldMatrix();

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

	ModelManager::GetInstance().LoadCmoModel(L"dice.cmo")->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

}

void PlayerTrackingEnemy::Finalize()
{
}

void PlayerTrackingEnemy::Reset()
{
}

void PlayerTrackingEnemy::CreateSdkMesh()
{
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


	GetModel()->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto skin = dynamic_cast<DirectX::SkinnedEffect*>(effect);
			if (skin)
			{
				skin->SetPerPixelLighting(true);
			}
		});

	m_animWalkSdk.Bind(*GetModel());
	m_animIdleSdk.Bind(*GetModel());
	m_animJumpSdk.Bind(*GetModel());
	m_animDieSdk.Bind(*GetModel());
	m_animRunSdk.Bind(*GetModel());
}

//float PlayerTrackingEnemy::MovedAngle(const DX::StepTimer& timer)
//{
//	//90度
//	static const float NINETY_ANGLE = DirectX::XM_PI / 2.0f;
//
//	float playerMoveAngle = GetRotation().ToEuler().y;
//
//	//右キーを押していた場合右に移動＆右を向く
//	if (IsPlayerKeyPushTime(timer.GetTotalSeconds(),DirectX::Keyboard::D))
//	{
//		playerMoveAngle = -NINETY_ANGLE;
//
//		m_isMove = true;
//		
//	}
//	//左キーを押していた場合右に移動＆左を向く
//	else if (IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::A))
//	{
//
//		playerMoveAngle = NINETY_ANGLE;
//
//		m_isMove = true;
//	}
//
//	//後ろキーを押していた場合右に移動＆後ろを向く
//	if (IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::S))
//	{
//		playerMoveAngle = NINETY_ANGLE * 2.0f;
//
//		m_isMove = true;
//	}
//	//前キーを押していた場合右に移動＆前を向く
//	else if (IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::W))
//	{
//		playerMoveAngle = 0;
//
//		m_isMove = true;
//	}
//
//	//左キーと前キーを押していた場合左前を向く
//	if ((IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::A)) && (IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::W)))
//	{
//		//45
//		playerMoveAngle = NINETY_ANGLE / 2.0f;
//	}
//
//	//左キーと後ろキーを押していた場合左後ろを向く
//	if ((IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::A)) && (IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::S)))
//	{
//		//90+45
//		playerMoveAngle = NINETY_ANGLE + NINETY_ANGLE / 2.0f;
//	}
//
//	//右キーと前キーを押していた場合右前を向く
//	if ((IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::D)) && (IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::W)))
//	{
//		//-45
//		playerMoveAngle = -NINETY_ANGLE / 2.0f;
//	}
//
//	//右キーと後ろキーを押していた場合右後ろを向く
//	if ((IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::D)) && (IsPlayerKeyPushTime(timer.GetTotalSeconds(), DirectX::Keyboard::S)))
//	{
//		//-(90+45)
//		playerMoveAngle = -(NINETY_ANGLE + NINETY_ANGLE / 2.0f);
//	}
//
//	return playerMoveAngle;
//}

void PlayerTrackingEnemy::SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime)
{
	sdkMeshAnimation->Update(elapsedTime);

	size_t modelBone = GetModel()->bones.size();
	sdkMeshAnimation->Apply(*GetModel(), modelBone, m_animBone.get());

}