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
	m_stageNum{stageNum}
{
	
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Initialize()
{
	std::vector<std::wstring> enemyJsonFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Json/");


	LoadEnemyJsonFile(enemyJsonFiles[m_stageNum]);

	GameContext::GetInstance().SetEnemyManager(this);

}

void EnemyManager::Update(const DX::StepTimer& timer)
{
	for (std::unique_ptr<Actor>& enemy : m_enemies)
	{
		if (enemy == nullptr)
			continue;

		enemy->Update(timer);
	}
}

void EnemyManager::Render(const Camera* camera)
{
	for (std::unique_ptr<Actor>& enemy : m_enemies)
	{
		if (enemy == nullptr)
			continue;

		enemy->Render(camera);
	}
}

void EnemyManager::Finalize()
{
}

void EnemyManager::Reset()
{
}

void EnemyManager::LoadEnemyJsonFile(std::wstring jsonFilePath)
{
	//fstream作成
	std::fstream file(jsonFilePath, std::ifstream::in);
	//json読み込み
	nlohmann::json enemyJson = nlohmann::json::parse(file);
	//ファイルを閉じる
	file.close();

	int enemyNum = enemyJson["Enemy"].size();

	for (int i = 0; i < enemyNum; i++)
	{
		DirectX::SimpleMath::Vector3 position = ConvertFloatArrayIntoVector3(enemyJson["Enemy"][i]["Position"]);
		DirectX::SimpleMath::Vector3 scale = ConvertFloatArrayIntoVector3(enemyJson["Enemy"][i]["Scale"]);
		DirectX::SimpleMath::Vector3 rotation = ConvertFloatArrayIntoVector3(enemyJson["Enemy"][i]["Rotation"]);

		std::vector<DirectX::SimpleMath::Vector3>tsts = { DirectX::SimpleMath::Vector3{ 10,0,0 }, DirectX::SimpleMath::Vector3{ -10,0,0 } };

		std::unique_ptr<Actor> enemy = std::make_unique<Enemy>(position, DirectX::SimpleMath::Vector3::Zero, scale, rotation, ModelManager::GetInstance().LoadSdkmeshModel(L"enemy.sdkmesh"), true, tsts);

		enemy->Initialize();

		AddEnemy(std::move(enemy));

	}

}

DirectX::SimpleMath::Vector3 EnemyManager::ConvertFloatArrayIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}

void EnemyManager::CreatePlayerTrackingEnemy(float time)
{
	DirectX::SimpleMath::Vector3 position = GameContext::GetInstance().GetPlayerPosition();
	DirectX::SimpleMath::Vector3 velocity = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 rotation = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.02);

	std::unique_ptr<Actor> enemy = std::make_unique<PlayerTrackingEnemy>(position, DirectX::SimpleMath::Vector3::Zero, scale, rotation, ModelManager::GetInstance().LoadSdkmeshModel(L"enemy.sdkmesh"), time);

	enemy->Initialize();

	AddEnemy(std::move(enemy));

}
