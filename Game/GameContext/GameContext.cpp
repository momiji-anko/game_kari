#include"pch.h"
#include"GameContext.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameContext::GameContext()
	:
	m_commonStates(nullptr),
	m_keyboardTracker(nullptr),
	m_spriteBath(nullptr)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameContext::~GameContext()
{
	m_commonStates = nullptr;
	m_keyboardTracker = nullptr;
	m_spriteBath = nullptr;
}
