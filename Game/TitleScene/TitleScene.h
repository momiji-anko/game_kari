/*
�^�C�g���V�[��
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include"Game/IScene.h"
#include "Game/GameMain.h"
#include"Game/Shader/Fade.h"
#include"StageSelect.h"
#include"Game/Shader/Fade.h"
#include"TitleText.h"

class TitleScene : public IScene
{
public:

	// �R���X�g���N�^
	TitleScene(GameMain* parent);

	// �f�X�g���N�^
	~TitleScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

private:
	//�A���t�@�̍ő�l
	static const float ALPHA_MAX_NUM;
	//�A���t�@�̍ŏ��l
	static const float ALPHA_MIN_NUM;

private:

	//�^�C�g���̏��
	enum class TitleState
	{
		//�t�F�[�h�C��
		FADEIN,
		//�^�C�g��
		TITLE,
		//�X�e�[�W�Z���N�g
		STAGESELECT,
		//���[�h�Z���N�g
		MODESELECT,
		//�t�F�[�h�A�E�g
		FADEOUT,
	};

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//�L�[�{�[�h�X�e�[�g�g���b�J�[
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;
	//�X�e�[�W�Z���N�g
	std::unique_ptr<StageSelect> m_stageSelect;
	//�t�F�[�h
	std::unique_ptr<Fade> m_fade;

	// �e�N�X�`��
	//PUSH_SPASE_KEY�̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushTexture;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//PUSH_SPASE_KEY�̃A���t�@�l
	float m_alpha;
	//PUSH_SPASE_KEY�̃A���t�@�ω����x
	float m_alphaSpeed;

	//�^�C�g������
	std::unique_ptr<TitleText> m_titleText;

	//�^�C�g���̏��
	TitleState m_titleSelect;

	GameMain* m_parent;

	std::unique_ptr<Camera> m_camera;

	std::unique_ptr<Actor> m_stageManager;

	float m_cameraAngle;

	int m_previousStageSelectNumber;


};