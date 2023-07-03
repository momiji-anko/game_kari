/*
�v���C�V�[��
*/
#pragma once

#include "../IScene.h"
#include "Game/GameMain.h"
#include"Game/GameObject/Player/Player.h"
#include<CommonStates.h>
#include"Game/GameObject/Goal/Goal.h"

class GridFloor;
class Camera;

class PlayScene : public IScene
{
private:

	std::unique_ptr<Camera> m_camera;

	std::unique_ptr<Player> m_player;

	std::unique_ptr<Goal> m_goal;

	std::unique_ptr<DirectX::CommonStates> m_commonState;

public:

	// �R���X�g���N�^
	PlayScene();

	// �f�X�g���N�^
	~PlayScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;
};