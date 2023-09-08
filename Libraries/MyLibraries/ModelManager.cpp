/*
* 2023/04/10
* ModelManager.cpp
* モデルを読み込んで管理する
* 麻生　楓
*/
#include"pch.h"
#include"ModelManager.h"
#include"DeviceResources.h"
#include<Effects.h>


/// <summary>
/// モデル読み込み
/// </summary>
/// <param name="fileName">モデルのファイルパス</param>
/// <returns>モデルの生ポインタ</returns>
DirectX::Model* ModelManager::LoadCmoModel(const std::wstring& fileName)
{
	//nullptrであればモデルを作る
	if (m_cmoModels[fileName] == nullptr)
	{
		//DeviceResourcesからデバイスの取得
		ID3D11Device1* device = DX::DeviceResources::GetInstance()->GetD3DDevice();

		//エフェクトファクトリの作成
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
		//	テクスチャの読み込みパス指定
		effectFactry->SetDirectory(filePath.c_str());

		//	ファイルを指定してモデルデータ読み込み＆読み込んだモデルを返す
		m_cmoModels[fileName] = DirectX::Model::CreateFromCMO(
			device,
			fullFilePath.c_str(),
			*effectFactry
		);
	}

	//モデルを生ポインタで返す
	return m_cmoModels[fileName].get();
}
/// <summary>
/// SDKMESHモデル読み込み
/// </summary>
/// <param name="fileName">モデルのファイルパス</param>
/// <returns>モデルの生ポインタ</returns>
DirectX::Model* ModelManager::LoadSdkmeshModel(const std::wstring& fileName)
{
	//nullptrであればモデルを作る
	if (m_sdkMeshModels[fileName] == nullptr)
	{
		//DeviceResourcesからデバイスの取得
		ID3D11Device1* device = DX::DeviceResources::GetInstance()->GetD3DDevice();

		//エフェクトファクトリの作成
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
		//	テクスチャの読み込みパス指定
		effectFactry->SetDirectory(filePath.c_str());

		//	ファイルを指定してモデルデータ読み込み＆読み込んだモデルを返す
		m_sdkMeshModels[fileName] = DirectX::Model::CreateFromSDKMESH(
			device,
			fullFilePath.c_str(),
			*effectFactry,
			DirectX::DX11::ModelLoader_Clockwise | DirectX::DX11::ModelLoader_IncludeBones
		);
	}
	
	//モデルを生ポインタで返す
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
