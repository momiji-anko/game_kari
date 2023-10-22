#include"pch.h"
#include"KeyManager.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>

#include"Key.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/Json/json.hpp"

/// <summary>
/// �R���X�g���N�^
/// </summary>
KeyManager::KeyManager(int stageNum)
	:
	Actor{
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		DirectX::SimpleMath::Vector3::Zero,
		nullptr,
		true
	},
	m_stageNum{ stageNum }
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
KeyManager::~KeyManager()
{
}

/// <summary>
/// ������	
/// </summary>
void KeyManager::Initialize()
{
	//goalJson�̃t�@�C���p�X���擾
	std::vector<std::wstring> goalJsonFiles = FileLoadManager::GetInstance().LoadFile(L"Resources/Stage/Json/");

	//Json��ǂݍ��݌��̍쐬
	LoadKeyJsonFile(goalJsonFiles[m_stageNum]);

	GameContext::GetInstance().SetIsAllGetKey(false);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void KeyManager::Update(const DX::StepTimer& timer)
{
	for (std::unique_ptr<Actor>& key : m_keys)
	{
		key->Update(timer);
	}
	//���ׂĂ̌����擾�������m�F
	CheckALLGetKey();
}

/// <summary>
/// �`��
/// </summary>
/// <param name="camera">�J�����̐��|�C���^</param>
void KeyManager::Render(const Camera* camera)
{
	for (std::unique_ptr<Actor>& key : m_keys)
	{
		key->Render(camera);
	}
}

/// <summary>
/// �I������
/// </summary>
void KeyManager::Finalize()
{
}

/// <summary>
/// ���Z�b�g
/// </summary>
void KeyManager::Reset()
{
}

void KeyManager::LoadKeyJsonFile(const std::wstring& filePath)
{
	//fstream�쐬
	std::fstream file(filePath, std::ifstream::in);
	//json�ǂݍ���
	nlohmann::json keyJson = nlohmann::json::parse(file);
	//�t�@�C�������
	file.close();

	for (int i = 0; i < keyJson["Key"].size(); i++)
	{
		//���̍쐬
		std::unique_ptr<Actor> key = std::make_unique<Key>(
			ConvertFloatArrayIntoVector3(keyJson["Key"][i]["Position"]),
			DirectX::SimpleMath::Vector3::Zero,
			ConvertFloatArrayIntoVector3(keyJson["Key"][i]["Scale"]),
			ConvertFloatArrayIntoVector3(keyJson["Key"][i]["Rotation"]),
			ModelManager::GetInstance().LoadCmoModel(L"key.cmo"),
			true
			);
		key->Initialize();
		//���̒ǉ�
		AddKey(std::move(key));
	}


}


/// <summary>
/// json�œǂݍ��񂾍��W��Vector3�ɕϊ�
/// </summary>
/// <param name="nums">json�œǂݍ��񂾍��W</param>
/// <returns>�ϊ��������W</returns>
DirectX::SimpleMath::Vector3 KeyManager::ConvertFloatArrayIntoVector3(const std::vector<float> nums)
{
	return DirectX::SimpleMath::Vector3(nums[0], nums[1], nums[2]);
}

/// <summary>
/// �v���C���[���������ׂăQ�b�g�������m�F
/// </summary>
void KeyManager::CheckALLGetKey()
{
	//�Q�b�g�������̐�
	int getKeyNum = 0;
	for (std::unique_ptr<Actor>& key : m_keys)
	{
		//�����A�N�e�B�u��ԂłȂ���΃Q�b�g���Ă���
		getKeyNum += !key->IsActive();
	}

	//���ׂĂ̌����Q�b�g������
	bool isAllGet = (getKeyNum == m_keys.size());
	GameContext::GetInstance().SetIsAllGetKey(isAllGet);
}


