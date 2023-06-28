/*
* AABBFor3D.cpp
* 
* 
* 
* 
* 
*/
#include"pch.h"

#include"AABBFor3D.h"
#include"DeviceResources.h"


//�R���X�g���N�^
AABBFor3D::AABBFor3D():
	m_center{},
	m_r{}
{

}

//�f�X�g���N�^
AABBFor3D::~AABBFor3D()
{

}

//������
void AABBFor3D::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//�R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);
	
	//�x�[�V�b�N�G�t�F�N�g�̍쐬
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);

	//���̓��C�A�E�g���쐬���邽�߂̏���
	const void* shaderByteCode;
	size_t byteCodeLength;

	//���_�������肷�邽�߂ɕK�v�ȏ��
	m_basicEffect->SetLightingEnabled(false);	//���C�gOFF
	m_basicEffect->SetTextureEnabled(false);	//�e�N�X�`��OFF
	m_basicEffect->SetVertexColorEnabled(true);	//���_�J���[ON

	//�g�p���钸�_�V�F�[�_�[���̎擾
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	//���̓��C�A�E�g�̍쐬
	device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);
	
	//�v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBach = 
		std::make_unique< DirectX::PrimitiveBatch < DirectX::VertexPositionColor>>(context);

	m_center = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	m_r = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
}

/*
	AABB�����蔻��̃{�b�N�X���f�o�b�N�\������
   
   ����
	DirectX::SimpleMath::Matrix world
	�e�̃��[���h���w�肷��
	�ʏ�́uDirectX::SimpleMath::Matrix::Identity�v���w�肷��
	DirectX::SimpleMath::Matrix view
	�J�����ŎZ�o���ꂽview���w�肷��
	DirectX::SimpleMath::Matrix projection
	�ˉe�s���ݒ肵��projection��ݒ肷��
	DirectX::XMFLOAT4 color
	�`������鎞�̐F���w�肷��

	�߂�l�@void
*/
void AABBFor3D::Draw(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection, const DirectX::XMFLOAT4& color)
{

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//�G�t�F�N�g�Ɋe��ϊ��s��̐ݒ�[���ݒ肪�܂����ƌ����ڂ��������Ȃ�Ȃ�
	m_basicEffect->SetWorld(world);				//���[���h�s���ݒ�
	m_basicEffect->SetView(view);				//�r���[�s���ݒ�
	m_basicEffect->SetProjection(projection);	//�ˉe�s��̐ݒ�
	m_basicEffect->Apply(context);				

	//���̓A�Z���u���X�e�[�W�ɓ��̓��C�A�E�g��ݒ�
	context->IASetInputLayout(m_inputLayout.Get());

	//���C���Ń{�b�N�X��`�悷��
	m_primitiveBach->Begin();
	DrawBox(color);
	m_primitiveBach->End();
}

/*
	AABB�̓����蔻��p�̃{�b�N�X�̏����X�V����

	����
	DirectX::SimpleMath::Vector3 minPos
	��ԏ�����X���W�A��ԏ�����Y���W�A��ԏ�����Z���W
	DirectX::SimpleMath::Vector3 maxPos
	��ԑ傫��X���W�A��ԑ傫��XY���W�A��ԑ傫��Z���W

	�߂�l void
*/
void AABBFor3D::SetData(const DirectX::SimpleMath::Vector3& minPos, const DirectX::SimpleMath::Vector3& maxPos)
{
	//�{�b�N�X�̒��S�_���v�Z����
	m_center.x = (minPos.x + maxPos.x) / 2.0f;
	m_center.y = (minPos.y + maxPos.y) / 2.0f;
	m_center.z = (minPos.z + maxPos.z) / 2.0f;

	//�e�ӂƒ��S�_�Ƃ̋������v�Z����
	m_r.x = maxPos.x - m_center.x;
	m_r.y = maxPos.y - m_center.y;
	m_r.z = maxPos.z - m_center.z;
}

/*
	�����蔻����s��

	����
	AABBFor3D* other
	�����蔻��̑���ƂȂ�AABB�I�u�W�F�N�g

	�߂�l bool 
	�������Ă��� true
	�����Ă��Ȃ� false
*/
bool AABBFor3D::DetectCollition(AABBFor3D* other)
{
	//���S�_���m�̋����𑪂�
	DirectX::SimpleMath::Vector3 centerLength = other->m_center - this->m_center;
	DirectX::SimpleMath::Vector3 lineLength = DirectX::SimpleMath::Vector3(
														this->m_r.x + other->m_r.x,
														this->m_r.y + other->m_r.y,
														this->m_r.z + other->m_r.z);

	//���S�_���m�̋����Ƌ������v�̘a���ׂ�
	if (fabsf(centerLength.x) <= lineLength.x &&
		fabsf(centerLength.y) <= lineLength.y &&
		fabsf(centerLength.z) <= lineLength.z)
	{
		//���S�_���m�̋����̂ق�����������΁A���Ă��Ă���
		return true;
	}

	//���Ă��Ă��Ȃ�
	return false;
}

/*
	�����p�́A�{�b�N�X��`�悷��֐�
	����
	DirectX::XMFLOAT4
	�{�b�N�X��`�悷�邽�߂̐F

	�߂�l�@void
*/
void AABBFor3D::DrawBox(const DirectX::XMFLOAT4& color)
{
	DirectX::VertexPositionColor lineList[24];
	DirectX::SimpleMath::Vector3 minPos;
	DirectX::SimpleMath::Vector3 maxPos;
	DirectX::SimpleMath::Vector3 tmp;

	
	for (int i = 0; i < 24; i++)
	{
		lineList[i].color = color;
	}

	//�ŏ����W�ƍő���W���v�Z����
	minPos.x = m_center.x - m_r.x;
	minPos.y = m_center.y - m_r.y;
	minPos.z = m_center.z - m_r.z;

	maxPos.x = m_center.x + m_r.x;
	maxPos.y = m_center.y + m_r.y;
	maxPos.z = m_center.z + m_r.z;

	lineList[0].position = minPos;
	lineList[1].position = minPos + DirectX::SimpleMath::Vector3(m_r.x * 2.0f, 0.0f, 0.0f);

	lineList[2].position = minPos;
	lineList[3].position = minPos + DirectX::SimpleMath::Vector3( 0.0f, m_r.y * 2.0f, 0.0f);

	lineList[4].position = minPos;
	lineList[5].position = minPos + DirectX::SimpleMath::Vector3( 0.0f, 0.0f, m_r.z * 2.0f);

	
	lineList[6].position = maxPos;
	lineList[7].position = maxPos - DirectX::SimpleMath::Vector3(m_r.x * 2.0f, 0.0f, 0.0f);

	lineList[8].position = maxPos;
	lineList[9].position = maxPos - DirectX::SimpleMath::Vector3(0.0f, m_r.y * 2.0f, 0.0f);

	lineList[10].position = maxPos;
	lineList[11].position = maxPos - DirectX::SimpleMath::Vector3( 0.0f, 0.0f, m_r.z * 2.0f);

	
	tmp.x = minPos.x;
	tmp.y = maxPos.y;
	tmp.z = maxPos.z;
	lineList[12].position = tmp;
	lineList[13].position = tmp - DirectX::SimpleMath::Vector3( 0.0f, 0.0f, m_r.z * 2.0f);

	lineList[14].position = tmp;
	lineList[15].position = tmp - DirectX::SimpleMath::Vector3( 0.0f, m_r.y * 2.0f, 0.0f);

	
	tmp.x = maxPos.x;
	tmp.y = maxPos.y;
	tmp.z = minPos.z;
	lineList[16].position = tmp;
	lineList[17].position = tmp - DirectX::SimpleMath::Vector3(m_r.x * 2.0f, 0.0f, 0.0f);

	lineList[18].position = tmp;
	lineList[19].position = tmp - DirectX::SimpleMath::Vector3(0.0f, m_r.y * 2.0f, 0.0f);

	//Y���ŏ��l�AZX���ő�l�̓_����2�{
	tmp.x = maxPos.x;
	tmp.y = minPos.y;
	tmp.z = maxPos.z;
	lineList[20].position = tmp;
	lineList[21].position = tmp - DirectX::SimpleMath::Vector3(m_r.x * 2.0f, 0.0f, 0.0f);

	lineList[22].position = tmp;
	lineList[23].position = tmp - DirectX::SimpleMath::Vector3(0.0f, 0.0f, m_r.z * 2.0f);

	//���C�����X�g��n���ĕ`�悵�Ă��炤
	m_primitiveBach->Draw(D3D_PRIMITIVE_TOPOLOGY_LINELIST, lineList, 24);

}
