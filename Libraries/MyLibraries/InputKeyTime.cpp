#include"pch.h"
#include "InputKeyTime.h"

InputKeyTime::InputKeyTime()
{

}

InputKeyTime::~InputKeyTime()
{

}

void InputKeyTime::Initialize()
{

}

void InputKeyTime::AddKeyEndTime(DirectX::Keyboard::Keys keys, float time)
{
	int keysNum = std::min(static_cast<int>(m_keyTimes[keys].size() - 1), 0);
	m_keyTimes[keys][keysNum].endTime_s = time;
}

void InputKeyTime::AddKeyStartTime(DirectX::Keyboard::Keys keys, float time)
{
	int keysNum=std::min(static_cast<int>(m_keyTimes[keys].size()-1), 0);
	//m_keyTimes[keys][keysNum].statTime_s = time;
	m_keyTimes[keys].push_back({ time ,0});
}

void InputKeyTime::Finalize()
{

}
