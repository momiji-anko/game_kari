#pragma once
#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"
#include<Keyboard.h>

class PlayerTrackingEnemy :public Actor
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="velocity">�ړ���</param>
	/// <param name="scale">�g�k</param>
	/// <param name="rotation">�X�P�[��</param>
	/// <param name="model">���f��</param>
	/// <param name="second">�b��</param>
	PlayerTrackingEnemy(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		DirectX::Model* model,
		float second
	);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayerTrackingEnemy();

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
	void CreateSdkMesh();

	void SdkMeshUpdate(DX::AnimationSDKMESH* sdkMeshAnimation, float elapsedTime);

	void Move(const DX::StepTimer& timer);

private:
	//�萔=====================================================
	//	1�b�Ԃɐi�ރ}�X�̐�
	static const float  MOVE_SPEED;
	//	1�b�Ԃɗ�����}�X�̐�
	static const float GRAVITY_FORCE;
	//	�W�����v��
	static const float JUMP_FORCE;

private:

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

	bool m_isMove;

	std::vector < DirectX::SimpleMath::Vector3 > m_playerPositions;
	int m_nowIndex;
	int m_index;
};