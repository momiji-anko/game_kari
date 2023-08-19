#pragma once
#include"Game/GameObject/Actor/Actor.h"

class StageManager :public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageManager(int stageNum);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageManager();

	/// <summary>
	/// 初期化	
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer)override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void Render(const Camera* camera)override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset()override;

	void LoadObjFile(const std::wstring& filePath);

	void AddStage(std::unique_ptr<Actor>& stage) { m_stage.emplace_back(std::move(stage)); }

	void LoadGoalJsonFile(const std::wstring& filePath);

	/// <summary>
	/// jsonで読み込んだ座標をVector3に変換
	/// </summary>
	/// <param name="nums">jsonで読み込んだ座標</param>
	/// <returns>変換した座標</returns>
	DirectX::SimpleMath::Vector3 ConvertFloatArrayIntoVector3(const std::vector<float> nums);

private:
	std::vector<std::unique_ptr<Actor>> m_stage;


	std::vector <DirectX::SimpleMath::Vector3> m_vertexesPosition;
	std::vector<std::vector<int>> m_indexes;
	int m_stageNum;
};
