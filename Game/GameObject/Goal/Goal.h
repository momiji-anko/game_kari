#pragma once 
#include"Game/GameObject/Actor/Actor.h"

class Goal : public Actor
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Goal(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model, bool active);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Goal();

	/// <summary>
	/// 初期化	
	/// </summary>
	void Initialize();

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

	
private:
};