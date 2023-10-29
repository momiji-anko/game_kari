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
#include<SpriteBatch.h>
#include"Game/Shader/Fade.h"

class GridFloor;
class PlayerCamera;
class GameMain;


class PlayScene : public IScene
{

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
private:
	//�J����
	std::unique_ptr<PlayerCamera> m_camera;

	//�v���C���[
	Actor* m_actor;

	//�e
	GameMain* m_parent;

	//�t�F�[�h
	std::unique_ptr<Fade> m_fade;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	//�V�[���O���t
	std::unique_ptr<SceneGraph> m_sceneGraph;
	//�R���W�����}�l�[�W���[
	std::unique_ptr<CollisionManager> m_collisionManager;


	//�������S�G���A
	std::unique_ptr<AABBFor3D> m_fallDeathArea;

	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;


};