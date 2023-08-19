#pragma once
#include"Game/GameObject/Actor/Actor.h"

class StageManager :public Actor
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
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

	void LoadObjFile(const std::wstring& filePath);

	void AddStage(std::unique_ptr<Actor>& stage) { m_stage.emplace_back(std::move(stage)); }

	void LoadGoalJsonFile(const std::wstring& filePath);

	/// <summary>
	/// json�œǂݍ��񂾍��W��Vector3�ɕϊ�
	/// </summary>
	/// <param name="nums">json�œǂݍ��񂾍��W</param>
	/// <returns>�ϊ��������W</returns>
	DirectX::SimpleMath::Vector3 ConvertFloatArrayIntoVector3(const std::vector<float> nums);

private:
	std::vector<std::unique_ptr<Actor>> m_stage;


	std::vector <DirectX::SimpleMath::Vector3> m_vertexesPosition;
	std::vector<std::vector<int>> m_indexes;
	int m_stageNum;
};
