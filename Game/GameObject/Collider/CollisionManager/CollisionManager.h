#pragma once
#include<unordered_map>
#include<functional>

#include"StepTimer.h"
#include"Libraries/MyLibraries/Singleton.h"

#include"Game/GameObject/Collider/AABBFor3D/AABBFor3D.h"
#include"Game/GameObject/Collider/Capsule/Capsule.h"
#include"Game/GameObject/Collider/PolygonToLineSegment/PolygonToLineSegment.h"

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
	bool DetectCollisionPlayerLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos);


	void SetPlayerAABB(AABBFor3D* aabb) { m_playerAABB = aabb; }
	void SetGoalAABB(AABBFor3D* aabb) { m_goalAABB = aabb; }

	void SetIndexes(const std::vector<std::vector<int>>& indexes) { m_indexes = indexes; }
	void SetVertexPosition(const std::vector<DirectX::SimpleMath::Vector3>& vertexesPosition) { m_vertexesPosition = vertexesPosition; }



private:
	std::vector<std::vector<int>> m_indexes;
	std::vector<DirectX::SimpleMath::Vector3> m_vertexesPosition;

	AABBFor3D* m_playerAABB;
	AABBFor3D* m_goalAABB;
	

};