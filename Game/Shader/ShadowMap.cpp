#include "pch.h"
#include "ShadowMap.h"
#include "VertexTypes.h"

#pragma comment(lib, "d3dcompiler.lib")


ShadowMap::ShadowMap()
	: m_Path{}
	, m_viewPort{}
{
}

// �������֐�
void ShadowMap::Initialize(ID3D11Device* device, const wchar_t* path)
{
	SetDirectory(path);

	// ���_�V�F�[�_�[�̓ǂݍ���
	DX::ThrowIfFailed(D3DReadFileToBlob(GetFullName(L"SM_VS.cso"), m_vsBlob.GetAddressOf()));
	DX::ThrowIfFailed(D3DReadFileToBlob(GetFullName(L"SM_VS_Depth.cso"), m_vsBlobDepth.GetAddressOf()));

	// �s�N�Z���V�F�[�_�[�̓ǂݍ���
	DX::ThrowIfFailed(D3DReadFileToBlob(GetFullName(L"SM_PS_Texture.cso"), m_psBlobTexture.GetAddressOf()));
	DX::ThrowIfFailed(D3DReadFileToBlob(GetFullName(L"SM_PS_NoneTex.cso"), m_psBlobNoneTex.GetAddressOf()));

	// ���_�V�F�[�_�[�̍쐬
	device->CreateVertexShader(m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(), nullptr, m_vertexShader.GetAddressOf());
	device->CreateVertexShader(m_vsBlobDepth->GetBufferPointer(), m_vsBlobDepth->GetBufferSize(), nullptr, m_vertexShaderDepth.GetAddressOf());

	// �s�N�Z���V�F�[�_�[�̍쐬
	device->CreatePixelShader(m_psBlobTexture->GetBufferPointer(), m_psBlobTexture->GetBufferSize(), nullptr, m_pixelShaderTexture.GetAddressOf());
	device->CreatePixelShader(m_psBlobNoneTex->GetBufferPointer(), m_psBlobNoneTex->GetBufferSize(), nullptr, m_pixelShaderNoneTex.GetAddressOf());

	// ���̓��C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		device->CreateInputLayout(
			DirectX::VertexPositionNormalTangentColorTexture::InputElements, DirectX::VertexPositionNormalTangentColorTexture::InputElementCount,
			m_vsBlob->GetBufferPointer(), m_vsBlob->GetBufferSize(),
			m_inputLayout.GetAddressOf()
		)
	);

	// �V���h�E �}�b�v�̍쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = SM_RESOLUTION;   // ��
	descDepth.Height = SM_RESOLUTION;  // ����
	descDepth.MipLevels = 1;       // �~�b�v�}�b�v ���x����
	descDepth.ArraySize = 1;       // �z��T�C�Y
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;  // �t�H�[�}�b�g
	descDepth.SampleDesc.Count = 1;  // �}���`�T���v�����O�̐ݒ�
	descDepth.SampleDesc.Quality = 0;  // �}���`�T���v�����O�̕i��
	descDepth.Usage = D3D11_USAGE_DEFAULT;      // �f�t�H���g�g�p�@
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // �[�x/�X�e���V���A�V�F�[�_ ���\�[�X �r���[�Ƃ��Ďg�p
	descDepth.CPUAccessFlags = 0;   // CPU����̓A�N�Z�X���Ȃ�
	descDepth.MiscFlags = 0;   // ���̑��̐ݒ�Ȃ�
	DX::ThrowIfFailed(device->CreateTexture2D(&descDepth, nullptr, m_shadowMap.GetAddressOf()));

	// �[�x/�X�e���V�� �r���[�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;            // �r���[�̃t�H�[�}�b�g
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	DX::ThrowIfFailed(device->CreateDepthStencilView(m_shadowMap.Get(), &descDSV, m_depthView.GetAddressOf()));

	// �V�F�[�_ ���\�[�X �r���[�̍쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = DXGI_FORMAT_R32_FLOAT; // �t�H�[�}�b�g
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;  // 2D�e�N�X�`��
	srDesc.Texture2D.MostDetailedMip = 0;   // �ŏ��̃~�b�v�}�b�v ���x��
	srDesc.Texture2D.MipLevels = static_cast<UINT>(-1);  // ���ׂẴ~�b�v�}�b�v ���x��
	DX::ThrowIfFailed(device->CreateShaderResourceView(m_shadowMap.Get(), &srDesc, m_shaderView.GetAddressOf()));

	// �r���[�|�[�g�̐ݒ�
	m_viewPort[0].TopLeftX = 0.0f;		// �r���[�|�[�g�̈�̍���X���W�B
	m_viewPort[0].TopLeftY = 0.0f;		// �r���[�|�[�g�̈�̍���Y���W�B
	m_viewPort[0].Width = static_cast<FLOAT>(SM_RESOLUTION);	// �r���[�|�[�g�̈�̕�
	m_viewPort[0].Height = static_cast<FLOAT>(SM_RESOLUTION);	// �r���[�|�[�g�̈�̍���
	m_viewPort[0].MinDepth = 0.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ŏ��l
	m_viewPort[0].MaxDepth = 1.0f;		// �r���[�|�[�g�̈�̐[�x�l�̍ő�l

	// �萔�o�b�t�@�̍쐬
	{
		D3D11_BUFFER_DESC desc = {};
		// �o�b�t�@�T�C�Y�͂P�U�̔{���łȂ��Ƃ����Ȃ�
		size_t size = sizeof(ConstantBuffer);
		if (size % 16) size++;
		desc.ByteWidth = static_cast<UINT>(size * 16);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		DX::ThrowIfFailed(device->CreateBuffer(&desc, nullptr, m_constantBuffer.GetAddressOf()));
	}

	// �V���h�E�}�b�v�p�T���v���[�̍쐬
	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_ANISOTROPIC;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 2;
	descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;
	descSampler.MinLOD = -FLT_MAX;
	descSampler.MaxLOD = FLT_MAX;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	descSampler.BorderColor[0] = 1.0f;
	descSampler.BorderColor[1] = 1.0f;
	descSampler.BorderColor[2] = 1.0f;
	descSampler.BorderColor[3] = 1.0f;
	DX::ThrowIfFailed(device->CreateSamplerState(&descSampler, m_sampler.GetAddressOf()));

}

void ShadowMap::Begin(ID3D11DeviceContext* context)
{
	// �f�o�C�X�R���e�L�X�g�̃N���A
	context->ClearState();

	// �V���h�E�}�b�v���N���A
	context->ClearDepthStencilView(m_depthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	//----- �`��Ώۂ��V���h�E�}�b�v�e�N�X�`���֕ύX -----//

	// �`��^�[�Q�b�g
	ID3D11RenderTargetView* pRender[1] = { nullptr };
	context->OMSetRenderTargets(1, pRender, m_depthView.Get());

	// �r���[�|�[�g
	context->RSSetViewports(1, m_viewPort);
}

void ShadowMap::End(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& lightViewProj)
{
	//----------------------------------//
	// �萔�o�b�t�@��ݒ�               //
	//----------------------------------//
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		DX::ThrowIfFailed(
			context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)
		);

		ConstantBuffer buffer;
		buffer.LightViewProj = XMMatrixTranspose(lightViewProj);

		*static_cast<ConstantBuffer*>(mappedResource.pData) = buffer;
		context->Unmap(m_constantBuffer.Get(), 0);
	}
	//----------------------------------//
	// �萔�o�b�t�@��ݒ�i�I�j         //
	//----------------------------------//
}

void ShadowMap::DrawShadowMap(ID3D11DeviceContext* context)
{
	// ���̓��C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_inputLayout.Get());

	// ���_�V�F�[�_�[
	context->VSSetShader(m_vertexShaderDepth.Get(), nullptr, 0);

	// �s�N�Z���V�F�[�_�[�i�w�肵�Ȃ��j
	context->PSSetShader(nullptr, nullptr, 0);
}

void ShadowMap::DrawShadow(ID3D11DeviceContext* context, bool texture)
{
	// ���̓��C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_inputLayout.Get());

	// ���_�V�F�[�_�[
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->VSSetConstantBuffers(1, 1, m_constantBuffer.GetAddressOf());

	// �s�N�Z���V�F�[�_�[
	if (texture)
	{
		context->PSSetShader(m_pixelShaderTexture.Get(), nullptr, 0);
	}
	else
	{
		context->PSSetShader(m_pixelShaderNoneTex.Get(), nullptr, 0);
	}
	context->PSSetShaderResources(1, 1, m_shaderView.GetAddressOf());
	context->PSSetSamplers(1, 1, m_sampler.GetAddressOf());
}

void ShadowMap::SetDirectory(const wchar_t* path)
{
	if (path && *path != 0)
	{
		wcscpy_s(m_Path, path);
		size_t len = wcsnlen(m_Path, MAX_PATH);
		if (len > 0 && len < (MAX_PATH - 1))
		{
			// Ensure it has a trailing slash
			if (m_Path[len - 1] != L'\\')
			{
				m_Path[len] = L'\\';
				m_Path[len + 1] = 0;
			}
		}
	}
	else
		*m_Path = 0;
}
