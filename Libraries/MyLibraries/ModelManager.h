/*
* 2023/04/10
* ModelManager.h 
* モデルマネージャー
* 麻生　楓
*/
#pragma once 
#include"Singleton.h"
#include <unordered_map>
#include<Model.h>
#include"Libraries/Animation/Animation.h"

/// <summary>
/// モデルマネージャー
/// </summary>
class ModelManager : public Singleton<ModelManager>
{
public:
	/// <summary>
	/// CMOモデル読み込み
	/// </summary>
	/// <param name="fileName">モデルのファイルパス</param>
	/// <returns>モデルの生ポインタ</returns>
	DirectX::Model* LoadCmoModel(const std::wstring& fileName);
	
	/// <summary>
	/// SDKMESHモデル読み込み
	/// </summary>
	/// <param name="fileName">モデルのファイルパス</param>
	/// <returns>モデルの生ポインタ</returns>
	DirectX::Model* LoadSdkmeshModel(const std::wstring& fileName);

	void Reset();
private:
	//モデルの配列
	std::unordered_map<std::wstring, std::unique_ptr<DirectX::Model>> m_cmoModels;

	std::unordered_map<std::wstring, std::unique_ptr<DirectX::Model>> m_sdkMeshModels;
	

};