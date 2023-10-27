#pragma once
#include"Game/GameObject/Actor/Actor.h"

class EnemyManager :public Actor
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
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

	/// <summary>
	/// json�ǂݍ���
	/// </summary>
	/// <param name="jsonFilePath">json�t�@�C���p�X</param>
	void LoadEnemyJsonFile(std::wstring jsonFilePath);

	/// <summary>
	/// json�œǂݍ��񂾍��W��Vector3�ɕϊ�
	/// </summary>
	/// <param name="nums">json�œǂݍ��񂾍��W</param>
	/// <returns>�ϊ��������W</returns>
	DirectX::SimpleMath::Vector3 ConvertFloatArrayIntoVector3(const std::vector<float> nums);

	/// <summary>
	/// �G�̒ǉ�
	/// </summary>
	/// <param name="enemy">�G</param>
	void AddEnemy(std::unique_ptr<Actor> enemy) { m_enemies.emplace_back(std::move(enemy)); }

	/// <summary>
	/// �v���C���[��ǂ�������G�̐���
	/// </summary>
	/// <param name="time">����</param>
	void CreatePlayerTrackingEnemy(float time);

private:
	//�X�e�[�W�ԍ�
	int m_stageNum;

	//�G�z��
	std::vector<std::unique_ptr<Actor>> m_enemies;

};