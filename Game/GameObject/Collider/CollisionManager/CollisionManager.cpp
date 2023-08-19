#include"pch.h"
#include"CollisionManager.h"
#include"Game/GameObject/Collider/PolygonToLineSegment/PolygonToLineSegment.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Initialize()
{

}

void CollisionManager::Finalize()
{
}

void CollisionManager::Release()
{
}

bool CollisionManager::DetectCollisionPlayer2Goal()
{
	return m_playerAABB->DetectCollition(m_goalAABB);
}

bool CollisionManager::DetectCollisionPlayerLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos)
{
	bool isCollision = false;

	for (int i = 0; i < m_indexes.size(); i++)
	{
		std::vector<DirectX::SimpleMath::Vector3> vertex = 
		{ 
			m_vertexesPosition[m_indexes[i][0]],
			m_vertexesPosition[m_indexes[i][1]],
			m_vertexesPosition[m_indexes[i][2]] 
		};
		
		bool ishit = PolygonToLineSegmentCollision::DetectCollision(vertex, line,normal,actorPos);

		if (isCollision == false&& ishit == true)
		{
			isCollision = true;
		}

	}
	return isCollision;
}