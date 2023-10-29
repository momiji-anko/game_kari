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
/// �R���W�����}�l�[�W���[
/// </summary>
class CollisionManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CollisionManager();
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~CollisionManager();

	/// <summary>
	/// ������	
	/// </summary>
	void Initialize();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// ���
	/// </summary>
	void Release();

	/// <summary>
	/// �����蔻��
	/// </summary>
	
	/// <summary>
	/// �v���C���[�ƃS�[���̓����蔻��
	/// </summary>
	/// <returns>�������Ă��邩</returns>
	bool DetectCollisionPlayer2Goal();

	/// <summary>
	/// �v���C���[�ƓG��AABB�����蔻��
	/// </summary>
	/// <returns>�������Ă��邩</returns>
	bool DetectCollisionPlayer2Enemies();
	/// <summary>
	/// �v���C���[�ƓG�̋��̓����蔻��
	/// </summary>
	/// <param name="enemySphere">�G�̋�</param>
	/// <returns>�������Ă��邩</returns>
	bool DetectCollisionPlayerSphere2EnemySphere(Sphere* enemySphere);
	/// <summary>
	/// �v���C���[�̍U������ƓG��AABB�����蔻��
	/// </summary>
	/// <param name="aabb">�G��AABB</param>
	/// <returns>�������Ă��邩</returns>
	bool DetectCollisionPlayerAttackAABB2Enemies(AABBFor3D* aabb);
	/// <summary>
	/// �v���C���[�ƌ��̓����蔻��
	/// </summary>
	/// <param name="aabb">���̋�</param>
	/// <returns>�������Ă��邩</returns>
	bool DetectCollisionPlayer2Keys(AABBFor3D* aabb);
	/// <summary>
	/// �v���C���[�ƓG�̓����蔻��
	/// </summary>
	/// <param name="aabb">�G��AABB</param>
	/// <returns>�������Ă��邩</returns>
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