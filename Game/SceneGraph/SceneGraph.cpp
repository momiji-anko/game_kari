#include"pch.h"
#include"SceneGraph.h"

#include"Game/GameObject/Stage/StageManager.h"
#include"Game/GameObject/Enemy/EnemyManager.h"
#include"Game/GameObject/Obstacle/ObstacleManager.h"

/// <summary>
/// �R���e�L�X�g
/// </summary>
SceneGraph::SceneGraph()
	:
	m_sceneGraph{},
	m_position{}
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneGraph::~SceneGraph()
{
}

/// <summary>
/// ������	
/// </summary>
void SceneGraph::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void SceneGraph::Update(const DX::StepTimer& timer)
{
	//�X�V
	for (std::unique_ptr<Actor>& node : m_sceneGraph)
	{
		node->Update(timer);
	}
	
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void SceneGraph::Render(const Camera* camera)
{
	//�`��
	for (std::unique_ptr<Actor>& node : m_sceneGraph)
	{
		node->Render(camera);
	}
}

/// <summary>
/// �I������
/// </summary>
void SceneGraph::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void SceneGraph::Reset()
{
}

/// <summary>
/// �m�[�h��ǉ�����
/// </summary>
/// <param name="sceneNode">�m�[�h</param>
void SceneGraph::AttachNode(std::unique_ptr<Actor> sceneNode)
{
	//�m�[�h��ǉ�����
	m_sceneGraph.emplace_back(move(sceneNode));
}
