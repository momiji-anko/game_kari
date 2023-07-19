//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"
#include"Game/PlayScene/PlayScene.h"
#include"Game/TitleScene/TitleScene.h"
// �V�[���̗�
enum class GAME_SCENE : int
{
    NONE,

    TITLE,
    PLAY,
};

// �O���錾
class DebugCamera;

class IScene;

/// <summary>
/// Game�N���X����A���[�U���������𔲂��o�����N���X
/// </summary>
class GameMain
{
private:

    // �L�[�{�[�h
    std::unique_ptr<DirectX::Keyboard> m_keybord;
    
    // �}�E�X
    std::unique_ptr<DirectX::Mouse> m_mouse;

	// ���̃V�[���ԍ�
	GAME_SCENE m_nextScene;

	// �V�[��
	IScene* m_pScene;

    PlayScene m_playScane;

    TitleScene m_titleScane;

public:
    GameMain();
    ~GameMain();

    void Initialize();
    void Update(const DX::StepTimer& timer);
    void Render();
    void Finalize();

	

    void ChengeScene(IScene* scene);
    
    IScene* GetPlayScene() { return &m_playScane; }

    IScene* GetTitleScene() { return &m_titleScane; }

};
