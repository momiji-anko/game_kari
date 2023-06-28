/*
* 2023/05/16
* Fade.h
* �t�F�[�h
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
#include <Model.h>
#include <vector>

/// <summary>
/// �t�F�[�h
/// </summary>
class Fade
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
		DirectX::SimpleMath::Vector4	Time;
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Fade();
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Fade();

	/// <summary>
	/// �V�F�[�_�[�쐬
	/// </summary>
	void Create();
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="fade">�t�F�[�h�A���t�@</param>
	void Initialize(const DirectX::SimpleMath::Vector3& pos,float fade = 1.0f);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void Update(DX::StepTimer timer);

	/// <summary>
	/// �`��
	/// </summary>
	void Render();

	/// <summary>
	/// �폜
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
	/// ���C���`�揈��
	/// </summary>
	/// <param name="world">���[���h�s��</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="proj">�v���W�F�N�V�����s��</param>
	void Draw(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

	/// <summary>
	/// �t�F�[�h�C��
	/// </summary>
	void FadeIn()  { m_fadeState = State::FADE_IN; }
	/// <summary>
	/// �t�F�[�h�A�E�g
	/// </summary>
	void FadeOut() { m_fadeState = State::FADE_OUT; }

	/// <summary>
	/// �t�F�[�h�A�E�g���Ă��邩�ǂ���
	/// </summary>
	/// <returns>true=�t�F�[�h�A�E�g���Ă���Afalse=�t�F�[�h�A�E�g���Ă��Ȃ�</returns>
	bool ISClose() { return m_fadeTime_s >= 1; }
	/// <summary>
	/// �t�F�[�h�C�����Ă��邩�ǂ���
	/// </summary>
	/// <returns>true=�t�F�[�h�C�����Ă���Afalse=�t�F�[�h�C�����Ă��Ȃ�</returns>
	bool ISOpen()  { return m_fadeTime_s <= 0; }

private:
	//�t�F�[�h�A�E�g�I���l
	static const float FADEOUT_END_NUM;
	//�t�F�[�h�C���I���l
	static const float FADEIN_END_NUM;

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
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	//�J�����̍��W
	DirectX::SimpleMath::Vector3 m_centerPosition;
	//�r���{�[�h�p�s��
	DirectX::SimpleMath::Matrix m_billboardTranslation;
	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//�v���W�F�N�V�����s��
	DirectX::SimpleMath::Matrix m_proj;


	//�t�F�[�h�̏��
	enum class State
	{
		FADE_IN,
		FADE_OUT,
		FADE_NONE,
	};
	//�t�F�[�h�^�C��
	float m_fadeTime_s;
	//�t�F�[�h�̏��
	State m_fadeState;
};