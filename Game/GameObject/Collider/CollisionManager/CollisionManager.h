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

	/// <summary>
	/// �v���C���[�ƃX�e�[�W�|���S���Ɠ����蔻��
	/// </summary>
	/// <param name="line">�v���C���[�̐���</param>
	/// <param name="normal">�@��</param>
	/// <param name="actorPos">�߂荞�ݔ����̍��W</param>
	/// <returns>�������Ă��邩</returns>
	bool DetectCollisionPlayerLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos);
	
	/// <summary>
	/// �J�����ƃX�e�[�W�|���S���Ɠ����蔻��
	/// </summary>
	/// <param name="line">�J�����̐���</param>
	/// <param name="normal">�@��</param>
	/// <param name="actorPos">�������Ă����ꍇ�̃J�����̏ꏊ</param>
	/// <returns>�������Ă��邩</returns>
	bool DetectCollisionCameraLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos);

	/// <summary>
	/// �v���C���[�Ɨ����G���A�̓����蔻��
	/// </summary>
	/// <returns>�������Ă��邩</returns>
	bool DetectCollisionPlayer2FallDeathAABB();

	/// <summary>
	/// �v���C���[�U������̐ݒ�
	/// </summary>
	/// <param name="aabb">�v���C���[�U������</param>
	void SetPlayerAttackAABB(AABBFor3D* aabb) { m_playerAttackAABB = aabb; }

	/// <summary>
	/// �v���C���[��AABB�����蔻��ݒ�
	/// </summary>
	/// <param name="aabb">�v���C���[��AABB</param>
	void SetPlayerAABB(AABBFor3D* aabb) { m_playerAABB = aabb; }
	/// <summary>
	/// �S�[��AABB�����蔻��ݒ�
	/// </summary>
	/// <param name="aabb">�S�[��AABB</param>
	void SetGoalAABB(AABBFor3D* aabb) { m_goalAABB = aabb; }
	/// <summary>
	/// �X�e�[�W�̒��_�ԍ��ݒ�
	/// </summary>
	/// <param name="indexes">���_�ԍ�</param>
	void SetIndexes(const std::vector<std::vector<int>>& indexes) { m_indexes = indexes; }
	/// <summary>
	/// �X�e�[�W�̒��_���W�ݒ�
	/// </summary>
	/// <param name="vertexesPosition">���_���W</param>
	void SetVertexPosition(const std::vector<DirectX::SimpleMath::Vector3>& vertexesPosition) { m_vertexesPosition = vertexesPosition; }
	/// <summary>
	/// �v���C���[�̋������蔻��ݒ�
	/// </summary>
	/// <param name="sphere">��</param>
	void SetPlayerSphere(Sphere* sphere) { m_playerSphere = sphere; }
	/// <summary>
	/// �������S�G���A�ݒ�
	/// </summary>
	/// <param name="aabb">AABB�����蔻��</param>
	void SetfallDeathAABB(AABBFor3D* aabb) { m_fallDeathAABB = aabb; }
	
	/// <summary>
	/// �G��AABB����ǉ�
	/// </summary>
	/// <param name="aabb">AABB</param>
	void AddEnemiesAABB(AABBFor3D* aabb) { m_enemiesAABB.push_back(aabb); }
	/// <summary>
	/// ����AABB�����蔻��ǉ�
	/// </summary>
	/// <param name="aabb">AABB</param>
	void AddKeysAABB(AABBFor3D* aabb) { m_keysAABB.push_back(aabb); }


private:
	//���_�ԍ�
	std::vector<std::vector<int>> m_indexes;
	//���_���W
	std::vector<DirectX::SimpleMath::Vector3> m_vertexesPosition;

	//�v���C���[AABB
	AABBFor3D* m_playerAABB;
	//�S�[��AABB
	AABBFor3D* m_goalAABB;

	//�GAABB
	std::vector<AABBFor3D*> m_enemiesAABB;
	//��AABB
	std::vector<AABBFor3D*> m_keysAABB;

	//�v���C���[�̋�
	Sphere* m_playerSphere;

	//�v���C���[�̍U������
	AABBFor3D* m_playerAttackAABB;
	//�������S�G���A
	AABBFor3D* m_fallDeathAABB;

};