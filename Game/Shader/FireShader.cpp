/*
* 2023/03/10
* FireShader.cpp
* �t�@�C���[�V�F�[�_�[
* �����@��
*/
#include "pch.h"
#include "FireShader.h"
#include "BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include"Libraries/MyLibraries/MyRandom.h"



const std::vector<D3D11_INPUT_ELEMENT_DESC> FireShader::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// �R���X�g���N�^
/// </summary>
FireShader::FireShader()
	:
	m_scale(1.0f),
	m_CBuffer{},
	m_inputLayout{},
	m_batch{},
	m_states{},
	m_texture{},
	m_vertexShader{},
	m_pixelShader{},
	m_geometryShader{},
	m_world{},
	m_view{},
	m_proj{},
	m_camera{},
	m_position{},
	m_velocity{},
	m_life{},
	m_startPosition{},
	m_startVelocity{},
	m_startLife{},
	m_wait{},
	m_positionList{},
	m_rot{},
	m_angle{},
	m_offsetPos{}
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
FireShader::~FireShader()
{
	Lost();
}
//-----------------------------------------------------------------------------------
//	Create()
//	�V�F�[�_���𐶐�
//	IN	:	�f�o�C�X			DX::DeviceResources* deviceResources
//	RE	:	void
//-----------------------------------------------------------------------------------
void FireShader::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto device = pDR->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}

	const wchar_t* name = L"Resources\\Textures\\6.bmp";
	DirectX::CreateWICTextureFromFile(pDR->GetD3DDevice(), name, nullptr, m_texture.GetAddressOf());


	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(pDR->GetD3DDeviceContext());

	m_states = std::make_unique<DirectX::CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	m_world = DirectX::SimpleMath::Matrix::Identity;


	m_angle = 0.0f;

	int count = 8;

	for (int i = 0; i < count; i++)
	{
		m_positionList.push_back(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 1.0f));
	}

}
//-----------------------------------------------------------------------------------
//	SetRenderState()
//	�J�����A�r���[�A�ˉe�̔��f
//	IN	:	�J����		DirectX::SimpleMath::Vector3 camera
//			�r���[�s��	DirectX::SimpleMath::Matrix view
//			�ˉe�s��	DirectX::SimpleMath::Matrix proj
//	RE	:	void
//-----------------------------------------------------------------------------------
void FireShader::SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	m_camera = camera;
	m_view = view;
	m_proj = proj;
}

/// <summary>
/// ������
/// </summary>
/// <param name="life">����</param>
/// <param name="pos">���W</param>
/// <param name="velocity">�ړ���</param>
void FireShader::Initialize(float life, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& velocity)
{
	//������
	m_startPosition = pos;
	m_startVelocity = velocity;
	m_startLife = life;


	//�V�[�h�l
	int seed = 100;
	m_wait = ((rand() % seed) / (float)seed) * m_startLife;

	//���Z�b�g
	Reset();
}

void FireShader::Reset()
{
	m_position = m_startPosition;
	m_velocity = m_startVelocity;
	m_life = m_startLife;
	m_scale = 0.2f;
}
//-----------------------------------------------------------------------------------
//	Update()
//	�X�V����
//	IN	:	�^�C�}�[		DX::StepTimer timer
//	RE	:	void
//-----------------------------------------------------------------------------------
void FireShader::Update(DX::StepTimer timer)
{
	//�����Ɏg���b���v(1�b��1.0f)���擾����B
	float time = static_cast<float>(timer.GetElapsedSeconds());
	//���񂾂񌸂炷
	m_wait -= time;
	//�O��荂����Ώ������Ȃ�
	if (m_wait > 0)
	{
		return;
	}

	DirectX::SimpleMath::Vector3 vel = DirectX::SimpleMath::Vector3(0.002f, 0, 0);

	//���S�Ɍ�����
	if (m_position.x > 0)
	{
		m_velocity.x -= vel.x;
	}
	else if (m_position.x < 0)
	{
		m_velocity.x += vel.x;
	}

	//�ړ�
	m_position += m_velocity;

	//���񂾂񏬂�������
	m_scale = Lerp(1.0f, 0.0f, 1.0f - m_life / m_startLife);
	
	//�p�x���Ȃ���
	m_angle += 0.1f;
	
	//���������炷
	m_life -= time;

	//�������O���Ⴍ�Ȃ�Ώ�����
	if (m_life < 0.0f)
	{
		Reset();
	}

}

//-----------------------------------------------------------------------------------
//	Render()
//	�`�揈��(���C���ȊO�̊O���v���j
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void FireShader::Render()
{
	////�r���{�[�h��
	m_world = DirectX::SimpleMath::Matrix::Identity;

	DirectX::SimpleMath::Matrix wrold = DirectX::SimpleMath::Matrix::Identity;
	wrold = DirectX::SimpleMath::Matrix::CreateBillboard(m_position + m_offsetPos, m_camera, DirectX::SimpleMath::Vector3::UnitY);

	DirectX::SimpleMath::Matrix revMat = DirectX::SimpleMath::Matrix::Identity;
	revMat._11 = -1.0f;
	revMat._33 = -1.0f;

	DirectX::SimpleMath::Matrix scaleMat, rotMat;
	scaleMat = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	rotMat = DirectX::SimpleMath::Matrix::CreateRotationZ(m_angle);
	//���[���h�s��v�Z
	m_world = scaleMat * rotMat * revMat * wrold;

	ShaderDraw();
}
//-----------------------------------------------------------------------------------
//	ShaderDraw()
//	���C���`�揈��
//	IN	:	void
//	RE	:	void
//-----------------------------------------------------------------------------------
void FireShader::ShaderDraw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	auto context = pDR->GetD3DDeviceContext();

	// ���_���(�|���S���̂S���_�̍��W���j
	DirectX::VertexPositionColorTexture vertex[] =
	{
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3::Zero,DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f)),
		DirectX::VertexPositionColorTexture(DirectX::SimpleMath::Vector3(0.0f,0.3f,0.0f),DirectX::SimpleMath::Vector4::One, DirectX::SimpleMath::Vector2(0.0f, 0.0f))
	};


	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = m_view.Transpose();
	cbuff.matProj = m_proj.Transpose();
	cbuff.matWorld = m_world.Transpose();
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1.0, 0.3, 0.1, 1);

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
	//ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	ID3D11BlendState* blendstate = m_states->Additive();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);

	// �J�����O�͍�����
	context->RSSetState(m_states->CullNone());

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
	//m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);

}

void FireShader::Draw()
{
}

void FireShader::Lost()
{
	m_CBuffer.Reset();
	m_inputLayout.Reset();
	m_batch.reset();
	m_states.reset();
	m_texture.Reset();
	m_vertexShader.Reset();
	m_pixelShader.Reset();
}




