#pragma once

#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"
#include<Keyboard.h>

/// <summary>
/// �v���C���[��ǂ�������G
/// </summary>
class PlayerTrackingEnemy :public Actor
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="velocity">���x</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="rotation">�p�x</param>
	/// <param name="model">���f��</param>
	/// <param name="second">���b�O�̃v���C���[�̓������R�s�[����b��</param>
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
	
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void Move(const DX::StepTimer& timer);

	/// <summary>
	/// �����蔻��X�V
	/// </summary>
	void CollisionAreaUpdate();

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
	
	//�����Ă�����
	bool m_isMove;
	
	//�v���C���[�̍��W���L�^����z��
	std::vector < DirectX::SimpleMath::Vector3 > m_playerPositions;
	//�v���C���[�̍��W���L�^����C���f�b�N�X
	int m_playerRecordIndex;
	//�v���C���[�ǐ՗p�C���f�b�N�X
	int m_playerTrackingIndex;
};