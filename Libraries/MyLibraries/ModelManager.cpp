/*
* 2023/04/10
* ModelManager.cpp
* ���f����ǂݍ���ŊǗ�����
* �����@��
*/
#include"pch.h"
#include"ModelManager.h"
#include"DeviceResources.h"
#include<Effects.h>


/// <summary>
/// ���f���ǂݍ���
/// </summary>
/// <param name="fileName">���f���̃t�@�C���p�X</param>
/// <returns>���f���̐��|�C���^</returns>
DirectX::Model* ModelManager::LoadCmoModel(const std::wstring& fileName)
{
	//nullptr�ł���΃��f�������
	if (m_cmoModels[fileName] == nullptr)
	{
		//DeviceResources����f�o�C�X�̎擾
		ID3D11Device1* device = DX::DeviceResources::GetInstance()->GetD3DDevice();

		//�G�t�F�N�g�t�@�N�g���̍쐬
		std::unique_ptr<DirectX::EffectFactory> effectFactry = std::make_unique<DirectX::EffectFactory>(device);

		std::wstring filePath = L"Resources/Models/";

		size_t pos = fileName.find(filePath);
		
		std::wstring fullFilePath;
		
		if (pos == std::string::npos)
		{
			fullFilePath = filePath + fileName;
		}
		else
		{
			fullFilePath = fileName;
		}
		//	�e�N�X�`���̓ǂݍ��݃p�X�w��
		effectFactry->SetDirectory(filePath.c_str());

		//	�t�@�C�����w�肵�ă��f���f�[�^�ǂݍ��݁��ǂݍ��񂾃��f����Ԃ�
		m_cmoModels[fileName] = DirectX::Model::CreateFromCMO(
			device,
			fullFilePath.c_str(),
			*effectFactry
		);
	}

	//���f���𐶃|�C���^�ŕԂ�
	return m_cmoModels[fileName].get();
}
/// <summary>
/// SDKMESH���f���ǂݍ���
/// </summary>
/// <param name="fileName">���f���̃t�@�C���p�X</param>
/// <returns>���f���̐��|�C���^</returns>
DirectX::Model* ModelManager::LoadSdkmeshModel(const std::wstring& fileName)
{
	//nullptr�ł���΃��f�������
	if (m_sdkMeshModels[fileName] == nullptr)
	{
		//DeviceResources����f�o�C�X�̎擾
		ID3D11Device1* device = DX::DeviceResources::GetInstance()->GetD3DDevice();

		//�G�t�F�N�g�t�@�N�g���̍쐬
		std::unique_ptr<DirectX::EffectFactory> effectFactry = std::make_unique<DirectX::EffectFactory>(device);

		std::wstring filePath = L"Resources/Models/";

		size_t pos = fileName.find(filePath);

		std::wstring fullFilePath;

		if (pos == std::string::npos)
		{
			fullFilePath = filePath + fileName;
		}
		else
		{
			fullFilePath = fileName;
		}
		//	�e�N�X�`���̓ǂݍ��݃p�X�w��
		effectFactry->SetDirectory(filePath.c_str());

		//	�t�@�C�����w�肵�ă��f���f�[�^�ǂݍ��݁��ǂݍ��񂾃��f����Ԃ�
		m_sdkMeshModels[fileName] = DirectX::Model::CreateFromSDKMESH(
			device,
			fullFilePath.c_str(),
			*effectFactry,
			DirectX::DX11::ModelLoader_Clockwise | DirectX::DX11::ModelLoader_IncludeBones
		);
	}
	
	//���f���𐶃|�C���^�ŕԂ�
	return m_sdkMeshModels[fileName].get();
}

void ModelManager::Reset()
{
	for (auto& model : m_cmoModels)
	{
		model.second.reset();
	}

	for (auto& model : m_sdkMeshModels)
	{
		model.second.reset();
	}

}
