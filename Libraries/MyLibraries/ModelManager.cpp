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
DirectX::Model* ModelManager::LoadModel(const std::wstring& fileName)
{
	//nullptr�ł���΃��f�������
	if (m_models[fileName] == nullptr)
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
		m_models[fileName] = DirectX::Model::CreateFromCMO(
			device,
			fullFilePath.c_str(),
			*effectFactry
		);
	}

	//���f���𐶃|�C���^�ŕԂ�
	return m_models[fileName].get();
}

void ModelManager::Reset()
{
	for (auto& model : m_models)
	{
		model.second.reset();
	}

}
