#include"pch.h"
#include"KeyManager.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>

#include"Key.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/Json/json.hpp"

/// <summary>
/// コンストラクタ
/// </summary>
KeyManager::KeyManager(int stageNum)
	:
	Actor{
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		nullptr,
		true
	},
	m_stageNum{ stageNum }
{
}

/// <summary>
/// デストラクタ
/// </summary>
KeyManager::~KeyManager()
{
}

/// <summary>
/// 初期化	
/// </summary>
void KeyManager::Initialize()
{
	//goalJsonのファイルパスを取得
	std::vector<std::wstring> goalJsonFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Json/");

	//Jsonを読み込み鍵の作成
	LoadKeyJsonFile(goalJsonFiles[m_stageNum]);

	GameContext::GetInstance().SetIsAllGetKey(false);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void KeyManager::Update(const DX::StepTimer& timer)
{
	for (std::unique_ptr<Actor>& key : m_keys)
	{
		key->Update(timer);
	}
	//すべての鍵を取得したか確認
	CheckALLGetKey();
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void KeyManager::Render(const Camera* camera)
{
	for (std::unique_ptr<Actor>& key : m_keys)
	{
		key->Render(camera);
	}
}

/// <summary>
/// 終了処理
/// </summary>
void KeyManager::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void KeyManager::Reset()
{
}

void KeyManager::LoadKeyJsonFile(const std::wstring& filePath)
{
	//fstream作成
	std::fstream file(filePath, std::ifstream::in);
	//json読み込み
	nlohmann::json keyJson = nlohmann::json::parse(file);
	//ファイルを閉じる
	file.close();

	for (int i = 0; i < keyJson["Key"].size(); i++)
	{
		//鍵の作成
		std::unique_ptr<Actor> key = std::make_unique<Key>(
			ConvertFloatArrayIntoVector3(keyJson["Key"][i]["Position"]),
			DirectX::SimpleMath::Vector3::Zero,
			ConvertFloatArrayIntoVector3(keyJson["Key"][i]["Scale"]),
			ConvertFloatArrayIntoVector3(keyJson["Key"][i]["Rotation"]),
			ModelManager::GetInstance().LoadCmoModel(L"key.cmo"),
			true
			);
		key->Initialize();
		//鍵の追加
		AddKey(std::move(key));
	}


}


/// <summary>
/// jsonで読み込んだ座標をVector3に変換
/// </summary>
/// <param name="nums">jsonで読み込んだ座標</param>
/// <returns>変換した座標</returns>
DirectX::SimpleMath::Vector3 KeyManager::ConvertFloatArrayIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}

/// <summary>
/// プレイヤーが鍵をすべてゲットしたか確認
/// </summary>
void KeyManager::CheckALLGetKey()
{
	//ゲットした鍵の数
	int getKeyNum = 0;
	for (std::unique_ptr<Actor>& key : m_keys)
	{
		//鍵がアクティブ状態でなければゲットしている
		getKeyNum += !key->IsActive();
	}

	//すべての鍵をゲットしたか
	bool isAllGet = (getKeyNum == m_keys.size());
	GameContext::GetInstance().SetIsAllGetKey(isAllGet);
}


