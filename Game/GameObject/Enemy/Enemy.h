#pragma once 
#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Libraries/Animation/Animation.h"

class Enemy :public Actor
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
	/// <param name="active">�A�N�e�B�u</param>
	Enemy(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		DirectX::Model* model,
		bool active,
		std::vector<DirectX::SimpleMath::Vector3>& wanderPosition
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
	/// 
	/// </summary>
	/// <param name="velocity"></param>
	/// <param name="position"></param>
	/// <returns></returns>
	DirectX::SimpleMath::Vector3 Move(const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& position);

	/// <summary>
	/// 
	/// </summary>
	void CollisionAreaUpdate();

private:
	static const float ENEMY_SPEHERE_RADIUS;

	static const float MOVE_SPEED;

	static const float ENEMY_HEGHT_COLLISITION_LINE;
private:

	DX::AnimationSDKMESH  m_animSdk;
	DX::AnimationSDKMESH  m_animIdleSdk;

	//�A�j���[�V�����X�P���g��
	DirectX::ModelBone::TransformArray m_animBone;

	//�X�P���g��
	DirectX::ModelBone::TransformArray m_bone;

	Sphere m_sphere;

	std::vector<DirectX::SimpleMath::Vector3 >m_wanderPosition;

	DirectX::SimpleMath::Vector3 m_startPosition;

	int m_wanderNumber;

	bool m_groundHit;
};