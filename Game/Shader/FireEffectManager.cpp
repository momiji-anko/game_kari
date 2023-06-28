/*
* *2023/03/10
* FireEffectManager.h
* �t�@�C���[�V�F�[�_�[
* �����@��
*/
#include "pch.h"
#include "DeviceResources.h"
#include "FireEffectManager.h"

#include <SimpleMath.h> 
#include <Effects.h> 
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 
#include <CommonStates.h>
#include<time.h>
#include<random>



void FireEffectManager::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();

	//�o�b�`�G�t�F�N�g�̍쐬
	m_batchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);

	// ���̓��C�A�E�g����
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());
	
	//�v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	
	//�e�N�X�`���̃��[�h
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/shadow.png",
		nullptr,
		m_texture.GetAddressOf());
	
	int count = 20;
	
	for (int i = 0; i < count; i++) 
	{
		std::unique_ptr<FireShader> effect = std::make_unique<FireShader>();
		effect->Create();
		m_effectList.push_back(std::move(effect));
	}

}

void FireEffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos)
{
	UNREFERENCED_PARAMETER(pos);

	srand((unsigned int)time(NULL));
	//life,pos,vel �̒l��m_effect������������
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		int range = 200;
		DirectX::SimpleMath::Vector3 vel = DirectX::SimpleMath::Vector3(((rand() % (range * 2)) - range) / (float)range * 0.04f, ((rand() % (range * 2))) / (float)range * 0.01f, 0);
		while (vel.Length() < 0.001f)
		{
			vel = DirectX::SimpleMath::Vector3(((rand() % (range * 2)) - range) / (float)range * 0.04f, ((rand() % (range * 2))) / (float)range * 0.01f, 0);
		}
		//float a = (std::rand()%10) - 5.0f / 10.0f;

		(*ite)->Initialize(life, DirectX::SimpleMath::Vector3::Zero, vel);
	}

}


void FireEffectManager::Update(const DX::StepTimer& timer)
{
	//timer��n����m_effect�̍X�V�������s��
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Update(timer);
	}
}

void FireEffectManager::Render()
{
	
	//m_effect��`�悷��
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->Render();
	}
}

void FireEffectManager::SetRenderState(const DirectX::SimpleMath::Vector3& camera, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	//camera,view,proj,�̒l��m_effect�ɓn��
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
		(*ite)->SetRenderState(camera,view,proj);
	}
}



void FireEffectManager::SetStartPosition(const DirectX::SimpleMath::Vector3& pos)
{
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
//		float a = (std::rand() % 10) - 5.0f ;
		(*ite)->SetStartPos(DirectX::SimpleMath::Vector3(pos.x , pos.y, pos.z));
	}

}

void FireEffectManager::SetOffsetPosition(const DirectX::SimpleMath::Vector3& pos)
{
	for (std::list<std::unique_ptr<FireShader>>::iterator ite = m_effectList.begin();
		ite != m_effectList.end(); ite++)
	{
//		float a = (std::rand() % 10) - 5.0f ;
		(*ite)->SetOffsetPosition(DirectX::SimpleMath::Vector3(pos.x , pos.y, pos.z));
	}

}
