#pragma once
#include"StepTimer.h"
#include<SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include<Keyboard.h>
#include "../IScene.h"
#include "Game/GameMain.h"
#include"Game/GameObject/Stage/StageManager.h"

class StageSelect
{
public:
	/// <summary>
   /// �I�������X�e�[�W�ԍ��̎擾
   /// </summary>
   /// <returns>�I�������X�e�[�W�ԍ�</returns>
	int  GetSelectStageNum() { return m_selectStageNum; }

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	StageSelect();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~StageSelect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	/// <returns>���̑I����ʂɍs����</returns>
	bool  Update(const DX::StepTimer& timer);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// �X�e�[�W�I����UI�\��
	/// </summary>
	void StageSelectUIDraw();


private:

	//���W
	DirectX::SimpleMath::Vector2 m_texturePosition;

	//�I�������X�e�[�W�̔ԍ�
	int m_selectStageNum;
	//�X�e�[�W�̍ő吔
	int m_stageMaxNum;


};