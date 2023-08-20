#include"pch.h"
#include"SceneGraph.h"

#include"Game/GameObject/Stage/StageManager.h"
#include"Game/GameObject/Enemy/EnemyManager.h"

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
	//ノードを追加する
	m_sceneGraph.emplace_back(move(sceneNode));
}
