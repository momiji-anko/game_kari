#include"pch.h"
#include"Goal.h"
#include"Game/GameContext/GameContext.h"
#include"DeviceResources.h"
#include"Libraries/MyLibraries/Camera.h"

Goal::Goal(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model, bool active)
	:
	Actor(position, velocity, scale, rotation, model, active)
{

}

Goal::~Goal()
{
}

void Goal::Initialize()
{
	if(GameContext::GetInstance().GetCollisionManager()!=nullptr)
		GameContext::GetInstance().GetCollisionManager()->SetGoalAABB(nullptr);

	AABBFor3D* aabb = GetAABB();
	DirectX::SimpleMath::Vector3 position = GetPosition();
	DirectX::SimpleMath::Vector3 area{ 0.5 };
	aabb->SetData(position - area, position + area);

	Capsule* capsule = GetCapsule();
	capsule->a = DirectX::SimpleMath::Vector3(0.f, -1, 0.f) + position;
	capsule->b = DirectX::SimpleMath::Vector3(0.f, 1, 0.f) + position;

}

void Goal::Update(const DX::StepTimer& timer)
{
	if(GameContext::GetInstance().GetIsAllGetKey() && GameContext::GetInstance().GetCollisionManager() != nullptr)
		GameContext::GetInstance().GetCollisionManager()->SetGoalAABB(GetAABB());

	
}

void Goal::Render(const Camera* camera)
{

	//デバイスリソースの取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	//デバイスコンテキストの取得
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	CalculateWorldMatrix();

	
	GetModel()->Draw(context, *GameContext::GetInstance().GetCommonState(), GetWorldMatrix(), camera->GetViewMatrix(), camera->GetProjectionMatrix());

}

void Goal::Finalize()
{
}

void Goal::Reset()
{
}
