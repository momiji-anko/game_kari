/*
�v���C�V�[��
*/
#pragma once

#include "../IScene.h"
#include "Game/GameMain.h"
#include"Game/GameObject/Player/Player.h"
#include<CommonStates.h>
#include"Game/GameObject/Goal/Goal.h"
#include"Game/SceneGraph/SceneGraph.h"
#include"Game/GameObject/Stage/StageManager.h"
#include"Game/GameObject/Collider/CollisionManager/CollisionManager.h"

class GridFloor;
class PlayerCamera;
class GameMain;


class PlayScene : public IScene
{
private:

	std::unique_ptr<PlayerCamera> m_camera;

	std::unique_ptr<Actor> m_player;
	Actor* m_actor;
	std::unique_ptr<Actor> m_goal;

	std::unique_ptr<SceneGraph> m_sceneGraph;

	std::unique_ptr<StageManager> m_stageManager;

	std::unique_ptr<DirectX::CommonStates> m_commonState;

	GameMain* m_parent;

	std::unique_ptr<CollisionManager> m_collisionManager;

	std::unique_ptr<AABBFor3D> m_area;

public:

	// �R���X�g���N�^
	PlayScene(GameMain* parent);

	// �f�X�g���N�^
	~PlayScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;
};