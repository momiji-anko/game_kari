/*
* 2023/04/10
* TextureManager.h
* �e�N�X�`����ǂݍ��݁��Ǘ�����N���X
* �����@��
*/
#pragma once 
#include"Singleton.h"
#include<WICTextureLoader.h>
#include<unordered_map>
#include<SimpleMath.h>

/// <summary>
/// �e�N�X�`���}�l�[�W���[
/// </summary>
class TextureManager :public Singleton<TextureManager>
{

public:

	/// <summary>
	/// �e�N�X�`����ǂݍ���
	/// </summary>
	/// <param name="fileName">�e�N�X�`���̃t�@�C���p�X</param>
	/// <returns>�e�N�X�`��</returns>
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTexture(const std::wstring& fileName);

	/// <summary>
	/// �e�N�X�`���T�C�Y�̎擾
	/// </summary>
	/// <param name="fileName">�e�N�X�`���̃t�@�C���p�X</param>
	/// <returns>�e�N�X�`���T�C�Y</returns>
	DirectX::SimpleMath::Vector2 GetTextureSize(const std::wstring& fileName);

	/// <summary>
	/// �e�N�X�`���T�C�Y�̐ݒ�
	/// </summary>
	/// <param name="resouce">���\�[�X</param>
	/// <param name="fileName">�e�N�X�`���̃t�@�C���p�X</param>
	void TextureSize(const Microsoft::WRL::ComPtr<ID3D11Resource>& resouce, const std::wstring& fileName);

private:
	//�e�N�X�`��
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

	//�e�N�X�`���T�C�Y
	std::unordered_map<std::wstring, DirectX::SimpleMath::Vector2> m_textureSize;
};