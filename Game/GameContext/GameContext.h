#pragma once 
#include"Libraries/MyLibraries/Singleton.h"
#include<SpriteBatch.h>
#include<Keyboard.h>
#include<CommonStates.h>

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
	void SetKeyBoardStateTracker(DirectX::Keyboard::KeyboardStateTracker* keyboard) { m_keyboardTracker = keyboard; }
	/// <summary>
	/// �L�[�{�[�h�X�e�[�g�g���b�J�[�̎擾
	/// </summary>
	/// <returns></returns>
	DirectX::Keyboard::KeyboardStateTracker* GetKeyBoardStateTracker() { return m_keyboardTracker; }
private:
	//�X�v���C�g�o�b�`
	DirectX::SpriteBatch* m_spriteBath;
	//�R�����X�e�[�g
	DirectX::CommonStates* m_commonStates;
	//�L�[�{�[�h�X�e�[�g�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker* m_keyboardTracker;

};