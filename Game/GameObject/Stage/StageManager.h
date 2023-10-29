#pragma once
#include"Game/GameObject/Actor/Actor.h"

class StageManager :public Actor
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	StageManager(int stageNum);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~StageManager();

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
	/// OBJ�t�@�C���ǂݍ���
	/// </summary>
	/// <param name="filePath">OBJ�t�@�C���p�X</param>
	void LoadObjFile(const std::wstring& filePath);

	/// <summary>
	/// �X�e�[�W�ǉ�
	/// </summary>
	/// <param name="stage">�X�e�[�W</param>
	void AddStage(std::unique_ptr<Actor>& stage) { m_stage.emplace_back(std::move(stage)); }

	/// <summary>
	/// �S�[��Json��ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void LoadGoalJsonFile(const std::wstring& filePath);

	/// <summary>
	/// json�œǂݍ��񂾍��W��Vector3�ɕϊ�
	/// </summary>
	/// <param name="nums">json�œǂݍ��񂾍��W</param>
	/// <returns>�ϊ��������W</returns>
	DirectX::SimpleMath::Vector3 ConvertFloatArrayIntoVector3(const std::vector<float> nums);

private:
	//�X�e�[�W
	std::vector<std::unique_ptr<Actor>> m_stage;

	//���_���W
	std::vector <DirectX::SimpleMath::Vector3> m_vertexesPosition;
	//�C���f�b�N�X
	std::vector<std::vector<int>> m_indexes;
	//�X�e�[�W�ԍ�
	int m_stageNum;
};
