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


	bool DetectCollisionPlayerLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos);
	bool DetectCollisionCameraLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos);

	bool DetectCollisionPlayer2FallDeathAABB();

	void SetPlayerAttackAABB(AABBFor3D* aabb) { m_playerAttackAABB = aabb; }

	void SetPlayerAABB(AABBFor3D* aabb) { m_playerAABB = aabb; }
	void SetGoalAABB(AABBFor3D* aabb) { m_goalAABB = aabb; }

	void SetIndexes(const std::vector<std::vector<int>>& indexes) { m_indexes = indexes; }
	void SetVertexPosition(const std::vector<DirectX::SimpleMath::Vector3>& vertexesPosition) { m_vertexesPosition = vertexesPosition; }

	void SetPlayerSphere(Sphere* sphere) { m_playerSphere = sphere; }
	void SetfallDeathAABB(AABBFor3D* aabb) { m_fallDeathAABB = aabb; }
	
	void AddEnemiesAABB(AABBFor3D* aabb) { m_enemiesAABB.push_back(aabb); }
	void AddKeysAABB(AABBFor3D* aabb) { m_keysAABB.push_back(aabb); }

private:
	std::vector<std::vector<int>> m_indexes;
	std::vector<DirectX::SimpleMath::Vector3> m_vertexesPosition;

	AABBFor3D* m_playerAABB;

	AABBFor3D* m_goalAABB;
	
	std::vector<AABBFor3D*> m_enemiesAABB;
	std::vector<AABBFor3D*> m_keysAABB;

	Sphere* m_playerSphere;

	AABBFor3D* m_playerAttackAABB;

	AABBFor3D* m_fallDeathAABB;

};