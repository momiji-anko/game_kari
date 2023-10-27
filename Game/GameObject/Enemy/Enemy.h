#pragma once 
#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"

/// <summary>
/// �G
/// </summary>
class Enemy :public Actor
{
public:
	//���G�͈͂̑傫��
	static const float ENEMY_SPEHERE_RADIUS;
	//�ړ����x
	static const float MOVE_SPEED;
	//�c�̐����̒���
	static const float ENEMY_HEGHT_COLLISITION_LINE;
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
	Enemy(
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
	~Enemy();

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
	/// �ړ��ł��邩����
	/// </summary>
	/// <param name="velocity">�ړ���</param>
	/// <param name="position">���݂̍��W</param>
	/// <returns>�ړ���</returns>
	DirectX::SimpleMath::Vector3 Move(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position);

	/// <summary>
	/// �����蔻��X�V
	/// </summary>
	void CollisionAreaUpdate();

	/// <summary>
	/// sdkmesh�̍쐬
	/// </summary>
	void CreateSdkMesh();
	
	/// <summary>
	/// sdkmesh�X�V
	/// </summary>
	/// <param name="sdkMeshAnimation">�A�j���[�V����</param>
	/// <param name="elapsedTime">�t���[���ԕb��</param>
	void SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime);



private:
	//�A�j���[�V����
	DX::AnimationSDKMESH  m_animSdk;
	DX::AnimationSDKMESH  m_animIdleSdk;

	//�A�j���[�V�����X�P���g��
	DirectX::ModelBone::TransformArray m_animBone;

	//�X�P���g��
	DirectX::ModelBone::TransformArray m_bone;

	//���̓����蔻��
	Sphere m_sphere;

	//�����ʒu
	DirectX::SimpleMath::Vector3 m_startPosition;

	//�n�ʂƓ������Ă��邩
	bool m_groundHit;
};