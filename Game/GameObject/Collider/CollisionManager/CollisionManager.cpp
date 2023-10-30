#include"pch.h"
#include"CollisionManager.h"
#include"Game/GameObject/Collider/PolygonToLineSegment/PolygonToLineSegment.h"

/// <summary>
/// コンストラクタ
/// </summary>
CollisionManager::CollisionManager()
	:
	m_enemiesAABB{},
	m_fallDeathAABB{},
	m_goalAABB{},
	m_indexes{},
	m_vertexesPosition{},
	m_keysAABB{},
	m_playerAABB{},
	m_playerAttackAABB{},
	m_playerSphere{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
CollisionManager::~CollisionManager()
{
}

/// <summary>
/// 初期化	
/// </summary>
void CollisionManager::Initialize()
{

}

/// <summary>
/// 終了処理
/// </summary>
void CollisionManager::Finalize()
{
}

/// <summary>
/// 解放
/// </summary>
void CollisionManager::Release()
{
}

/// <summary>
/// 当たり判定
/// </summary>

/// <summary>
/// プレイヤーとゴールの当たり判定
/// </summary>
/// <returns>当たっているか</returns>
bool CollisionManager::DetectCollisionPlayer2Goal()
{
	//ゴールが存在している場合判定を行う
	if(m_goalAABB != nullptr)
	return m_playerAABB->DetectCollition(m_goalAABB);

	return false;
}

/// <summary>
/// プレイヤーと敵のAABB当たり判定
/// </summary>
/// <returns>当たっているか</returns>
bool CollisionManager::DetectCollisionPlayer2Enemies()
{
	//敵すべてと当たり判定を取る
	for (AABBFor3D* enemyAABB : m_enemiesAABB)
	{
		if (m_playerAABB->DetectCollition(enemyAABB))
		{
			return true;
		}
	}

	return false;
}

/// <summary>
/// プレイヤーと敵の球の当たり判定
/// </summary>
/// <param name="enemySphere">敵の球</param>
/// <returns>当たっているか</returns>
bool CollisionManager::DetectCollisionPlayerSphere2EnemySphere(Sphere* enemySphere)
{
	return SphereCollision::DetectCollition(*m_playerSphere,*enemySphere);
}

/// <summary>
/// プレイヤーの攻撃判定と敵のAABB当たり判定
/// </summary>
/// <param name="aabb">敵のAABB</param>
/// <returns>当たっているか</returns>
bool CollisionManager::DetectCollisionPlayerAttackAABB2Enemies(AABBFor3D* aabb)
{
	//プレイヤーの攻撃判定が存在している場合判定を取る
	if (m_playerAttackAABB != nullptr)
		return m_playerAttackAABB->DetectCollition(aabb);

	return false;
}

/// <summary>
/// プレイヤーと鍵の当たり判定
/// </summary>
/// <param name="aabb">鍵の球</param>
/// <returns>当たっているか</returns>
bool CollisionManager::DetectCollisionPlayer2Keys(AABBFor3D* aabb)
{
	return m_playerAABB->DetectCollition(aabb);
}

/// <summary>
/// プレイヤーと敵の当たり判定
/// </summary>
/// <param name="aabb">敵のAABB</param>
/// <returns>当たっているか</returns>
bool CollisionManager::DetectCollisionPlayer2Enemies(AABBFor3D* aabb)
{
	return m_playerAABB->DetectCollition(aabb);	
	
	return false;
}

/// <summary>
/// プレイヤーとステージポリゴンと当たり判定
/// </summary>
/// <param name="line">プレイヤーの線分</param>
/// <param name="normal">法線</param>
/// <param name="actorPos">めり込み判定後の座標</param>
/// <returns>当たっているか</returns>
bool CollisionManager::DetectCollisionPlayerLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos)
{
	//当たっているか
	bool isCollision = false;

	//すべてのポリゴンを当たり判定を取る
	for (int i = 0; i < m_indexes.size(); i++)
	{
		//頂点座標
		std::vector<DirectX::SimpleMath::Vector3> vertex = 
		{ 
			m_vertexesPosition[m_indexes[i][0]],
			m_vertexesPosition[m_indexes[i][1]],
			m_vertexesPosition[m_indexes[i][2]] 
		};
		
		//当たったか
		bool ishit = PolygonToLineSegmentCollision::DetectCollision(vertex, line,normal,actorPos);

		//すべてのポリゴンと当たり判定を取るためbreakしない
		//当たっているか
		if (isCollision == false&& ishit == true)
		{
			//当たっている状態にする
			isCollision = true;
		}

	}
	//結果を返す
	return isCollision;
}

/// <summary>
/// カメラとステージポリゴンと当たり判定
/// </summary>
/// <param name="line">カメラの線分</param>
/// <param name="normal">法線</param>
/// <param name="actorPos">当たっていた場合のカメラの場所</param>
/// <returns>当たっているか</returns>
bool CollisionManager::DetectCollisionCameraLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos)
{
	//当たっているか
	bool isCollision = false;

	//すべてのポリゴンを当たり判定を取る
	for (int i = 0; i < m_indexes.size(); i++)
	{
		//頂点座標
		std::vector<DirectX::SimpleMath::Vector3> vertex = 
		{ 
			m_vertexesPosition[m_indexes[i][0]],
			m_vertexesPosition[m_indexes[i][1]],
			m_vertexesPosition[m_indexes[i][2]] 
		};

		//当たったか
		bool ishit = PolygonToLineSegmentCollision::DetectCollision_Camera(vertex, line, normal, actorPos);

		//すべてのポリゴンと当たり判定を取るためbreakしない
		//当たっているか
		if (isCollision == false && ishit == true)
		{
			//当たっている状態にする
			isCollision = true;
		}

	}
	//結果を返す
	return isCollision;
}

/// <summary>
/// プレイヤーと落下エリアの当たり判定
/// </summary>
/// <returns>当たっているか</returns>
bool CollisionManager::DetectCollisionPlayer2FallDeathAABB()
{
	return m_playerAABB->DetectCollition(m_fallDeathAABB);
}
