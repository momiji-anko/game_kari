#include"pch.h"
#include"SceneGraph.h"

#include"Game/GameObject/Stage/StageManager.h"
#include"Game/GameObject/Enemy/EnemyManager.h"
#include"Game/GameObject/Obstacle/ObstacleManager.h"

SceneGraph::SceneGraph()
	:
	m_sceneGraph{},
	m_angle{},
	m_position{}
{
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::Initialize()
{
	std::unique_ptr<Actor> stageManager=std::make_unique<StageManager>(0);
	stageManager->Initialize();
	AttachNode(std::move(stageManager));

	std::unique_ptr<Actor> enemyManager=std::make_unique<EnemyManager>(0);
	enemyManager->Initialize();
	AttachNode(std::move(enemyManager));


	std::unique_ptr<Actor> obstacleManager=std::make_unique<ObstacleManager>(0);
	obstacleManager->Initialize();
	AttachNode(std::move(obstacleManager));

}

void SceneGraph::Update(const DX::StepTimer& timer)
{

	for (std::unique_ptr<Actor>& node : m_sceneGraph)
	{
		node->Update(timer);
	}
	
}

void SceneGraph::Render(const Camera* camera)
{
	for (std::unique_ptr<Actor>& node : m_sceneGraph)
	{
		node->Render(camera);
	}
}

void SceneGraph::Finalize()
{
}

void SceneGraph::Reset()
{
}

void SceneGraph::AttachNode(std::unique_ptr<Actor> sceneNode)
{
	//ÉmÅ[ÉhÇí«â¡Ç∑ÇÈ
	m_sceneGraph.emplace_back(move(sceneNode));
}
