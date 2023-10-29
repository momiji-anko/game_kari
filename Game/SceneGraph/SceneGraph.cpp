#include"pch.h"
#include"SceneGraph.h"

#include"Game/GameObject/Stage/StageManager.h"
#include"Game/GameObject/Enemy/EnemyManager.h"
#include"Game/GameObject/Obstacle/ObstacleManager.h"

/// <summary>
/// コンテキスト
/// </summary>
SceneGraph::SceneGraph()
	:
	m_sceneGraph{},
	m_position{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneGraph::~SceneGraph()
{
}

/// <summary>
/// 初期化	
/// </summary>
void SceneGraph::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void SceneGraph::Update(const DX::StepTimer& timer)
{
	//更新
	for (std::unique_ptr<Actor>& node : m_sceneGraph)
	{
		node->Update(timer);
	}
	
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void SceneGraph::Render(const Camera* camera)
{
	//描画
	for (std::unique_ptr<Actor>& node : m_sceneGraph)
	{
		node->Render(camera);
	}
}

/// <summary>
/// 終了処理
/// </summary>
void SceneGraph::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void SceneGraph::Reset()
{
}

/// <summary>
/// ノードを追加する
/// </summary>
/// <param name="sceneNode">ノード</param>
void SceneGraph::AttachNode(std::unique_ptr<Actor> sceneNode)
{
	//ノードを追加する
	m_sceneGraph.emplace_back(move(sceneNode));
}
