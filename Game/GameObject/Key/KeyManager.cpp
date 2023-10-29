#include"pch.h"
#include"KeyManager.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstdlib>

#include"ClearKey.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"Libraries/MyLibraries/ModelManager.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/Json/json.hpp"
#include"Game/GameObject/Enemy/EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="stageNum">�X�e�[�W�ԍ�</param>
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
	m_stageNum{ stageNum },
	m_isAllGetKey{false},
	m_keys{}
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
	//���ׂĂ̌�������Ă��Ȃ�
	GameContext::GetInstance().SetIsAllGetKey(false);
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�^�C�}�[</param>
void KeyManager::Update(const DX::StepTimer& timer)
{
	//���X�V
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
	//���`��
	for (std::unique_ptr<Actor>& key : m_keys)
	{
		key->Render(camera);
	}
	//����UI�`��
	RenderKeyUI();
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
		std::unique_ptr<Actor> key = std::make_unique<ClearKey>(
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

/// <summary>
/// ����UI�`��
/// <summary>
void KeyManager::RenderKeyUI()
{
	//�X�v���C�g�o�b�`�̎擾
	DirectX::SpriteBatch* spriteBatch = GameContext::GetInstance().GetSpriteBatch();
	//�`��J�n
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, GameContext::GetInstance().GetCommonState()->NonPremultiplied());

	//�e�N�X�`���}�l�[�W���[�擾
	TextureManager& textureManager = TextureManager::GetInstance();
	//���̃e�N�X�`���擾
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> keyTex = textureManager.LoadTexture(L"key.png");
	//���̃e�N�X�`���T�C�Y�擾
	DirectX::SimpleMath::Vector2 texSize = textureManager.GetTextureSize(L"key.png");
	//���̑傫��
	float keyTexScale = 0.3f;
	//���̔z�񕪁A�e�N�X�`���`�悷��
	for (int i = 0; i < m_keys.size(); i++)
	{
		//����Ă����Ԃł���Ή������Ȃ�
		DirectX::SimpleMath::Color textureColor = DirectX::Colors::White;
		//����Ă��Ȃ��ꍇ�͊D�F�ɂ���
		if (m_keys[i]->IsActive())
			textureColor = DirectX::Colors::Gray;

		//���̕`��
		spriteBatch->Draw(keyTex.Get(), DirectX::SimpleMath::Vector2(texSize.x * keyTexScale * i, 0.0f), nullptr, textureColor, 0.0f, DirectX::SimpleMath::Vector2::Zero, keyTexScale);

	}

	spriteBatch->End();

}


