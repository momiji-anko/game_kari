#include"pch.h"
#include"EnemyManager.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>

#include"Enemy.h"
#include"PlayerTrackingEnemy.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/Json/json.hpp"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="stageNum">ステージ番号</param>
EnemyManager::EnemyManager(int stageNum)
	:
	Actor(
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		nullptr,
		true
	),
	m_enemies{},
	m_stageNum{stageNum},
	m_trackStartTime{2.0f}
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyManager::~EnemyManager()
{

}

/// <summary>
/// 初期化	
/// </summary>
void EnemyManager::Initialize()
{
	//敵のjsonファイル調べる
	std::vector<std::wstring> enemyJsonFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Json/");

	//json読み込み
	LoadEnemyJsonFile(enemyJsonFiles[m_stageNum]);

	//敵マネージャーの設定
	GameContext::GetInstance().SetEnemyManager(this);

	//何秒前のプレイヤーを追跡する時間を初期化	
	m_trackStartTime = 2.0f;
}


/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void EnemyManager::Update(const DX::StepTimer& timer)
{
	//敵の更新
	for (std::unique_ptr<Actor>& enemy : m_enemies)
	{
		if (enemy == nullptr)
			continue;

		enemy->Update(timer);
	}
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void EnemyManager::Render(const Camera* camera)
{
	//敵の描画
	for (std::unique_ptr<Actor>& enemy : m_enemies)
	{
		if (enemy == nullptr)
			continue;

		enemy->Render(camera);
	}
}

/// <summary>
/// 終了処理
/// </summary>
void EnemyManager::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void EnemyManager::Reset()
{
}

/// <summary>
/// json読み込み
/// </summary>
/// <param name="jsonFilePath">jsonファイルパス</param>
void EnemyManager::LoadEnemyJsonFile(std::wstring jsonFilePath)
{
	//fstream作成
	std::fstream file(jsonFilePath, std::ifstream::in);
	//json読み込み
	nlohmann::json enemyJson = nlohmann::json::parse(file);
	//ファイルを閉じる
	file.close();

	//敵の配列のサイズ
	int enemyNum = enemyJson["Enemy"].size();

	//敵の生成
	for (int i = 0; i < enemyNum; i++)
	{
		//座標
		DirectX::SimpleMath::Vector3 position = ConvertFloatArrayIntoVector3(enemyJson["Enemy"][i]["Position"]);
		//拡大率
		DirectX::SimpleMath::Vector3 scale = ConvertFloatArrayIntoVector3(enemyJson["Enemy"][i]["Scale"]);
		//角度
		DirectX::SimpleMath::Vector3 rotation = ConvertFloatArrayIntoVector3(enemyJson["Enemy"][i]["Rotation"]);

		//敵の生成
		std::unique_ptr<Actor> enemy = std::make_unique<Enemy>(position, DirectX::SimpleMath::Vector3::Zero, scale, rotation, ModelManager::GetInstance().LoadSdkmeshModel(L"enemy.sdkmesh"), true);
		//初期化
		enemy->Initialize();
		//敵の追加
		AddEnemy(std::move(enemy));
	}
}

/// <summary>
/// jsonで読み込んだ座標をVector3に変換
/// </summary>
/// <param name="nums">jsonで読み込んだ座標</param>
/// <returns>変換した座標</returns>
DirectX::SimpleMath::Vector3 EnemyManager::ConvertFloatArrayIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}

/// <summary>
/// プレイヤーを追いかける敵の生成
/// </summary>
void EnemyManager::CreatePlayerTrackingEnemy()
{
	//座標
	DirectX::SimpleMath::Vector3 position = GameContext::GetInstance().GetPlayerPosition();
	//移動
	DirectX::SimpleMath::Vector3 velocity = DirectX::SimpleMath::Vector3::Zero;
	//角度
	DirectX::SimpleMath::Vector3 rotation = DirectX::SimpleMath::Vector3::Zero;
	//拡大率
	DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.02);
	
	//生成
	std::unique_ptr<Actor> enemy = std::make_unique<PlayerTrackingEnemy>(position, DirectX::SimpleMath::Vector3::Zero, scale, rotation, ModelManager::GetInstance().LoadSdkmeshModel(L"enemy.sdkmesh"), m_trackStartTime);
	//初期化
	enemy->Initialize();
	
	//追加
	AddEnemy(std::move(enemy));

	//時間を半分にする
	m_trackStartTime /= 2.0f;

}
