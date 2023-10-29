#pragma once 
#include"Game/GameObject/Actor/Actor.h"

/// <summary>
/// 障害物マネージャー
/// </summary>
class ObstacleManager : public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObstacleManager(int stageNum);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ObstacleManager();

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

	void LoadJsonFile(const std::wstring& filePath);

	/// <summary>
	/// jsonで読み込んだ座標をVector3に変換
	/// </summary>
	/// <param name="nums">jsonで読み込んだ座標</param>
	/// <returns>変換した座標</returns>
	DirectX::SimpleMath::Vector3 ConvertFloatArrayIntoVector3(const std::vector<float> nums);

	void AddObstacle(std::unique_ptr<Actor> obstacle);
	void AddObstacleSpawner(std::unique_ptr<Actor> obstacleSpawner) { m_obstacleSpawner.emplace_back(std::move(obstacleSpawner)); }

	void DeleteObstacle();

private:
	/// <summary>
	/// 障害物の更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void ObstacleUpdate(const DX::StepTimer& timer);
	/// <summary>
	/// 障害物スポナーの更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void ObstacleSpawnerUpdate(const DX::StepTimer& timer);

	/// <summary>
	/// 障害物の描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void ObstacleRender(const Camera* camera);

	/// <summary>
	/// 障害物スポナーの描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void ObstacleSpawnerRender(const Camera* camera);

private:
	//障害物
	std::vector<std::unique_ptr<Actor>> m_obstacle;
	//障害物のスポーン座標
	std::vector<DirectX::SimpleMath::Vector3> m_obstacleSpawnPosition;
	//ステージ番号
	int m_stageNum;

	//スポナー
	std::vector<std::unique_ptr<Actor>> m_obstacleSpawner;
};
