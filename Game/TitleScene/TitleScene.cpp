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



/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
TitleScene::TitleScene(GameMain* parent)
	:
	m_parent{parent},
	m_stageSelect{},
	m_cameraAngle{0},
	m_previousStageSelectNumber{0}
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
	GameContext::GetInstance().SetCommonState(m_commonState.get());

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	
	//GameContextにスプライトバッチを登録
	GameContext::GetInstance().SetSpriteBatch(m_spriteBatch.get());

	//ステージセレクト作成
	m_stageSelect = std::make_unique<StageSelect>();
	m_stageSelect->Initialize();

	//カメラ作成
	m_camera = std::make_unique<Camera>();
	
	//ステージマネージャー作成
	m_stageManager = std::make_unique<StageManager>(m_previousStageSelectNumber);
	m_stageManager->Initialize();

	//フェードの作成
	m_fade = std::make_unique<Fade>();
	//shader作成
	m_fade->Create();
	//初期化
	m_fade->Initialize(DirectX::SimpleMath::Vector3::Zero);
	//最初にフェードインする
	m_fade->FadeIn();

	//タイトル文字作成
	m_titleText = std::make_unique<TitleText>();
	m_titleText->Initialize();

}

/*--------------------------------------------------
更新
--------------------------------------------------*/
void TitleScene::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	// マウス入力情報を取得する
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	m_fade->Update(timer);

	//フェードアウトしている状態であればプレイシーン移行
	if (m_fade->ISClose())
		m_parent->ChengeScene(m_parent->GetPlayScene());

	//フェードインしていない場合これ以降処理しない
	if (!m_fade->ISOpen())
		return;


	//タイトル文字の動き
	m_titleText->Update(timer);

	//選択ステージが変えた場合、背景のステージを変える
	if (m_previousStageSelectNumber != m_stageSelect->GetSelectStageNum())
	{
		//選択していたステージの番号を更新
		m_previousStageSelectNumber = m_stageSelect->GetSelectStageNum();

		//ステージマネージャーの再生成
		m_stageManager.reset(new StageManager(m_previousStageSelectNumber));
		m_stageManager->Initialize();
	}

	//ステージマネージャー更新
	m_stageManager->Update(timer);

	//カメラを回転させる
	m_cameraAngle += 0.001f;

	//ステージが選ばれたらフェードアウトする
	if (m_stageSelect->Update(timer))
	{
		m_fade->FadeOut();		
	}
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void TitleScene::Draw()
{
	//画像描画開始
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//ステージ選択描画
	m_stageSelect->Draw();

	// ビュー行列
	DirectX::SimpleMath::Matrix view;
	// プロジェクション行列
	// カメラ座標
	DirectX::SimpleMath::Vector3 eye = { cos(m_cameraAngle) * 180,100,-sin(m_cameraAngle) * 180 };
	// 注視点
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3::Zero;
	// 上向きベクトル
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;
	// ビュー行列計算
	view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
	// ビュー設定
	m_camera->SetViewMatrix(view);
	
	//ステージマネージャー描画
	m_stageManager->Render(m_camera.get());
	
	//タイトル文字描画
	m_titleText->Draw();


	//画像描画終了
	m_spriteBatch->End();

	//フェード描画
	m_fade->Render();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void TitleScene::Finalize()
{
}
