/*
* 2023/05/16
* Fade.cpp
* �t�F�[�h
* �����@��
*/
#include "pch.h"
#include "Fade.h"
#include"BinaryFile.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include<Mouse.h>
#include<Keyboard.h>
#include"DeviceResources.h"

//�t�F�[�h�A�E�g�I���l
const float Fade::FADEOUT_END_NUM = 1.0f;

//�t�F�[�h�C���I���l
const float Fade::FADEIN_END_NUM = 0.0f;

const std::vector<D3D11_INPUT_ELEMENT_DESC> Fade::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// �R���X�g���N�^
/// </summary>
Fade::Fade():
	m_CBuffer{},
	m_timer{},
	m_inputLayout{},
	m_batch{},
	m_states{},
	m_texture{},
	m_vertexShader{},
	m_pixelShader{},
	m_geometryShader{},
	m_centerPosition{},
	m_billboardTranslation{},
	m_view{},
	m_proj{},
	m_fadeTime_s{},
	m_fadeState{}
{
	
	
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Fade::~Fade()
{
	Lost();
}
//-----------------------------------------------------------------------------------
//	Create()
//	�V�F�[�_���𐶐�
//	IN	:	�f�o�C�X			DX::DeviceResources* deviceResources
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/FadeVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/FadePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/FadeGS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf()
	);
	

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	const wchar_t* name = L"Resources\\Textures\\089.png";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name, nullptr, m_texture.GetAddressOf());



	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<DirectX::CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage		=	D3D11_USAGE_DEFAULT;
	bd.ByteWidth	=	sizeof(ConstBuffer);
	bd.BindFlags	=	D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;	
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);




}
//-----------------------------------------------------------------------------------
//	SetRenderState()
//	�J�����A�r���[�A�ˉe�̔��f
//	IN	:	�J����		DirectX::SimpleMath::Vector3 camera
//			�r���[�s��	DirectX::SimpleMath::Matrix view
//			�ˉe�s��	DirectX::SimpleMath::Matrix proj
//	RE	:	void
//-----------------------------------------------------------------------------------
void Fade::SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	m_view = view;
	m_proj = proj;

	m_billboardTranslation = DirectX::SimpleMath::Matrix::CreateBillboard(m_centerPosition, camera, DirectX::SimpleMath::Vector3::UnitY);

	//�r���{�[�h�̌v�Z�ŗ��Ԃ�̂ŕ␳
	//Y����180�x��]����s��
	DirectX::SimpleMath::Matrix rev = DirectX::SimpleMath::Matrix::Identity;
	rev._11 = -1;
	rev._33 = -1;

	//�␳�s����ɂ����đ��ɉe�����Ȃ��悤�ɂ���
	m_billboardTranslation = rev * m_billboardTranslation;
}

/// <summary>
/// ������
/// </summary>
/// <param name="pos">���W</param>
/// <param name="fade">�t�F�[�h�A���t�@</param>
void Fade::Initialize(const DirectX::SimpleMath::Vector3& pos, float fade)
{
	m_centerPosition = pos;
	m_fadeTime_s = fade;
	m_fadeState = State::FADE_NONE;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void Fade::Update(DX::StepTimer timer)
{
	//�^�C�}�[�̑��
	m_timer = timer;
	
	//�o�ߎ���
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	//�t�F�[�h��Ԃɂ���ď�����ς���
	switch (m_fadeState)
	{
	//�t�F�[�h�C��
	case Fade::State::FADE_IN:
		//�t�F�[�h�^�C�����o�ߎ��Ԃň���
		m_fadeTime_s -= elapsedTime;

		//�t�F�[�h�^�C�����O�ȉ��ɂȂ����牽�����Ȃ�
		if (m_fadeTime_s <= FADEIN_END_NUM)
		{
			m_fadeState = State::FADE_NONE;
		}

		break;
	//�t�F�[�h�C��
	case Fade::State::FADE_OUT:
		//�t�F�[�h�^�C�����o�ߎ��Ԃő���
		m_fadeTime_s += elapsedTime;

		//�t�F�[�h�^�C�����P�ȏ�ɂȂ����牽�����Ȃ�
		if (m_fadeTime_s >= FADEOUT_END_NUM)
		{
			m_fadeState = State::FADE_NONE;
		}
		break;

	case Fade::State::FADE_NONE:
		break;
	default:
		break;
	}
}

/// <summary>
/// �`��
/// </summary>
void Fade::Render()
{

	//�S��ʃG�t�F�N�g
	DirectX::SimpleMath::Matrix  mat = DirectX::SimpleMath::Matrix::Identity;
	Draw(mat, mat, mat);

	//�|���S���G�t�F�N�g
	//Draw(m_billboardTranslation, m_view, m_proj);
}

/// <summary>
/// ���C���`�揈��
/// </summary>
/// <param name="world">���[���h�s��</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�v���W�F�N�V�����s��</param>
void Fade::Draw(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// ���_���(�|���S���̒��_�̍��W���j
	DirectX::VertexPositionColorTexture vertex[1] =
	{
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f),DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
	};

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();

	cbuff.Time = DirectX::SimpleMath::Vector4((float)m_timer.GetTotalSeconds(), m_fadeTime_s, DirectX::Mouse::Get().GetState().x / 800.0f, DirectX::Mouse::Get().GetState().y / 600.0f);
	
	
	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthDefault(), 0);

	// �J�����O�͔����v���
	context->RSSetState(m_states->CullCounterClockwise());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());


	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);

}

/// <summary>
/// �폜
/// </summary>
void Fade::Lost()
{
	m_CBuffer.Reset();
	m_inputLayout.Reset();
	m_batch.reset();
	m_states.reset();
	m_texture.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
}
