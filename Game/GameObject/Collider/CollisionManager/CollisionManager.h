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

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	/// <summary>
	/// èâä˙âª	
	/// </summary>
	void Initialize();



	/// <summary>
	/// èIóπèàóù
	/// </summary>
	void Finalize();


	void Release();

	bool DetectCollisionPlayer2Goal();
	bool DetectCollisionPlayer2Enemies();
	bool DetectCollisionPlayerSphere2EnemySphere(Sphere* enemySphere);
	bool DetectCollisionPlayerAttackAABB2Enemies(AABBFor3D* aabb);
	bool DetectCollisionPlayer2Enemies(AABBFor3D* aabb);


	bool DetectCollisionPlayerLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos);
	bool DetectCollisionCameraLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos);

	void SetPlayerAttackAABB(AABBFor3D* aabb) { m_playerAttackAABB = aabb; }

	void SetPlayerAABB(AABBFor3D* aabb) { m_playerAABB = aabb; }
	void SetGoalAABB(AABBFor3D* aabb) { m_goalAABB = aabb; }

	void SetIndexes(const std::vector<std::vector<int>>& indexes) { m_indexes = indexes; }
	void SetVertexPosition(const std::vector<DirectX::SimpleMath::Vector3>& vertexesPosition) { m_vertexesPosition = vertexesPosition; }

	void SetPlayerSphere(Sphere* sphere) { m_playerSphere = sphere; }
	
	void AddEnemiesAABB(AABBFor3D* aabb) { m_enemiesAABB.push_back(aabb); }

private:
	std::vector<std::vector<int>> m_indexes;
	std::vector<DirectX::SimpleMath::Vector3> m_vertexesPosition;

	AABBFor3D* m_playerAABB;

	AABBFor3D* m_goalAABB;
	
	std::vector<AABBFor3D*> m_enemiesAABB;

	Sphere* m_playerSphere;

	AABBFor3D* m_playerAttackAABB;

};