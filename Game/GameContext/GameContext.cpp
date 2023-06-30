#include"pch.h"
#include"GameContext.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameContext::GameContext()
	:
	m_commonStates(nullptr),
	m_keyboardTracker(nullptr),
	m_spriteBath(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
GameContext::~GameContext()
{
	m_commonStates = nullptr;
	m_keyboardTracker = nullptr;
	m_spriteBath = nullptr;
}
