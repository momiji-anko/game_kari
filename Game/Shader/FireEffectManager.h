//-----------------------------------
//	�G�t�F�N�g�Ǘ��N���X	
//	FireEffectManager.h
//-----------------------------------
#pragma once

#include "StepTimer.h"

#include <SimpleMath.h>
#include <list>
#include <WICTextureLoader.h>

#include"FireShader.h"

/// <summary>
/// �t�@�C���[�G�t�F�N�g�Ǘ��N���X
/// </summary>
class FireEffectManager
{
public:
	/// <summary>
	/// �`��ݒ�
	/// </summary>
	/// <param name="camera">�J�����̍��W</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�v���W�F�N�V�����s��</param>
	void SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// �������W
	/// </summary>
	/// <param name="pos">���W</param>
	void SetStartPosition(const DirectX::SimpleMath::Vector3& pos);

	/// <summary>
	/// �I�t�Z�b�g���W�ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetOffsetPosition(const DirectX::SimpleMath::Vector3& pos);

public:
	/// <summary>
	/// �V�F�[�_�[�̍쐬
	/// </summary>
	void Create();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="life">����</param>
	/// <param name="pos">���W</param>
	/// <param name="velocity">�ړ���</param>
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�X�V</param>
	void Update(const DX::StepTimer& timer);
	/// <summary>
	/// �`��
	/// </summary>
	void Render();

private:

	//�o�b�`�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;

	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//�G�t�F�N�g�̃|�C���^
	std::list<std::unique_ptr<FireShader>> m_effectList;
	//�e�N�X�`���f�[�^
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;
};