#pragma once 
#include"Game/GameObject/Actor/Actor.h"


class Stage : public Actor
{
public:

	void SetVertexPosition(const std::vector<DirectX::SimpleMath::Vector3>& vertexPositon) { m_vertexPosition = vertexPositon; }
	std::vector<DirectX::SimpleMath::Vector3> GetVertexPosition()const { return m_vertexPosition; }

	void SetIndex(const std::vector<int>& index) { m_index = index; }
	std::vector<int> GetIndex()const { return m_index; }

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Stage();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Stage();

	/// <summary>
	/// 初期化	
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="velocity">移動量</param>
	/// <param name="scale">拡縮</param>
	/// <param name="rotation">スケール</param>
	/// <param name="model">モデル</param>
	/// <param name="active">アクティブ</param>
	void Initialize(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		DirectX::Model* model,
		bool active
	)override;

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

	std::vector<DirectX::SimpleMath::Vector3> m_vertexPosition;

	std::vector<int> m_index;
};
