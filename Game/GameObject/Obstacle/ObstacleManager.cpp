#include"pch.h"

#include"ObstacleManager.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>

#include"Obstacle.h"
#include"ObstacleSpawner.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/Json/json.hpp"

ObstacleManager::ObstacleManager(int stageNum)
	:
	Actor{
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		nullptr,
		true
	},
	m_stageNum{stageNum}
{

}

ObstacleManager::~ObstacleManager()
{

}

void ObstacleManager::Initialize()
{
	std::vector<std::wstring> jsonFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Json/");

	LoadJsonFile(jsonFiles[m_stageNum]);

}

void ObstacleManager::Update(const DX::StepTimer& timer)
{
	
	ObstacleUpdate(timer);

	ObstacleSpawnerUpdate(timer);
	
	DeleteObstacle();
}

void ObstacleManager::Render(const Camera* camera)
{
	
	ObstacleRender(camera);

	ObstacleSpawnerRender(camera);
	
}

void ObstacleManager::Finalize()
{

}

void ObstacleManager::Reset()
{

}

void ObstacleManager::LoadJsonFile(const std::wstring& filePath)
{
	//fstreamçÏê¨
	std::fstream file(filePath, std::ifstream::in);
	//jsonì«Ç›çûÇ›
	nlohmann::json obstacleJson = nlohmann::json::parse(file);
	//ÉtÉ@ÉCÉãÇï¬Ç∂ÇÈ
	file.close();

	float speed = obstacleJson["Obstacle"]["Speed"];
	DirectX::SimpleMath::Vector3 scale = ConvertFloatArrayIntoVector3(obstacleJson["Obstacle"]["Scale"]);
	DirectX::SimpleMath::Vector3 rotation = ConvertFloatArrayIntoVector3(obstacleJson["Obstacle"]["Rotation"]);

	float obstacleSpawnArea = obstacleJson["ObstacleSpawnArea"];

	std::vector<DirectX::SimpleMath::Vector3> obstacleSpawnPosition;
	
	for (int i = 0; i < obstacleJson["ObstacleSpawnPosition"].size(); i++)
	{
		std::unique_ptr<Actor> obstacleSpawner;

		obstacleSpawner = std::make_unique<ObstacleSpawner>(
			ConvertFloatArrayIntoVector3(obstacleJson["ObstacleSpawnPosition"][i]), 
			this,
			obstacleSpawnArea, 
			speed, 
			scale, 
			rotation,
			ModelManager::GetInstance().LoadCmoModel(L"dice.cmo")
			);
		obstacleSpawner->Initialize();

		obstacleSpawnPosition.push_back(ConvertFloatArrayIntoVector3(obstacleJson["ObstacleSpawnPosition"][i]));

		AddObstacleSpawner(std::move(obstacleSpawner));
	}

}

DirectX::SimpleMath::Vector3 ObstacleManager::ConvertFloatArrayIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}

void ObstacleManager::AddObstacle(std::unique_ptr<Actor> addObstacle)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacle)
	{
		if (obstacle == nullptr)
		{
			obstacle = std::move(addObstacle);

			return;
		}
	}

	m_obstacle.emplace_back(std::move(addObstacle));

}

void ObstacleManager::DeleteObstacle()
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacle)
	{
		if (obstacle == nullptr)
			continue; 

		if (!obstacle->IsActive())
		{
			obstacle.reset();
			obstacle = nullptr;
		}
	}
}

void ObstacleManager::ObstacleUpdate(const DX::StepTimer& timer)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacle)
	{
		if (obstacle != nullptr)
			obstacle->Update(timer);
	}
}

void ObstacleManager::ObstacleSpawnerUpdate(const DX::StepTimer& timer)
{
	for (std::unique_ptr<Actor>& obstacleSpawner : m_obstacleSpawner)
	{
		obstacleSpawner->Update(timer);
	}
}

void ObstacleManager::ObstacleRender(const Camera* camera)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacle)
	{
		if (obstacle != nullptr)
			obstacle->Render(camera);
	}
}

void ObstacleManager::ObstacleSpawnerRender(const Camera* camera)
{
	for (std::unique_ptr<Actor>& obstacle : m_obstacleSpawner)
	{
		obstacle->Render(camera);
	}
}
