#include"pch.h"
#include"GameContext.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameContext::GameContext()
	:
	m_commonStates{nullptr},
	m_keyboardTracker{nullptr},
	m_spriteBath{nullptr},
	m_angleY{},
	m_collisionManager{},
	m_enemyManager{},
	m_isAllGetKeys{false},
	m_isClear{false},
	m_playerDeath{false},
	m_playerPosition{},
	m_selectStage{0}
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
