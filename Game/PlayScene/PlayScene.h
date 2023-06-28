/*
�v���C�V�[��
*/
#pragma once

#include "../IScene.h"
#include "Game/GameMain.h"

class GridFloor;
class Camera;

class PlayScene : public IScene
{
private:

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