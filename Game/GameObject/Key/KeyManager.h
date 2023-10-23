#pragma once
#include"Game/GameObject/Actor/Actor.h"

class EnemyManager;

class KeyManager :public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	KeyManager(int stageNum);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~KeyManager();

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


	void AddKey(std::unique_ptr<Actor>& stage) { m_keys.emplace_back(std::move(stage)); }

	void LoadKeyJsonFile(const std::wstring& filePath);

	/// <summary>
	/// jsonで読み込んだ座標をVector3に変換
	/// </summary>
	/// <param name="nums">jsonで読み込んだ座標</param>
	/// <returns>変換した座標</returns>
	DirectX::SimpleMath::Vector3 ConvertFloatArrayIntoVector3(const std::vector<float> nums);

	/// <summary>
	/// プレイヤーが鍵をすべてゲットしたか確認
	/// </summary>
	void CheckALLGetKey();

private:
	//鍵
	std::vector<std::unique_ptr<Actor>> m_keys;
	//ステージの番号
	int m_stageNum;
	//鍵をすべて取ったか
	bool m_isAllGetKey;

	EnemyManager* m_enemyManager;

};
