#include"pch.h"
#include "Stage.h"


Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model, bool active)
{
	SetPosition(position);
	SetVelocity(velocity);
	SetScale(scale);
	SetRotation(rotation);
	SetModel(model);
	SetActive(active);
}

void Stage::Update(const DX::StepTimer& timer)
{
}

void Stage::Render(const Camera* camera)
{
}

void Stage::Finalize()
{
}

void Stage::Reset()
{
}
