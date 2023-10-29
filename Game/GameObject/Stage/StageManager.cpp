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

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="stageNum">ステージ番号</param>
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
	//OBJファイル読み込み
	std::vector<std::wstring> objFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Obj/");
	//Jsonファイル読み込み
	std::vector<std::wstring> goalJsonFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Json/");
	
	//OBJファイル読み込みステージの当たり判定作成
	LoadObjFile(objFiles[m_stageNum]);
	//ゴール読み込み
	LoadGoalJsonFile(goalJsonFiles[m_stageNum]);
}

/// <summary>
/// デストラクタ
/// </summary>
StageManager::~StageManager()
{
}

/// <summary>
/// 初期化	
/// </summary>
void StageManager::Initialize()
{
	//ステージの生成
	std::unique_ptr<Actor> stage = std::make_unique<Stage>(
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::One, 
		DirectX::SimpleMath::Vector3::Zero,
		ModelManager::GetInstance().LoadCmoModel(L"stage1.cmo"),
		true
		);
	//ステージ追加
	AddStage(stage);

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
void StageManager::Update(const DX::StepTimer& timer)
{
	//ステージ更新
	for (std::unique_ptr<Actor>& stage : m_stage)
	{
		stage->Update(timer);
	}
}

/// <summary>
/// 描画
/// </summary>
/// <param name="camera">カメラの生ポインタ</param>
void StageManager::Render(const Camera* camera)
{
	//ステージ描画
	for (std::unique_ptr<Actor>& stage : m_stage)
	{
		stage->Render(camera);
	}
}

/// <summary>
/// 終了処理
/// </summary>
void StageManager::Finalize()
{
}

/// <summary>
/// リセット
/// </summary>
void StageManager::Reset()
{
}

/// <summary>
/// OBJファイル読み込み
/// </summary>
/// <param name="filePath">OBJファイルパス</param>
void StageManager::LoadObjFile(const std::wstring& filePath)
{
	// obj形式のファイル読み込み
	//頂点座標
	std::vector<DirectX::SimpleMath::Vector3> vertexes;
	//インデックス
	std::vector<int> indexes;

	//OBJファイル
	std::ifstream ifs(filePath);

	//読み込んだ文字列
	std::string str;
	//一列読み込む
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
	//終わったらファイルを閉じる
	ifs.close();

	
	//インデックス代入
	for (int i = 0; i < indexes.size() / 3; i++)
	{
		m_indexes.push_back({ indexes[i * 3], indexes[i * 3 + 1],indexes[i * 3 + 2] });
	}
	//頂点座標代入
	m_vertexesPosition = vertexes;

	//コリジョンマネージャー取得
	CollisionManager* collisionManager = GameContext::GetInstance().GetCollisionManager();

	//コリジョンマネージャーがNULLでなければコリジョンマネージャーに当たり判定を登録
	if (collisionManager != nullptr)
	{
		GameContext::GetInstance().GetCollisionManager()->SetVertexPosition(m_vertexesPosition);
		GameContext::GetInstance().GetCollisionManager()->SetIndexes(m_indexes);
	}
	
}

/// <summary>
/// ゴールJsonを読み込み
/// </summary>
/// <param name="filePath">ファイルパス</param>
void StageManager::LoadGoalJsonFile(const std::wstring& filePath)
{
	//fstream作成
	std::fstream file(filePath, std::ifstream::in);
	//json読み込み
	nlohmann::json stageJson = nlohmann::json::parse(file);
	//ファイルを閉じる
	file.close();
	//座標取得
	DirectX::SimpleMath::Vector3 position = ConvertFloatArrayIntoVector3(stageJson["Goal"]["Position"]);
	//スケール読み込み
	DirectX::SimpleMath::Vector3 scale = ConvertFloatArrayIntoVector3(stageJson["Goal"]["Scale"]);
	//角度読み込み
	DirectX::SimpleMath::Vector3 rotation = ConvertFloatArrayIntoVector3(stageJson["Goal"]["Rotation"]);
	//生成
	std::unique_ptr<Actor> goal = std::make_unique<Goal>(
		position,
		DirectX::SimpleMath::Vector3::Zero,
		scale,
		rotation,
		ModelManager::GetInstance().LoadCmoModel(L"door.cmo"),
		true
		);

	//初期化
	goal->Initialize();
	//追加
	AddStage(std::move(goal));

}

/// <summary>
/// jsonで読み込んだ座標をVector3に変換
/// </summary>
/// <param name="nums">jsonで読み込んだ座標</param>
/// <returns>変換した座標</returns>
DirectX::SimpleMath::Vector3 StageManager::ConvertFloatArrayIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}



