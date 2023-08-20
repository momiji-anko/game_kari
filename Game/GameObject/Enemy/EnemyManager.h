#pragma once
#include"Game/GameObject/Actor/Actor.h"

class EnemyManager :public Actor
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	EnemyManager(int stageNum);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~EnemyManager();

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

	void LoadEnemyJsonFile(std::wstring jsonFilePath);

	/// <summary>
	/// json�œǂݍ��񂾍��W��Vector3�ɕϊ�
	/// </summary>
	/// <param name="nums">json�œǂݍ��񂾍��W</param>
	/// <returns>�ϊ��������W</returns>
	DirectX::SimpleMath::Vector3 ConvertFloatArrayIntoVector3(const std::vector<float> nums);


	void AddEnemy(std::unique_ptr<Actor> enemy) { m_enemies.emplace_back(std::move(enemy)); }


private:

	int m_stageNum;

	std::vector<std::unique_ptr<Actor>> m_enemies;
};