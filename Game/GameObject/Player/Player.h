#pragma once 
#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"
#include<Keyboard.h>

/// <summary>
/// �v���C���[
/// </summary>
class Player :public Actor
{
public:
	//�����̒���
	static const float COLLISION_LINE_LENGTH;

	//1�b�Ԃɗ�����}�X�̐�
	static const float GRAVITY_FORCE;

	//1�b�Ԃɐi�ރ}�X�̐�
	static const float  MOVE_SPEED;

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

	/// <summary>
	/// �ړ�
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void PlayerMove(const DX::StepTimer& timer);

	/// <summary>
	/// �����蔻��X�V
	/// </summary>
	void CollisionAreaUpdate();

	/// <summary>
	/// SDKMESH�X�V
	/// </summary>
	/// <param name="sdkMeshAnimation">�A�j���[�V����</param>
	/// <param name="elapsedTime">�t���[���Ԏ���</param>
	void SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation,float elapsedTime);

	/// <summary>
	/// �U������X�V
	/// </summary>
	/// <param name="isGroundHit">���n���Ă��邩</param>
	void AttackAreaUpdate(bool isGroundHit);

	/// <summary>
	/// �v���C���[�̈ړ�����
	/// </summary>
	/// <param name="keyState">�L�[</param>
	/// <returns>����</returns>
	float PlayerMoveAngle(DirectX::Keyboard::State& keyState);

	/// <summary>
	/// SDKMESH�쐬
	/// </summary>
	void CreateSdkMesh();


private:
	//�萔=====================================================
	//	�W�����v��
	static const float JUMP_FORCE;	
	//�������ۂ̎��S���鍂��
	static const float FALL_DEAD_AREA;
	//���f���^�C���̍ő吔
	static const float MAX_MODEL_TIME_S;
	//���f���^�C���̑��x
	static const int MODEL_TIME_SPEED;


private:
	//���̓����蔻��
	Sphere m_sphere;
	//�U���͈�
	std::unique_ptr<AABBFor3D> m_attackAABB;

	//�A�j���[�V����
	DX::AnimationSDKMESH  m_animWalkSdk;
	DX::AnimationSDKMESH  m_animIdleSdk;
	DX::AnimationSDKMESH  m_animJumpSdk;
	DX::AnimationSDKMESH  m_animDieSdk;
	DX::AnimationSDKMESH  m_animRunSdk;

	//�A�j���[�V�����X�P���g��
	DirectX::ModelBone::TransformArray m_animBone;

	//�X�P���g��
	DirectX::ModelBone::TransformArray m_bone;


	//�ړ��{�^������������
	bool m_isMoveKey;

	//���S�^�C��
	float m_deathTime;

};