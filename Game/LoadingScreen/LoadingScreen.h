/*
���[�h���
*/
#pragma once


#include <thread>
#include <atomic>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include "../ILoadingScreen.h"

#include<SimpleMath.h>
#include<vector>

class LoadingScreen : public ILoadingScreen
{

public:

	// �R���X�g���N�^
	LoadingScreen();

	// �f�X�g���N�^
	~LoadingScreen();

	// ������
	void Initialize(IScene* targetScene) override;

	// �X�V
	void Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	// �ǂݍ��ݒ����ǂ����̎擾
	bool IsLoading();

private:
	// �ΏۃV�[���̃��\�[�X�̓ǂݍ���
	void LoadSceneResources(IScene* targetScene) override;

private:

	// ���\�[�X�ǂݍ��ݗp
	std::thread m_loadingThread;
	std::atomic<bool> m_isLoading;

	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// �e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	DirectX::SimpleMath::Vector2 m_texPosition;
	std::vector<DirectX::SimpleMath::Vector2> m_texturePositions;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_blackTexture;

};