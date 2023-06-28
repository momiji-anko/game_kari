/*
プレイシーン
*/
#include "pch.h"

#include "DeviceResources.h"
#include "../GameMain.h"
#include<windows.h>
#include "PlayScene.h"

// 名前空間の利用
using namespace DirectX;

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
PlayScene::PlayScene()
{
}


/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
PlayScene::~PlayScene()
{
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void PlayScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void PlayScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void PlayScene::Finalize()
{
}
