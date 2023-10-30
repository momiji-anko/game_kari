#include"pch.h"
#include"CollisionManager.h"
#include"Game/GameObject/Collider/PolygonToLineSegment/PolygonToLineSegment.h"

/// <summary>
/// �R���X�g���N�^
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
/// �f�X�g���N�^
/// </summary>
CollisionManager::~CollisionManager()
{
}

/// <summary>
/// ������	
/// </summary>
void CollisionManager::Initialize()
{

}

/// <summary>
/// �I������
/// </summary>
void CollisionManager::Finalize()
{
}

/// <summary>
/// ���
/// </summary>
void CollisionManager::Release()
{
}

/// <summary>
/// �����蔻��
/// </summary>

/// <summary>
/// �v���C���[�ƃS�[���̓����蔻��
/// </summary>
/// <returns>�������Ă��邩</returns>
bool CollisionManager::DetectCollisionPlayer2Goal()
{
	//�S�[�������݂��Ă���ꍇ������s��
	if(m_goalAABB != nullptr)
	return m_playerAABB->DetectCollition(m_goalAABB);

	return false;
}

/// <summary>
/// �v���C���[�ƓG��AABB�����蔻��
/// </summary>
/// <returns>�������Ă��邩</returns>
bool CollisionManager::DetectCollisionPlayer2Enemies()
{
	//�G���ׂĂƓ����蔻������
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
/// �v���C���[�ƓG�̋��̓����蔻��
/// </summary>
/// <param name="enemySphere">�G�̋�</param>
/// <returns>�������Ă��邩</returns>
bool CollisionManager::DetectCollisionPlayerSphere2EnemySphere(Sphere* enemySphere)
{
	return SphereCollision::DetectCollition(*m_playerSphere,*enemySphere);
}

/// <summary>
/// �v���C���[�̍U������ƓG��AABB�����蔻��
/// </summary>
/// <param name="aabb">�G��AABB</param>
/// <returns>�������Ă��邩</returns>
bool CollisionManager::DetectCollisionPlayerAttackAABB2Enemies(AABBFor3D* aabb)
{
	//�v���C���[�̍U�����肪���݂��Ă���ꍇ��������
	if (m_playerAttackAABB != nullptr)
		return m_playerAttackAABB->DetectCollition(aabb);

	return false;
}

/// <summary>
/// �v���C���[�ƌ��̓����蔻��
/// </summary>
/// <param name="aabb">���̋�</param>
/// <returns>�������Ă��邩</returns>
bool CollisionManager::DetectCollisionPlayer2Keys(AABBFor3D* aabb)
{
	return m_playerAABB->DetectCollition(aabb);
}

/// <summary>
/// �v���C���[�ƓG�̓����蔻��
/// </summary>
/// <param name="aabb">�G��AABB</param>
/// <returns>�������Ă��邩</returns>
bool CollisionManager::DetectCollisionPlayer2Enemies(AABBFor3D* aabb)
{
	return m_playerAABB->DetectCollition(aabb);	
	
	return false;
}

/// <summary>
/// �v���C���[�ƃX�e�[�W�|���S���Ɠ����蔻��
/// </summary>
/// <param name="line">�v���C���[�̐���</param>
/// <param name="normal">�@��</param>
/// <param name="actorPos">�߂荞�ݔ����̍��W</param>
/// <returns>�������Ă��邩</returns>
bool CollisionManager::DetectCollisionPlayerLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos)
{
	//�������Ă��邩
	bool isCollision = false;

	//���ׂẴ|���S���𓖂��蔻������
	for (int i = 0; i < m_indexes.size(); i++)
	{
		//���_���W
		std::vector<DirectX::SimpleMath::Vector3> vertex = 
		{ 
			m_vertexesPosition[m_indexes[i][0]],
			m_vertexesPosition[m_indexes[i][1]],
			m_vertexesPosition[m_indexes[i][2]] 
		};
		
		//����������
		bool ishit = PolygonToLineSegmentCollision::DetectCollision(vertex, line,normal,actorPos);

		//���ׂẴ|���S���Ɠ����蔻�����邽��break���Ȃ�
		//�������Ă��邩
		if (isCollision == false&& ishit == true)
		{
			//�������Ă����Ԃɂ���
			isCollision = true;
		}

	}
	//���ʂ�Ԃ�
	return isCollision;
}

/// <summary>
/// �J�����ƃX�e�[�W�|���S���Ɠ����蔻��
/// </summary>
/// <param name="line">�J�����̐���</param>
/// <param name="normal">�@��</param>
/// <param name="actorPos">�������Ă����ꍇ�̃J�����̏ꏊ</param>
/// <returns>�������Ă��邩</returns>
bool CollisionManager::DetectCollisionCameraLine2Polygon(const std::vector<DirectX::SimpleMath::Vector3>& line, DirectX::SimpleMath::Vector3& normal, DirectX::SimpleMath::Vector3& actorPos)
{
	//�������Ă��邩
	bool isCollision = false;

	//���ׂẴ|���S���𓖂��蔻������
	for (int i = 0; i < m_indexes.size(); i++)
	{
		//���_���W
		std::vector<DirectX::SimpleMath::Vector3> vertex = 
		{ 
			m_vertexesPosition[m_indexes[i][0]],
			m_vertexesPosition[m_indexes[i][1]],
			m_vertexesPosition[m_indexes[i][2]] 
		};

		//����������
		bool ishit = PolygonToLineSegmentCollision::DetectCollision_Camera(vertex, line, normal, actorPos);

		//���ׂẴ|���S���Ɠ����蔻�����邽��break���Ȃ�
		//�������Ă��邩
		if (isCollision == false && ishit == true)
		{
			//�������Ă����Ԃɂ���
			isCollision = true;
		}

	}
	//���ʂ�Ԃ�
	return isCollision;
}

/// <summary>
/// �v���C���[�Ɨ����G���A�̓����蔻��
/// </summary>
/// <returns>�������Ă��邩</returns>
bool CollisionManager::DetectCollisionPlayer2FallDeathAABB()
{
	return m_playerAABB->DetectCollition(m_fallDeathAABB);
}
