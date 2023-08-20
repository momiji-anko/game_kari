#include"pch.h"
#include"StageManager.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>

#include"Stage.h"
#include"Game/GameObject/Goal/Goal.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/Json/json.hpp"

StageManager::StageManager(int stageNum)
	:
	Actor(
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::Zero,
		nullptr,
		true
	),
	m_stageNum(stageNum)
{

	std::vector<std::wstring> objFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Obj/");
	std::vector<std::wstring> goalJsonFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Json/");

	LoadObjFile(objFiles[m_stageNum]);

	LoadGoalJsonFile(goalJsonFiles[m_stageNum]);
}

StageManager::~StageManager()
{
}

void StageManager::Initialize()
{

	std::unique_ptr<Actor> stage = std::make_unique<Stage>(
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::One, 
		DirectX::SimpleMath::Vector3::Zero,
		ModelManager::GetInstance().LoadModel(L"stage1.cmo"),
		true
		);

	AddStage(stage);

}

void StageManager::Update(const DX::StepTimer& timer)
{
	for (std::unique_ptr<Actor>& stage : m_stage)
	{
		stage->Update(timer);
	}
}

void StageManager::Render(const Camera* camera)
{
	for (std::unique_ptr<Actor>& stage : m_stage)
	{
		stage->Render(camera);
	}
}

void StageManager::Finalize()
{
}
void StageManager::Reset()
{
}

void StageManager::LoadObjFile(const std::wstring& filePath)
{
	// obj形式のファイル読み込み
	std::vector<DirectX::SimpleMath::Vector3> vertexes;
	std::vector<int> indexes;
	std::ifstream ifs(filePath);

	std::string str;
	while (getline(ifs, str))
	{
		// 頂点であれば座標を抜き取る
		if (str[0] == 'v' && str[1] == ' ')
		{
			DirectX::SimpleMath::Vector3 val;
			sscanf_s(str.data(), "v  %f %f %f", &val.x, &val.y, &val.z);
			vertexes.push_back(val);
		}

		// インデックスであればインデックス情報を抜き取る
		if (str[0] == 'f')
		{
			int a, b, c;
			int tmp1, tmp2, tmp3, tmp4;
			a = b = c = INT32_MAX;

			sscanf_s(str.data(), "f %d/%d %d/%d %d", &a, &tmp1, &b, &tmp2, &c);

			if (a == INT32_MAX || b == INT32_MAX || c == INT32_MAX)
			{
				sscanf_s(str.data(), "f %d/%d/%d %d/%d/%d %d", &a, &tmp1, &tmp3, &b, &tmp2, &tmp4, &c);
			}

			// 三角形の頂点インデックス番号が同じ物を含む場合は無視する
			if (a != b && a != c && b != c)
			{
				indexes.push_back(a - 1);
				indexes.push_back(c - 1);
				indexes.push_back(b - 1);
			}
		}
	}
	ifs.close();
	std::vector<std::vector<int>> inde;

	for (int i = 0; i < indexes.size() / 3; i++)
	{
		m_indexes.push_back({ indexes[i * 3], indexes[i * 3 + 1],indexes[i * 3 + 2] });
	}
	m_vertexesPosition = vertexes;

	GameContext::GetInstance().GetCollisionManager()->SetIndexes(m_indexes);
	GameContext::GetInstance().GetCollisionManager()->SetVertexPosition(m_vertexesPosition);
}

void StageManager::LoadGoalJsonFile(const std::wstring& filePath)
{
	//fstream作成
	std::fstream file(filePath, std::ifstream::in);
	//json読み込み
	nlohmann::json stageJson = nlohmann::json::parse(file);
	//ファイルを閉じる
	file.close();

	DirectX::SimpleMath::Vector3 position = ConvertFloatArrayIntoVector3(stageJson["Goal"]["Position"]);
	DirectX::SimpleMath::Vector3 scale = ConvertFloatArrayIntoVector3(stageJson["Goal"]["Scale"]);
	DirectX::SimpleMath::Vector3 rotation = ConvertFloatArrayIntoVector3(stageJson["Goal"]["Rotation"]);

	std::unique_ptr<Actor> goal = std::make_unique<Goal>(
		position,
		DirectX::SimpleMath::Vector3::Zero,
		scale,
		rotation,
		ModelManager::GetInstance().LoadModel(L"dice.cmo"),
		true
		);


	goal->Initialize();
	
	AddStage(std::move(goal));

}

DirectX::SimpleMath::Vector3 StageManager::ConvertFloatArrayIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}



