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

	void CreateObstacle();

private:
	void FireUpdate(const DX::StepTimer& timer);

private:
	static const float OBSTACLE_SPAWN_COOL_TIME_S;

private:
	float m_area;

	ObstacleManager* m_obstacleManager;

	float m_obstacleSpawnCoolTime_s;

	Sphere m_sphere;

	DirectX::SimpleMath::Vector3 m_obstaceleScale;
	DirectX::SimpleMath::Vector3 m_obstacleRotation;
	float m_obstacleSpeed;


	std::unique_ptr<FireEffectManager> m_fireShaderManager;
};