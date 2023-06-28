/*
* 2023/03/10
* FireShader.h
* �t�@�C���[�V�F�[�_�[
* �����@��
*/
#pragma once

#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

/// <summary>
/// �t�@�C���[�V�F�[�_�[
/// </summary>
class FireShader
{
public:
	//�C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	/// <summary>
	/// �R���X�g�o�b�t�@�[
	/// </summary>
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	FireShader();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FireShader();

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
	void Initialize(float life, const DirectX::SimpleMath::Vector3& pos,const DirectX::SimpleMath::Vector3& velocity);
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�X�V</param>
	void Update(DX::StepTimer timer);
	/// <summary>
	/// �`��
	/// </summary>
	void Render();
	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();
	/// <summary>
	/// ����
	/// </summary>
	void Lost();

	/// <summary>
	/// �`��ݒ�
	/// </summary>
	/// <param name="camera">�J�����̍��W</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�v���W�F�N�V�����s��</param>
	void SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	DirectX::SimpleMath::Vector3 GetPosition() 
	{
		return m_position;
	}

	/// <summary>
	/// ���`���
	/// </summary>
	float Lerp(float start, float end, float t)
	{
		return start + (end - start) * t;
	}
	/// <summary>
	/// �������W
	/// </summary>
	/// <param name="pos">���W</param>
	void SetStartPos(const DirectX::SimpleMath::Vector3& pos) { m_startPosition = pos; }

	/// <summary>
	/// �I�t�Z�b�g���W�ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetOffsetPosition(const DirectX::SimpleMath::Vector3& pos) { m_offsetPos = pos; }

private:
	/// <summary>
	/// �V�F�[�_�[�`��
	/// </summary>
	void ShaderDraw();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	//�V�F�[�_�[�ɓn���o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	//�^�C�}�[
	DX::StepTimer                           m_timer;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_states;
	
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader>   m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader>    m_pixelShader;
	//�W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	//���[���h�s��
	DirectX::SimpleMath::Matrix  m_world;
	//�r���[�s��
	DirectX::SimpleMath::Matrix  m_view;
	//�v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix  m_proj;
	//�J�����̍��W
	DirectX::SimpleMath::Vector3 m_camera;
	//���W
	DirectX::SimpleMath::Vector3 m_position;
	//�x���V�e�B
	DirectX::SimpleMath::Vector3 m_velocity;
	//����
	float						 m_life;
	//�������W						 
	DirectX::SimpleMath::Vector3 m_startPosition;
	//�����x���V�e�B
	DirectX::SimpleMath::Vector3 m_startVelocity;
	//��������
	float						 m_startLife;
	//�E�F�C�g						 
	float						 m_wait;
								 
	//�|�W�V�������X�g
	std::vector < DirectX::SimpleMath::Vector3 > m_positionList;
	//��]
	float m_rot;
	//�p�x
	float m_angle;
	//�g�k	
	float m_scale;
	//�I�t�Z�b�g���W
	DirectX::SimpleMath::Vector3 m_offsetPos;

};