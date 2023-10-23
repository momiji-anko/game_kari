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
	/// コンストラクタ
	/// </summary>
	InputKeyTime();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~InputKeyTime();

	/// <summary>
	/// 初期化	
	/// </summary>
	void Initialize();


	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

private:
	//Wキー押した時の情報
	std::vector<KeyTime> m_WkeyKimes;
	//Dキー押した時の情報
	std::vector<KeyTime> m_DkeyKimes;
	//Sキー押した時の情報
	std::vector<KeyTime> m_SkeyKimes;
	//Aキー押した時の情報
	std::vector<KeyTime> m_AkeyKimes;
	//SPACEキー押した時の情報
	std::vector<KeyTime> m_SpavekeyKimes;
	std::unordered_map<DirectX::Keyboard::Keys, std::vector<KeyTime>> m_keyTimes;


};
