#pragma once
#include"Game/GameObject/Actor/Actor.h"

class StageManager :public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
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

	/// <summary>
	/// OBJファイル読み込み
	/// </summary>
	/// <param name="filePath">OBJファイルパス</param>
	void LoadObjFile(const std::wstring& filePath);

	/// <summary>
	/// ステージ追加
	/// </summary>
	/// <param name="stage">ステージ</param>
	void AddStage(std::unique_ptr<Actor>& stage) { m_stage.emplace_back(std::move(stage)); }

	/// <summary>
	/// ゴールJsonを読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadGoalJsonFile(const std::wstring& filePath);

	/// <summary>
	/// jsonで読み込んだ座標をVector3に変換
	/// </summary>
	/// <param name="nums">jsonで読み込んだ座標</param>
	/// <returns>変換した座標</returns>
	DirectX::SimpleMath::Vector3 ConvertFloatArrayIntoVector3(const std::vector<float> nums);

private:
	//ステージ
	std::vector<std::unique_ptr<Actor>> m_stage;

	//頂点座標
	std::vector <DirectX::SimpleMath::Vector3> m_vertexesPosition;
	//インデックス
	std::vector<std::vector<int>> m_indexes;
	//ステージ番号
	int m_stageNum;
};
