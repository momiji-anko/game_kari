#pragma once 

#include"Game/GameObject/Actor/Actor.h"
#include"Game/GameObject/Collider/Sphere/Sphere.h"
#include"Game/Shader/FireEffectManager.h"

class ObstacleManager;

class  ObstacleSpawner : public Actor
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="obstacleManager">��Q���}�l�[�W���[</param>
	/// <param name="area">�͈�</param>
	/// <param name="obstacleSpeed">��Q���̑��x</param>
	/// <param name="obstacleScale">��Q���̊g�嗦</param>
	/// <param name="obstacleRotation">��Q���̊p�x</param>
	/// <param name="model">���f��</param>
	ObstacleSpawner(DirectX::SimpleMath::Vector3 position, ObstacleManager* obstacleManager, float area,float obstacleSpeed,DirectX::SimpleMath::Vector3 obstacleScale,DirectX::SimpleMath::Vector3 obstacleRotation,DirectX::Model* model);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ObstacleSpawner();

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

	/// <summary>
	/// ��Q���쐬
	/// </summary>
	void CreateObstacle();

private:
	/// <summary>
	/// �G�t�F�N�g�̍X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void FireUpdate(const DX::StepTimer& timer);

private:
	//��Q�������N�[���^�C��
	static const float OBSTACLE_SPAWN_COOL_TIME_S;

private:
	//��Q�������͈�
	float m_area;
	//��Q���}�l�[�W���[
	ObstacleManager* m_obstacleManager;
	//��Q�������N�[���^�C��
	float m_obstacleSpawnCoolTime_s;
	//���̓����蔻��
	Sphere m_sphere;
	//��Q���g�嗦
	DirectX::SimpleMath::Vector3 m_obstaceleScale;
	//��Q���p�x
	DirectX::SimpleMath::Vector3 m_obstacleRotation;
	//��Q���̃X�s�[�h
	float m_obstacleSpeed;

	//�G�t�F�N�g
	std::unique_ptr<FireEffectManager> m_fireShaderManager;
};