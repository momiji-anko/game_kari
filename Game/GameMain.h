//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"

class PlayScene;
class ResultScene;
class TitleScene;
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
public:
    GameMain();
    ~GameMain();

    void Initialize();
    void Update(const DX::StepTimer& timer);
    void Render();
    void Finalize();

    void ChengeScene(IScene* scene);

    IScene* GetPlayScene();

    IScene* GetTitleScene();

    IScene* GetResultScene();

private:

    // �L�[�{�[�h
    std::unique_ptr<DirectX::Keyboard> m_keybord;
    
    // �}�E�X
    std::unique_ptr<DirectX::Mouse> m_mouse;

	// ���̃V�[���ԍ�
	GAME_SCENE m_nextScene;

	// �V�[��
	IScene* m_pScene;

    std::unique_ptr<PlayScene> m_playScane;

    std::unique_ptr<TitleScene> m_titleScane;

    std::unique_ptr<ResultScene> m_resultScene;

};
