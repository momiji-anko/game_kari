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
// シーンの列挙
enum class GAME_SCENE : int
{
    NONE,

    TITLE,
    PLAY,
};

// 前方宣言
class DebugCamera;

class IScene;

/// <summary>
/// Gameクラスから、ユーザ処理部分を抜き出したクラス
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

    // キーボード
    std::unique_ptr<DirectX::Keyboard> m_keybord;
    
    // マウス
    std::unique_ptr<DirectX::Mouse> m_mouse;

	// 次のシーン番号
	GAME_SCENE m_nextScene;

	// シーン
	IScene* m_pScene;

    std::unique_ptr<PlayScene> m_playScane;

    std::unique_ptr<TitleScene> m_titleScane;

    std::unique_ptr<ResultScene> m_resultScene;

};
