#pragma once 
#include"Libraries/MyLibraries/Singleton.h"
#include<SpriteBatch.h>
#include<Keyboard.h>
#include<CommonStates.h>
#include"Game/GameObject/Collider/CollisionManager/CollisionManager.h"

/// <summary>
/// �Q�[���R���e�L�X�g
/// </summary>
class GameContext :public Singleton<GameContext>
{
public:
	/// <summary>
	/// �X�v���C�g�o�b�`�̐ݒ�
	/// </summary>
	/// <param name="spriteBath">�X�v���C�g�o�b�`�̐��|�C���^</param>
	void SetSpriteBath(DirectX::SpriteBatch* spriteBath) { m_spriteBath = spriteBath; }
	/// <summary>
	/// �X�v���C�g�o�b�`�̎擾
	/// </summary>
	/// <returns>�X�v���C�g�o�b�`�̐��|�C���^</returns>
	DirectX::SpriteBatch* GetSpriteBath() { return m_spriteBath; }

	/// <summary>
	/// �R�����X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="commonState">�R�����X�e�[�g�̐��|�C���^</param>
	void SetCommonState(DirectX::CommonStates* commonState) { m_commonStates = commonState; }
	/// <summary>
	/// �R�����X�e�[�g�̎擾
	/// </summary>
	/// <returns>�R�����X�e�[�g�̐��|�C���^</returns>
	DirectX::CommonStates* GetCommonState() { return m_commonStates; }

	/// <summary>
	/// �L�[�{�[�h�X�e�[�g�g���b�J�[�̐ݒ�
	/// </summary>
	/// <param name="keyboard">�L�[�{�[�h�X�e�[�g�g���b�J�[�̐��|�C���^</param>
	void SetKeyboardStateTracker(DirectX::Keyboard::KeyboardStateTracker* keyboard) { m_keyboardTracker = keyboard; }
	/// <summary>
	/// �L�[�{�[�h�X�e�[�g�g���b�J�[�̎擾
	/// </summary>
	/// <returns></returns>
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardStateTracker() { return m_keyboardTracker; }

	/// <summary>
	/// �R���W�����}�l�[�W���[�̐ݒ�
	/// </summary>
	/// <param name="manager">�R���W�����}�l�[�W���[�̐��|�C���^</param>
	void SetCollisionManager(CollisionManager* manager) { m_collisionManager = manager; }
	/// <summary>
	/// �R���W�����}�l�[�W���[�̎擾
	/// </summary>
	/// <returns>�R���W�����}�l�[�W���[�̐��|�C���^</returns>
	CollisionManager* GetCollisionManager() { return m_collisionManager; }

	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& position) { m_playerPosition = position; }
	DirectX::SimpleMath::Vector3 GetPlayerPosition() { return m_playerPosition; }

	void SetCameraAngleY(float angle) { m_angleY = angle; }
	float GetCmeraAngleY() { return m_angleY; }

	void SetPlayerDeath(bool flag) { m_playerDeath = flag; }
	bool IsPlayerDeath() { return m_playerDeath; }

	void SetSelectStageNum(int stageNum) { m_selectStage = stageNum; }
	int GetSelectStageNum() { return m_selectStage; }

	void SetIsClear(bool isClear) { m_isClear = isClear; }
	bool ISClear() { return m_isClear; }

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameContext();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameContext();

private:
	//�X�v���C�g�o�b�`
	DirectX::SpriteBatch* m_spriteBath;
	//�R�����X�e�[�g
	DirectX::CommonStates* m_commonStates;
	//�L�[�{�[�h�X�e�[�g�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker* m_keyboardTracker;
	//�R���W�����}�l�[�W���[
	CollisionManager* m_collisionManager;

	DirectX::SimpleMath::Vector3 m_playerPosition;

	float m_angleY;

	bool m_playerDeath;

	int m_selectStage;

	bool m_isClear;
};