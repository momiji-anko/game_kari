/*
タイトルシーン
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"
#include"Game/GameContext/GameContext.h"
#include "TitleScene.h"
#include"Libraries/MyLibraries/Camera.h"


using namespace DirectX;

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
TitleScene::TitleScene(GameMain* parent)
	:
	m_parent{ parent },
	m_stageSelect{},
	m_cameraAngle{ 0 }
{
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
TitleScene::~TitleScene()
{
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void TitleScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//キーボードステートトラッカー取得
	m_keyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	GameContext::GetInstance().SetKeyboardStateTracker(m_keyboardStateTracker.get());

	// コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/TridentLogo.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);

	GameContext::GetInstance().SetSpriteBath(m_spriteBatch.get());

	m_stageSelect = std::make_unique<StageSelect>();
	m_stageSelect->Initialize();

	m_camera = std::make_unique<Camera>();
	


}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
void TitleScene::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	// マウス入力情報を取得する
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();


	if (m_previousNumber != m_stageSelect->GetSelectStageNum())
	{
		m_previousNumber = m_stageSelect->GetSelectStageNum();

		m_stageManager.reset(new StageManager(m_previousNumber));
		m_stageManager->Initialize();
	}

	m_stageManager->Update(timer);

	m_cameraAngle += 0.1f;

	if (m_stageSelect->Update(timer))
	{
		m_parent->ChengeScene(m_parent->GetPlayScene());
	}

}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void TitleScene::Draw()
{
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	SimpleMath::Vector2 pos(640 - 128, 360 - 128);

	m_spriteBatch->Draw(m_texture.Get(), pos);

	m_spriteFont->DrawString(m_spriteBatch.get(), L"Title Scene", DirectX::XMFLOAT2(10, 10));

	m_stageSelect->Draw();


	// ビュー行列
	DirectX::SimpleMath::Matrix view;
	// プロジェクション行列
	DirectX::SimpleMath::Matrix projection;
	// カメラ座標
	DirectX::SimpleMath::Vector3 eye = { cos(m_cameraAngle) * 18,10,-sin(m_cameraAngle) * 18 };
	// 注視点
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3::Zero;
	// 上向きベクトル
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;
	// ビュー行列計算
	view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
	m_camera->SetViewMatrix(view);

	m_stageManager->Render(m_camera.get());
	m_spriteBatch->End();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void TitleScene::Finalize()
{
}
