#include"pch.h"
#include"StageManager.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>

#include"Stage.h"
#include"Game/GameObject/Goal/Goal.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/Json/json.hpp"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="stageNum">�X�e�[�W�ԍ�</param>
StageManager::StageManager(int stageNum)
	:
	Actor(
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::Zero,
		nullptr,
		true
	),
	m_stageNum(stageNum)
{
	//OBJ�t�@�C���ǂݍ���
	std::vector<std::wstring> objFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Obj/");
	//Json�t�@�C���ǂݍ���
	std::vector<std::wstring> goalJsonFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Json/");
	
	//OBJ�t�@�C���ǂݍ��݃X�e�[�W�̓����蔻��쐬
	LoadObjFile(objFiles[m_stageNum]);
	//�S�[���ǂݍ���
	LoadGoalJsonFile(goalJsonFiles[m_stageNum]);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
StageManager::~StageManager()
{
}

/// <summary>
/// ������	
/// </summary>
void StageManager::Initialize()
{
	//�X�e�[�W�̐���
	std::unique_ptr<Actor> stage = std::make_unique<Stage>(
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::Zero, 
		DirectX::SimpleMath::Vector3::One, 
		DirectX::SimpleMath::Vector3::Zero,
		ModelManager::GetInstance().LoadCmoModel(L"stage1.cmo"),
		true
		);
	//�X�e�[�W�ǉ�
	AddStage(stage);

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void StageManager::Update(const DX::StepTimer& timer)
{
	//�X�e�[�W�X�V
	for (std::unique_ptr<Actor>& stage : m_stage)
	{
		stage->Update(timer);
	}
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void StageManager::Render(const Camera* camera)
{
	//�X�e�[�W�`��
	for (std::unique_ptr<Actor>& stage : m_stage)
	{
		stage->Render(camera);
	}
}

/// <summary>
/// �I������
/// </summary>
void StageManager::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void StageManager::Reset()
{
}

/// <summary>
/// OBJ�t�@�C���ǂݍ���
/// </summary>
/// <param name="filePath">OBJ�t�@�C���p�X</param>
void StageManager::LoadObjFile(const std::wstring& filePath)
{
	// obj�`���̃t�@�C���ǂݍ���
	//���_���W
	std::vector<DirectX::SimpleMath::Vector3> vertexes;
	//�C���f�b�N�X
	std::vector<int> indexes;

	//OBJ�t�@�C��
	std::ifstream ifs(filePath);

	//�ǂݍ��񂾕�����
	std::string str;
	//���ǂݍ���
	while (getline(ifs, str))
	{
		// ���_�ł���΍��W�𔲂����
		if (str[0] == 'v' && str[1] == ' ')
		{
			DirectX::SimpleMath::Vector3 val;
			sscanf_s(str.data(), "v  %f %f %f", &val.x, &val.y, &val.z);
			vertexes.push_back(val);
		}

		// �C���f�b�N�X�ł���΃C���f�b�N�X���𔲂����
		if (str[0] == 'f')
		{
			int a, b, c;
			int tmp1, tmp2, tmp3, tmp4;
			a = b = c = INT32_MAX;

			sscanf_s(str.data(), "f %d/%d %d/%d %d", &a, &tmp1, &b, &tmp2, &c);

			if (a == INT32_MAX || b == INT32_MAX || c == INT32_MAX)
			{
				sscanf_s(str.data(), "f %d/%d/%d %d/%d/%d %d", &a, &tmp1, &tmp3, &b, &tmp2, &tmp4, &c);
			}

			// �O�p�`�̒��_�C���f�b�N�X�ԍ������������܂ޏꍇ�͖�������
			if (a != b && a != c && b != c)
			{
				indexes.push_back(a - 1);
				indexes.push_back(c - 1);
				indexes.push_back(b - 1);
			}
		}
	}
	//�I�������t�@�C�������
	ifs.close();

	
	//�C���f�b�N�X���
	for (int i = 0; i < indexes.size() / 3; i++)
	{
		m_indexes.push_back({ indexes[i * 3], indexes[i * 3 + 1],indexes[i * 3 + 2] });
	}
	//���_���W���
	m_vertexesPosition = vertexes;

	//�R���W�����}�l�[�W���[�擾
	CollisionManager* collisionManager = GameContext::GetInstance().GetCollisionManager();

	//�R���W�����}�l�[�W���[��NULL�łȂ���΃R���W�����}�l�[�W���[�ɓ����蔻���o�^
	if (collisionManager != nullptr)
	{
		GameContext::GetInstance().GetCollisionManager()->SetVertexPosition(m_vertexesPosition);
		GameContext::GetInstance().GetCollisionManager()->SetIndexes(m_indexes);
	}
	
}

/// <summary>
/// �S�[��Json��ǂݍ���
/// </summary>
/// <param name="filePath">�t�@�C���p�X</param>
void StageManager::LoadGoalJsonFile(const std::wstring& filePath)
{
	//fstream�쐬
	std::fstream file(filePath, std::ifstream::in);
	//json�ǂݍ���
	nlohmann::json stageJson = nlohmann::json::parse(file);
	//�t�@�C�������
	file.close();
	//���W�擾
	DirectX::SimpleMath::Vector3 position = ConvertFloatArrayIntoVector3(stageJson["Goal"]["Position"]);
	//�X�P�[���ǂݍ���
	DirectX::SimpleMath::Vector3 scale = ConvertFloatArrayIntoVector3(stageJson["Goal"]["Scale"]);
	//�p�x�ǂݍ���
	DirectX::SimpleMath::Vector3 rotation = ConvertFloatArrayIntoVector3(stageJson["Goal"]["Rotation"]);
	//����
	std::unique_ptr<Actor> goal = std::make_unique<Goal>(
		position,
		DirectX::SimpleMath::Vector3::Zero,
		scale,
		rotation,
		ModelManager::GetInstance().LoadCmoModel(L"door.cmo"),
		true
		);

	//������
	goal->Initialize();
	//�ǉ�
	AddStage(std::move(goal));

}

/// <summary>
/// json�œǂݍ��񂾍��W��Vector3�ɕϊ�
/// </summary>
/// <param name="nums">json�œǂݍ��񂾍��W</param>
/// <returns>�ϊ��������W</returns>
DirectX::SimpleMath::Vector3 StageManager::ConvertFloatArrayIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}



