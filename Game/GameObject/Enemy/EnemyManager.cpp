#include"pch.h"
#include"EnemyManager.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>

#include"Enemy.h"
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


}

void EnemyManager::Update(const DX::StepTimer& timer)
{
}

void EnemyManager::Render(const Camera* camera)
{
}

void EnemyManager::Finalize()
{
}

void EnemyManager::Reset()
{
}

void EnemyManager::LoadEnemyJsonFile(std::wstring jsonFilePath)
{
	//fstream�쐬
	std::fstream file(jsonFilePath, std::ifstream::in);
	//json�ǂݍ���
	nlohmann::json stageJson = nlohmann::json::parse(file);
	//�t�@�C�������
	file.close();

	DirectX::SimpleMath::Vector3 position = ConvertFloatArrayIntoVector3(stageJson["Enemy"]["Position"]);
	DirectX::SimpleMath::Vector3 scale = ConvertFloatArrayIntoVector3(stageJson["Enemy"]["Scale"]);
	DirectX::SimpleMath::Vector3 rotation = ConvertFloatArrayIntoVector3(stageJson["Enemy"]["Rotation"]);



}

DirectX::SimpleMath::Vector3 EnemyManager::ConvertFloatArrayIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}