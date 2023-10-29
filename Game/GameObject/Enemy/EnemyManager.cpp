#include"pch.h"
#include"EnemyManager.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>

#include"Enemy.h"
#include"PlayerTrackingEnemy.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/Json/json.hpp"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="stageNum">�X�e�[�W�ԍ�</param>
EnemyManager::EnemyManager(int stageNum)
	:
	Actor(
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		nullptr,
		true
	),
	m_enemies{},
	m_stageNum{stageNum},
	m_trackStartTime{2.0f}
{
	
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyManager::~EnemyManager()
{

}

/// <summary>
/// ������	
/// </summary>
void EnemyManager::Initialize()
{
	//�G��json�t�@�C�����ׂ�
	std::vector<std::wstring> enemyJsonFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Json/");

	//json�ǂݍ���
	LoadEnemyJsonFile(enemyJsonFiles[m_stageNum]);

	//�G�}�l�[�W���[�̐ݒ�
	GameContext::GetInstance().SetEnemyManager(this);

	//���b�O�̃v���C���[��ǐՂ��鎞�Ԃ�������	
	m_trackStartTime = 2.0f;
}


/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void EnemyManager::Update(const DX::StepTimer& timer)
{
	//�G�̍X�V
	for (std::unique_ptr<Actor>& enemy : m_enemies)
	{
		if (enemy == nullptr)
			continue;

		enemy->Update(timer);
	}
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void EnemyManager::Render(const Camera* camera)
{
	//�G�̕`��
	for (std::unique_ptr<Actor>& enemy : m_enemies)
	{
		if (enemy == nullptr)
			continue;

		enemy->Render(camera);
	}
}

/// <summary>
/// �I������
/// </summary>
void EnemyManager::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void EnemyManager::Reset()
{
}

/// <summary>
/// json�ǂݍ���
/// </summary>
/// <param name="jsonFilePath">json�t�@�C���p�X</param>
void EnemyManager::LoadEnemyJsonFile(std::wstring jsonFilePath)
{
	//fstream�쐬
	std::fstream file(jsonFilePath, std::ifstream::in);
	//json�ǂݍ���
	nlohmann::json enemyJson = nlohmann::json::parse(file);
	//�t�@�C�������
	file.close();

	//�G�̔z��̃T�C�Y
	int enemyNum = enemyJson["Enemy"].size();

	//�G�̐���
	for (int i = 0; i < enemyNum; i++)
	{
		//���W
		DirectX::SimpleMath::Vector3 position = ConvertFloatArrayIntoVector3(enemyJson["Enemy"][i]["Position"]);
		//�g�嗦
		DirectX::SimpleMath::Vector3 scale = ConvertFloatArrayIntoVector3(enemyJson["Enemy"][i]["Scale"]);
		//�p�x
		DirectX::SimpleMath::Vector3 rotation = ConvertFloatArrayIntoVector3(enemyJson["Enemy"][i]["Rotation"]);

		//�G�̐���
		std::unique_ptr<Actor> enemy = std::make_unique<Enemy>(position, DirectX::SimpleMath::Vector3::Zero, scale, rotation, ModelManager::GetInstance().LoadSdkmeshModel(L"enemy.sdkmesh"), true);
		//������
		enemy->Initialize();
		//�G�̒ǉ�
		AddEnemy(std::move(enemy));
	}
}

/// <summary>
/// json�œǂݍ��񂾍��W��Vector3�ɕϊ�
/// </summary>
/// <param name="nums">json�œǂݍ��񂾍��W</param>
/// <returns>�ϊ��������W</returns>
DirectX::SimpleMath::Vector3 EnemyManager::ConvertFloatArrayIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}

/// <summary>
/// �v���C���[��ǂ�������G�̐���
/// </summary>
void EnemyManager::CreatePlayerTrackingEnemy()
{
	//���W
	DirectX::SimpleMath::Vector3 position = GameContext::GetInstance().GetPlayerPosition();
	//�ړ�
	DirectX::SimpleMath::Vector3 velocity = DirectX::SimpleMath::Vector3::Zero;
	//�p�x
	DirectX::SimpleMath::Vector3 rotation = DirectX::SimpleMath::Vector3::Zero;
	//�g�嗦
	DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(0.02);
	
	//����
	std::unique_ptr<Actor> enemy = std::make_unique<PlayerTrackingEnemy>(position, DirectX::SimpleMath::Vector3::Zero, scale, rotation, ModelManager::GetInstance().LoadSdkmeshModel(L"enemy.sdkmesh"), m_trackStartTime);
	//������
	enemy->Initialize();
	
	//�ǉ�
	AddEnemy(std::move(enemy));

	//���Ԃ𔼕��ɂ���
	m_trackStartTime /= 2.0f;

}
