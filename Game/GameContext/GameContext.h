#pragma once 
#include"Libraries/MyLibraries/Singleton.h"
#include<SpriteBatch.h>
#include<Keyboard.h>
#include<CommonStates.h>
#include"Game/GameObject/Collider/CollisionManager/CollisionManager.h"
#include"Game/GameObject/Enemy/EnemyManager.h"

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
	void SetSpriteBatch(DirectX::SpriteBatch* spriteBath) { m_spriteBath = spriteBath; }
	/// <summary>
	/// �X�v���C�g�o�b�`�̎擾
	/// </summary>
	/// <returns>�X�v���C�g�o�b�`�̐��|�C���^</returns>
	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBath; }

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
	/// <summary>
	/// �G�}�l�[�W���[�ݒ�
	/// </summary>
	/// <param name="manager">�G�}�l�[�W���[���|�C���^</param>
	void SetEnemyManager(EnemyManager* manager) { m_enemyManager = manager; }
	/// <summary>
	/// �G�}�l�[�W���[�擾
	/// </summary>
	/// <returns>�G�}�l�[�W���[���|�C���^</returns>
	EnemyManager* GetEnemyManager() { return m_enemyManager; }

	/// <summary>
	/// �v���C���[�̍��W�ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& position) { m_playerPosition = position; }
	/// <summary>
	/// �v���C���[�̍��W�擾
	/// </summary>
	/// <returns>���W</returns>
	DirectX::SimpleMath::Vector3 GetPlayerPosition() { return m_playerPosition; }

	/// <summary>
	/// �J������Y���p�x��ݒ�
	/// </summary>
	/// <param name="angle">�p�x</param>
	void SetCameraAngleY(float angle) { m_angleY = angle; }
	/// <summary>
	/// �J������Y���p�x�擾
	/// </summary>
	/// <returns>�p�x</returns>
	float GetCmeraAngleY() { return m_angleY; }

	/// <summary>
	/// �v���C���[�����S���Ă��邩�ݒ�
	/// </summary>
	/// <param name="flag">���S���Ă��邩</param>
	void SetPlayerDeath(bool flag) { m_playerDeath = flag; }
	/// <summary>
	/// �v���C���[�����S���Ă��邩�擾
	/// </summary>
	/// <returns>���S���Ă��邩</returns>
	bool IsPlayerDeath() { return m_playerDeath; }

	/// <summary>
	/// �I�������X�e�[�W�ԍ�
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	void SetSelectStageNum(int stageNum) { m_selectStage = stageNum; }
	/// <summary>
	/// �X�e�[�W�ԍ��擾
	/// </summary>
	/// <returns>�X�e�[�W�ԍ�</returns>
	int GetSelectStageNum() { return m_selectStage; }

	/// <summary>
	/// �N���A���Ă��邩�ݒ�
	/// </summary>
	/// <param name="isClear">�N���A���Ă��邩</param>
	void SetIsClear(bool isClear) { m_isClear = isClear; }
	/// <summary>
	/// �N���A���Ă��邩�擾
	/// </summary>
	/// <returns>�N���A���Ă��邩</returns>
	bool ISClear() { return m_isClear; }

	/// <summary>
	/// ���ׂĂ̌����擾���Ă��邩�ݒ�
	/// </summary>
	/// <param name="isAllGetKeys">���ׂĎ擾���Ă��邩</param>
	void SetIsAllGetKey(bool isAllGetKeys) { m_isAllGetKeys = isAllGetKeys; }
	/// <summary>
	/// ���ׂĂ̌����擾���Ă��邩�擾
	/// </summary>
	/// <returns>���ׂĎ擾���Ă��邩</returns>
	bool GetIsAllGetKey() { return m_isAllGetKeys; }

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

	//�J������Y���p�x
	float m_angleY;
	//�v���C���[�����S���Ă��邩
	bool m_playerDeath;
	//�I�����Ă���X�e�[�W�̔ԍ�
	int m_selectStage;
	//�N���A���Ă��邩
	bool m_isClear;
	//���ׂĂ̌����擾���Ă��邩
	bool m_isAllGetKeys;
	//�G�̃}�l�[�W���[
	EnemyManager* m_enemyManager;
};