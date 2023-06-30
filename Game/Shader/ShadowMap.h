#pragma once

#include <d3dcompiler.h>
#include "SimpleMath.h"

class ShadowMap
{

private:

	// �p�X�t���̖��O�ɕϊ�����֐�
	wchar_t* GetFullName(const wchar_t* name)
	{
		static wchar_t fullName[MAX_PATH];

		wcscpy_s(fullName, m_Path);
		wcscat_s(fullName, name);

		return fullName;
	}

public:

	// �R���X�g���N�^
	ShadowMap();

	// �������֐�
	void Initialize(ID3D11Device* device, const wchar_t* path = nullptr);

	// �V���h�E�}�b�v�쐬�J�n
	void Begin(ID3D11DeviceContext* context);

	// �V���h�E�}�b�v�쐬�I��
	void End(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& lightViewProj);

	// �V���h�E�}�b�v�쐬���ɌĂяo���J�X�^���X�e�[�g
	void DrawShadowMap(ID3D11DeviceContext* context);

	// �e�t���̃��f����`�悵�������ɌĂяo���J�X�^���X�e�[�g
	// �e�N�X�`���Ȃ��̃��f����\���������ꍇ�͑�Q������false�ɂ��鎖
	void DrawShadow(ID3D11DeviceContext* context, bool texture = true);

	// �V���h�E�}�b�v�p�e�N�X�`�����\�[�X���擾����֐�
	ID3D11ShaderResourceView* GetShadowMapTexture()
	{
		return m_shaderView.Get();
	}
	
	void SetDirectory(const wchar_t* path);

private:

	// �V���h�E�}�b�v�̉𑜓x
	static const int SM_RESOLUTION = 1024;

	// cso�̏ꏊ
	wchar_t m_Path[MAX_PATH];

	// ���_�V�F�[�_�[�̃f�[�^
	Microsoft::WRL::ComPtr<ID3DBlob> m_vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_vsBlobDepth;

	// �s�N�Z���V�F�[�_�[�̃f�[�^
	Microsoft::WRL::ComPtr<ID3DBlob> m_psBlobTexture;
	Microsoft::WRL::ComPtr<ID3DBlob> m_psBlobNoneTex;

	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShaderDepth;

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShaderTexture;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShaderNoneTex;

	// �V���h�E�}�b�v�p�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_shadowMap;

	// �[�x�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthView;

	// �V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderView;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �V���h�E�}�b�v�쐬���̃r���[�|�[�g
	D3D11_VIEWPORT m_viewPort[1];

	// �萔�o�b�t�@�̍\���̂̒�`
	struct ConstantBuffer
	{
		DirectX::XMMATRIX LightViewProj;
	};

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// �T���v���[
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;

};

