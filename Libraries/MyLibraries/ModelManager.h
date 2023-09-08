/*
* 2023/04/10
* ModelManager.h 
* ���f���}�l�[�W���[
* �����@��
*/
#pragma once 
#include"Singleton.h"
#include <unordered_map>
#include<Model.h>
#include"Libraries/Animation/Animation.h"

/// <summary>
/// ���f���}�l�[�W���[
/// </summary>
class ModelManager : public Singleton<ModelManager>
{
public:
	/// <summary>
	/// CMO���f���ǂݍ���
	/// </summary>
	/// <param name="fileName">���f���̃t�@�C���p�X</param>
	/// <returns>���f���̐��|�C���^</returns>
	DirectX::Model* LoadCmoModel(const std::wstring& fileName);
	
	/// <summary>
	/// SDKMESH���f���ǂݍ���
	/// </summary>
	/// <param name="fileName">���f���̃t�@�C���p�X</param>
	/// <returns>���f���̐��|�C���^</returns>
	DirectX::Model* LoadSdkmeshModel(const std::wstring& fileName);

	void Reset();
private:
	//���f���̔z��
	std::unordered_map<std::wstring, std::unique_ptr<DirectX::Model>> m_cmoModels;

	std::unordered_map<std::wstring, std::unique_ptr<DirectX::Model>> m_sdkMeshModels;
	

};