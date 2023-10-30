#pragma once
#include<unordered_map>
#include<functional>

#include"StepTimer.h"
#include"Libraries/MyLibraries/Singleton.h"

#include"Game/GameObject/Collider/AABBFor3D/AABBFor3D.h"
#include"Game/GameObject/Collider/Capsule/Capsule.h"
#include"Game/GameObject/Collider/PolygonToLineSegment/PolygonToLineSegment.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"

class Camera;

/// <summary>
/// コリジョンマネージャー
/// </summary>
class CollisionManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CollisionManager();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~CollisionManager();

	/// <summary>
	/// 初期化	
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 解放
	/// </summary>
	void Release();

	/// <summary>
	/// 当たり判定
	/// </summary>
	
	/// <summary>
	/// プレイヤーとゴールの当たり判定
	/// </summary>
	/// <returns>当たっているか</returns>
	bool DetectCollisionPlayer2Goal();

	/// <summary>
	/// プレイヤーと敵のAABB当たり判定
	/// </summary>
	/// <returns>当たっているか</returns>
	bool DetectCollisionPlayer2Enemies();

	/// <summary>
	/// プレイヤーと敵の球の当たり判定
	/// </summary>
	/// <param name="enemySphere">敵の球</param>
	/// <returns>当たっているか</returns>
	bool DetectCollisionPlayerSphere2EnemySphere(Sphere* enemySphere);

	/// <summary>
	/// プレイヤーの攻撃判定と敵のAABB当たり判定
	/// </summary>
	/// <param name="aabb">敵のAABB</param>
	/// <returns>当たっているか</returns>
	bool DetectCollisionPlayerAttackAABB2Enemies(AABBFor3D* aabb);

	/// <summary>
	/// プレイヤーと鍵の当たり判定
	/// </summary>
	/// <param name="aabb">鍵の球</param>
	/// <returns>当たっているか</returns>
	bool DetectCollisionPlayer2Keys(AABBFor3D* aabb);

	/// <summary>
	/// プレイヤーと敵の当たり判定
	/// </summary>
	/// <param name="aabb">敵のAABB</param>
	/// <returns>当たっているか</returns>
	bool DetectCollisionPlayer2Enemies(AABBFor3D* aabb);

	/// <summary>
	/// プレイヤーとステージポリゴンと当たり判定
	/// </summary>
	/// <param name="line">プレイヤーの線分</param>
	/// <param name="normal">法線</param>
	/// <param name="actorPos">めり込み判定後の座標</param>
	/// <returns>当たっているか</returns>
	bool DetectCollisionPlayerLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos);
	
	/// <summary>
	/// カメラとステージポリゴンと当たり判定
	/// </summary>
	/// <param name="line">カメラの線分</param>
	/// <param name="normal">法線</param>
	/// <param name="actorPos">当たっていた場合のカメラの場所</param>
	/// <returns>当たっているか</returns>
	bool DetectCollisionCameraLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos);

	/// <summary>
	/// プレイヤーと落下エリアの当たり判定
	/// </summary>
	/// <returns>当たっているか</returns>
	bool DetectCollisionPlayer2FallDeathAABB();

	/// <summary>
	/// プレイヤー攻撃判定の設定
	/// </summary>
	/// <param name="aabb">プレイヤー攻撃判定</param>
	void SetPlayerAttackAABB(AABBFor3D* aabb) { m_playerAttackAABB = aabb; }

	/// <summary>
	/// プレイヤーのAABB当たり判定設定
	/// </summary>
	/// <param name="aabb">プレイヤーのAABB</param>
	void SetPlayerAABB(AABBFor3D* aabb) { m_playerAABB = aabb; }
	/// <summary>
	/// ゴールAABB当たり判定設定
	/// </summary>
	/// <param name="aabb">ゴールAABB</param>
	void SetGoalAABB(AABBFor3D* aabb) { m_goalAABB = aabb; }
	/// <summary>
	/// ステージの頂点番号設定
	/// </summary>
	/// <param name="indexes">頂点番号</param>
	void SetIndexes(const std::vector<std::vector<int>>& indexes) { m_indexes = indexes; }
	/// <summary>
	/// ステージの頂点座標設定
	/// </summary>
	/// <param name="vertexesPosition">頂点座標</param>
	void SetVertexPosition(const std::vector<DirectX::SimpleMath::Vector3>& vertexesPosition) { m_vertexesPosition = vertexesPosition; }
	/// <summary>
	/// プレイヤーの球当たり判定設定
	/// </summary>
	/// <param name="sphere">球</param>
	void SetPlayerSphere(Sphere* sphere) { m_playerSphere = sphere; }
	/// <summary>
	/// 落下死亡エリア設定
	/// </summary>
	/// <param name="aabb">AABB当たり判定</param>
	void SetfallDeathAABB(AABBFor3D* aabb) { m_fallDeathAABB = aabb; }
	
	/// <summary>
	/// 敵のAABB判定追加
	/// </summary>
	/// <param name="aabb">AABB</param>
	void AddEnemiesAABB(AABBFor3D* aabb) { m_enemiesAABB.push_back(aabb); }
	/// <summary>
	/// 鍵のAABB当たり判定追加
	/// </summary>
	/// <param name="aabb">AABB</param>
	void AddKeysAABB(AABBFor3D* aabb) { m_keysAABB.push_back(aabb); }


private:
	//頂点番号
	std::vector<std::vector<int>> m_indexes;
	//頂点座標
	std::vector<DirectX::SimpleMath::Vector3> m_vertexesPosition;

	//プレイヤーAABB
	AABBFor3D* m_playerAABB;
	//ゴールAABB
	AABBFor3D* m_goalAABB;

	//敵AABB
	std::vector<AABBFor3D*> m_enemiesAABB;
	//鍵AABB
	std::vector<AABBFor3D*> m_keysAABB;

	//プレイヤーの球
	Sphere* m_playerSphere;

	//プレイヤーの攻撃判定
	AABBFor3D* m_playerAttackAABB;
	//落下死亡エリア
	AABBFor3D* m_fallDeathAABB;

};