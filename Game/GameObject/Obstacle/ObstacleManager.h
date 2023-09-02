#pragma once 
#include"Game/GameObject/Actor/Actor.h"

class ObstacleManager : public Actor
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ObstacleManager(int stageNum);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ObstacleManager();

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

	void LoadJsonFile(const std::wstring& filePath);

	/// <summary>
	/// json�œǂݍ��񂾍��W��Vector3�ɕϊ�
	/// </summary>
	/// <param name="nums">json�œǂݍ��񂾍��W</param>
	/// <returns>�ϊ��������W</returns>
	DirectX::SimpleMath::Vector3 ConvertFloatArrayIntoVector3(const std::vector<float> nums);

	void AddObstacle(std::unique_ptr<Actor> obstacle);
	void AddObstacleSpawner(std::unique_ptr<Actor> obstacleSpawner) { m_obstacleSpawner.emplace_back(std::move(obstacleSpawner)); }

	void DeleteObstacle();

private:
	/// <summary>
	/// ��Q���̍X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void ObstacleUpdate(const DX::StepTimer& timer);
	/// <summary>
	/// ��Q���X�|�i�[�̍X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void ObstacleSpawnerUpdate(const DX::StepTimer& timer);

	/// <summary>
	/// ��Q���̕`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	void ObstacleRender(const Camera* camera);

	/// <summary>
	/// ��Q���X�|�i�[�̕`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	void ObstacleSpawnerRender(const Camera* camera);

private:
	std::vector<std::unique_ptr<Actor>> m_obstacle;

	std::vector<DirectX::SimpleMath::Vector3> m_obstacleSpawnPosition;

	int m_stageNum;


	std::vector<std::unique_ptr<Actor>> m_obstacleSpawner;
};
