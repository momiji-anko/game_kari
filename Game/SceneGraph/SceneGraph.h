#pragma once
#include"Game/GameObject/Actor/Actor.h"
#include<vector>

class SceneGraph 
{

public:
	/// <summary>
	/// コンテキスト
	/// </summary>
	SceneGraph();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneGraph();

	/// <summary>
	/// 初期化	
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="timer">タイマー</param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラの生ポインタ</param>
	void Render(const Camera* camera);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// ノードを追加する
	/// </summary>
	/// <param name="sceneNode">ノード</param>
	void AttachNode(std::unique_ptr<Actor> sceneNode);
private:
	//シーングラフ
	std::vector<std::unique_ptr<Actor>> m_sceneGraph;
	// 位置
	DirectX::SimpleMath::Vector3 m_position;
};