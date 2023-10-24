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
	
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());

	m_playerPositions[m_nowIndex] = GameContext::GetInstance().GetPlayerPosition()-DirectX::SimpleMath::Vector3(0, Player::COLLISION_LINE_LENGTH /2.0f,0);
	
	int previousIndex = m_nowIndex-1;

	if (previousIndex < 0)
	{
		previousIndex = m_playerPositions.size() - 1;
	}

	if (m_playerPositions[m_nowIndex] != m_playerPositions[previousIndex])
	{
		m_nowIndex++;
	}



	if (m_isMove)
	{
		Move(timer);
	}

	if (m_nowIndex >= m_playerPositions.size())
	{
		m_nowIndex = 0;
		m_isMove = true;
	}
	

}

void PlayerTrackingEnemy::Render(const Camera* camera)
{
	if (!m_isMove)
		return;

	/*ID3D11Device1* device = pDR->GetD3DDevice();

	static std::shared_ptr<DirectX::BasicEffect> effect;

	if (effect == nullptr)
		effect = std::make_shared<DirectX::BasicEffect>(device);

	effect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
	effect->SetView(camera->GetViewMatrix());
	effect->SetProjection(camera->GetProjectionMatrix());
	effect->SetColorAndAlpha(DirectX::SimpleMath::Vector4(0.5f, 0.0f, 0.5f, 0.5f));

	effect->SetTextureEnabled(false);

	effect->EnableDefaultLighting();

	for (const auto& mesh : GetModel()->meshes)
	{
		for (const auto& part : mesh->meshParts)
		{
			part->ModifyEffect(pDR->GetD3DDevice(), effect);
		}
	}*/


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


void PlayerTrackingEnemy::Move(const DX::StepTimer& timer)
{
	float elapsedTime_s = static_cast<float>(timer.GetElapsedSeconds());


	SetPosition(m_playerPositions[m_index]);

	int nextIndex = m_index + 1;

	if (nextIndex >= m_playerPositions.size())
	{
		nextIndex = 0;
	}

	DirectX::SimpleMath::Vector3 nextvelocity = m_playerPositions[nextIndex] - m_playerPositions[m_index];
	DirectX::SimpleMath::Vector3 angleVelocity = nextvelocity - DirectX::SimpleMath::Vector3(0.0f, nextvelocity.y, 0.0f);

	DirectX::SimpleMath::Quaternion rotation = DirectX::SimpleMath::Quaternion::FromToRotation(DirectX::SimpleMath::Vector3::Forward, angleVelocity);

	m_index++;

	SdkMeshUpdate(&m_animWalkSdk, elapsedTime_s);
	if (std::abs(nextvelocity.y) >= 0.1f)
	{
		SdkMeshUpdate(&m_animJumpSdk, timer.GetElapsedSeconds());

	}
	else if (nextvelocity.Length() > (Player::MOVE_SPEED * 1.4f) * elapsedTime_s)
	{
		SdkMeshUpdate(&m_animRunSdk, timer.GetElapsedSeconds());
	}


	SetRotation(rotation);


	if (m_index >= m_playerPositions.size())
	{
		m_index = 0;
	}
}

void PlayerTrackingEnemy::SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime)
{
	sdkMeshAnimation->Update(elapsedTime);

	size_t modelBone = GetModel()->bones.size();
	sdkMeshAnimation->Apply(*GetModel(), modelBone, m_animBone.get());

}