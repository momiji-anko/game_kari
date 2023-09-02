#pragma once 
#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"

class Player :public Actor
{
public:
	static const float COLLISION_LINE_LENGTH;

	//	1�b�Ԃɗ�����}�X�̐�
	static const float GRAVITY_FORCE;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="velocity">�ړ���</param>
	/// <param name="scale">�g�k</param>
	/// <param name="rotation">�X�P�[��</param>
	/// <param name="model">���f��</param>
	/// <param name="active">�A�N�e�B�u</param>
	Player(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		DirectX::Model* model,
		bool active
	);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();

	/// <summary>
	/// ������	
	/// </summary>
	 void Initialize()override;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	 void Update(const DX::StepTimer& timer)override;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	 void Render(const Camera* camera)override;

	/// <summary>
	/// �I������
	/// </summary>
	 void Finalize()override;

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	 void Reset()override;

private:
	void PlayerMove(const DX::StepTimer& timer);

	void CollisionAreaUpdate();

private:
	//�萔=====================================================
	//	1�b�Ԃɐi�ރ}�X�̐�
	static const float  MOVE_SPEED;
	//	1�b�Ԃɗ�����}�X�̐�
	static const float GRAVITY_FORCE;
	//	�W�����v��
	static const float JUMP_FORCE;	
	//�������ۂ̎��S���鍂��
	static const float FALL_DEAD_AREA;
	//���f���^�C���̍ő吔
	static const float MAX_MODEL_TIME_S;
	//���f���^�C���̑��x
	static const float MODEL_TIME_SPEED;


private:
	Sphere m_sphere;

	std::unique_ptr<AABBFor3D> m_attackAABB;
};