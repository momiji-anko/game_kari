/*
* 2023/04/10
* TextureManager.cpp
* �e�N�X�`����ǂݍ���&�Ǘ�����
* �����@��
*/
#include"pch.h"
#include"TextureManager.h"
#include"DeviceResources.h"
#include"MemoryLeakDetector.h"

/// <summary>
/// �e�N�X�`����ǂݍ���
/// </summary>
/// <param name="fileName">�e�N�X�`���̃t�@�C���p�X</param>
/// <returns>�e�N�X�`��</returns>
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> TextureManager::LoadTexture(const std::wstring& fileName)
{
	//�e�N�X�`�������ɓǂݍ��܂�Ă��Ȃ��ꍇ�ǂݍ���
	if (m_textures[fileName] == nullptr)
	{
		//DeviceResources����f�o�C�X�̎擾
		ID3D11Device1* device = DX::DeviceResources::GetInstance()->GetD3DDevice();

		Microsoft::WRL::ComPtr<ID3D11Resource> resouce;

		//�e�N�X�`���̓ǂݍ���
		DirectX::CreateWICTextureFromFile(
			device,
			fileName.c_str(),
			resouce.ReleaseAndGetAddressOf(),
			m_textures[fileName].ReleaseAndGetAddressOf()
		);

		//�e�N�X�`���T�C�Y��ݒ�
		TextureSize(resouce, fileName);
		
	}

	return m_textures[fileName];
}

/// <summary>
/// �e�N�X�`���T�C�Y�̎擾
/// </summary>
/// <param name="fileName">�e�N�X�`���̃t�@�C���p�X</param>
/// <returns>�e�N�X�`���T�C�Y</returns>
DirectX::SimpleMath::Vector2 TextureManager::GetTextureSize(const std::wstring& fileName)
{
	//�e�N�X�`�����ǂݍ��܂ꂽ�ꍇ�e�N�X�`���T�C�Y��Ԃ�
	if (m_textures[fileName] != nullptr)
	{
		return m_textureSize[fileName];
	}

	//�e�N�X�`�����ǂݍ��܂�Ă��Ȃ��ꍇ�[����Ԃ�
	return DirectX::SimpleMath::Vector2::Zero;
}

/// <summary>
/// �e�N�X�`���T�C�Y�̐ݒ�
/// </summary>
/// <param name="resouce">���\�[�X</param>
/// <param name="fileName">�e�N�X�`���̃t�@�C���p�X</param>
void TextureManager::TextureSize(const Microsoft::WRL::ComPtr<ID3D11Resource>& resouce, const std::wstring& fileName)
{
	//�e�N�X�`���QD
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
	//���\�[�X����e�N�X�`���QD�ɃL���X�g
	resouce.As(&texture2d);
	
	D3D11_TEXTURE2D_DESC texDesc;
	//�e�N�X�`���QD����Desc���擾
	texture2d->GetDesc(&texDesc);

	//�e�N�X�`���T�C�Y��ݒ�
	m_textureSize[fileName] = DirectX::SimpleMath::Vector2(static_cast<float>(texDesc.Width), static_cast<float>(texDesc.Height));
}
