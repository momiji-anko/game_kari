#pragma once
#include<Keyboard.h>

#include<vector>
#include<unordered_map>

class InputKeyTime
{
public:

	struct KeyTime
	{
		float statTime_s;
		float endTime_s;
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	InputKeyTime();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~InputKeyTime();

	/// <summary>
	/// ������	
	/// </summary>
	void Initialize();


	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

private:
	//W�L�[���������̏��
	std::vector<KeyTime> m_WkeyKimes;
	//D�L�[���������̏��
	std::vector<KeyTime> m_DkeyKimes;
	//S�L�[���������̏��
	std::vector<KeyTime> m_SkeyKimes;
	//A�L�[���������̏��
	std::vector<KeyTime> m_AkeyKimes;
	//SPACE�L�[���������̏��
	std::vector<KeyTime> m_SpavekeyKimes;
	std::unordered_map<DirectX::Keyboard::Keys, std::vector<KeyTime>> m_keyTimes;


};
