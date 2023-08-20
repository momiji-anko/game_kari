#pragma once
#include"Game/GameObject/Actor/Actor.h"

class EnemyManager :public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyManager(int stageNum);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyManager();

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

	void LoadEnemyJsonFile(std::wstring jsonFilePath);

	/// <summary>
	/// jsonで読み込んだ座標をVector3に変換
	/// </summary>
	/// <param name="nums">jsonで読み込んだ座標</param>
	/// <returns>変換した座標</returns>
	DirectX::SimpleMath::Vector3 ConvertFloatArrayIntoVector3(const std::vector<float> nums);


	void AddEnemy(std::unique_ptr<Actor> enemy) { m_enemies.emplace_back(std::move(enemy)); }


private:

	int m_stageNum;

	std::vector<std::unique_ptr<Actor>> m_enemies;
};