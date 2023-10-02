#pragma once
#include<SimpleMath.h>
#include <d3dcompiler.h>


class PlayerEffect
{
public:
	PlayerEffect();



private:
	// cso�̏ꏊ
	std::wstring m_Path;

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