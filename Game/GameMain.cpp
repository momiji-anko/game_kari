//
// Scene.cpp
//
#include "pch.h"

#include "DeviceResources.h"
#include "GameMain.h"

#include "Libraries/MyLibraries/DebugCamera.h"
#include "Libraries/MyLibraries/GridFloor.h"

#include"Game/PlayScene/PlayScene.h"
#include"Game/TitleScene/TitleScene.h"
#include"Game/ResultScene/ResultScene.h"

// 名前空間の利用
using namespace DirectX;

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
GameMain::GameMain()
	:
	m_nextScene{ GAME_SCENE::TITLE },		// 初期シーンの設定
	m_pScene{ nullptr },
	m_playScane{},
	m_titleScane{},
	m_resultScene{}
{
}

//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
GameMain::~GameMain()
{
	Finalize();
}

//-------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------
void GameMain::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// キーボード関連
	m_keybord = std::make_unique<DirectX::Keyboard>();

	// マウス関連
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(pDR->GetHwnd());

	m_playScane = std::make_unique<PlayScene>(this);
	m_titleScane = std::make_unique<TitleScene>(this);
	m_resultScene = std::make_unique<ResultScene>(this);

	
	ChengeScene(m_playScane.get());
}

//-------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------
void GameMain::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = m_keybord->GetState();

	// ESCキーで終了
	if (keyState.Escape)
	{
		PostQuitMessage(0);
	}


	// 実態があれば更新
	if (m_pScene != nullptr)
	{
		m_pScene->Update(timer);
	}
}

//-------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------
void GameMain::Render()
{
	// 実態があれば描画
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}
}

//-------------------------------------------------------------------
// 後始末
//-------------------------------------------------------------------
void GameMain::Finalize()
{


}

void GameMain::ChengeScene(IScene* scene)
{
	if (m_pScene != nullptr)
		m_pScene->Finalize();

	m_pScene = scene;

	m_pScene->Initialize();
}

IScene* GameMain::GetPlayScene()
{
	return m_playScane.get();
}

IScene* GameMain::GetTitleScene()
{
	return m_titleScane.get();
}

IScene* GameMain::GetResultScene()
{
	return m_resultScene.get();
}
