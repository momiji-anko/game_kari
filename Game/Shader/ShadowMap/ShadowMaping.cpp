#include"pch.h"
#include"ShadowMaping.h"
#include"DeviceResources.h"
#include "VertexTypes.h"

#pragma comment(lib, "d3dcompiler.lib")


ShadowMaping::ShadowMaping()
	:
	m_Path{},
	m_viewPort{}
{
}

void ShadowMaping::Initialize(ID3D11Device* device, const wchar_t* path)
{
	DirectX::SimpleMath::Vector3 lightPosition{ 0,10,0 };
	DirectX::SimpleMath::Vector3 LightTargetPosition=DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 lightUpVector = DirectX::SimpleMath::Vector3::Up;
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::CreateLookAt(lightPosition, LightTargetPosition, lightUpVector);

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// �E�B���h�E�T�C�Y�̎擾
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // ��p
	float aspectRatio = width / height;							// ��ʏc����
	float nearPlane = 1.0f;                                     // �J���������ԋ߂����e��
	float farPlane = 10000.0f;                                    // �J���������ԉ������e��
	DirectX::SimpleMath::Matrix projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	DirectX::SimpleMath::Vector4 clearColor = DirectX::SimpleMath::Vector4::One;
	UINT  rendertarget_HeghtAndWidth = 1028;



	CD3D11_TEXTURE2D_DESC descDe{
		DXGI_FORMAT_R32_TYPELESS,//�t�H�[�}�b�g
		rendertarget_HeghtAndWidth,//Width
		rendertarget_HeghtAndWidth,//Height
		1,//�z��T�C�Y
		1,//�~�b�v�}�b�v
		D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE,//�[�x
		D3D11_USAGE_DEFAULT,
		0,
		1,
		0,
		0
	};
	
	D3D11_TEXTURE2D_DESC descDepth = descDe;


	// �[�x/�X�e���V�� �r���[�̍쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;           
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Flags = 0;
	descDSV.Texture2D.MipSlice = 0;
	DX::ThrowIfFailed(device->CreateDepthStencilView(m_shadowMap.Get(), &descDSV, m_depthView.GetAddressOf()));


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

void ShadowMaping::Begin(ID3D11DeviceContext* context)
{
	
}

void ShadowMaping::End(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& lightViewProj)
{
	
}

void ShadowMaping::DrawShadowMap(ID3D11DeviceContext* context)
{
	
}

void ShadowMaping::DrawShadow(ID3D11DeviceContext* context, bool texture)
{
	
}

void ShadowMaping::SetDirectory(const wchar_t* path)
{
	m_Path = path;

	wchar_t* a = L"\\";

	//�p�X�̖�����\\�����邩�m�F
	if (m_Path.back() != *a)
	{
		m_Path += (L"\\");
	}
}
