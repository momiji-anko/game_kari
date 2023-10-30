/*
タイトルシーン
*/

#include"pch.h"

#include<WICTextureLoader.h>
#include<SimpleMath.h>

#include"../GameMain.h"
#include"DeviceResources.h"

#include"ResultScene.h"
#include"Game/GameMain.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Game/GameContext/GameContext.h"


//アルファの最大値
const float ResultScene::ALPHA_MAX_NUM = 1.0f;
//アルファの最小値
const float ResultScene::ALPHA_MIN_NUM = 0.0f;

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
ResultScene::ResultScene(GameMain* parent)
	:
	m_parent{parent},
	m_commonState{},
	m_fade{},
	m_spriteBatch{},
	m_spriteFont{},
	m_alpha{1.0f},
	m_alphaSpeed{-0.01f}
{
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
ResultScene::~ResultScene()
{
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void ResultScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	// コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	//フェードの作成
	m_fade = std::make_unique<Fade>();
	//shader作成
	m_fade->Create();
	//初期化
	m_fade->Initialize(DirectX::SimpleMath::Vector3::Zero);
	//最初にフェードインする
	m_fade->FadeIn();

}

/*--------------------------------------------------
更新
--------------------------------------------------*/
void ResultScene::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	// マウス入力情報を取得する
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	//フェード更新
	m_fade->Update(timer);

	//アルファをアルファスピードで足す
	m_alpha += m_alphaSpeed;

	//アルファが０か１になったらアルファスピードを反転させる
	if (m_alpha < ALPHA_MIN_NUM || m_alpha > ALPHA_MAX_NUM)
	{
		//アルファスピードを反転させる
		m_alphaSpeed *= -1.0f;
	}

	//フェードアウトするか
	if (m_fade->ISOpen() && keyState.Space)
	{
		m_fade->FadeOut();		
	}

	//フェードアウトしている状態であればタイトルシーンに移行
	if (m_fade->ISClose())
		m_parent->ChengeScene(m_parent->GetTitleScene());

}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void ResultScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	
	//画像描画開始
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());
	//テクスチャマネージャー取得
	TextureManager& textureManager = TextureManager::GetInstance();
	//画像のファイル名
	std::wstring tex[] =
	{
		L"youdead.png",
		L"clear.png",
	};
	//クリアしているか取得
	int isclear = GameContext::GetInstance().ISClear();
	//画像を読み込み
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> clearTexture = textureManager.LoadTexture(tex[isclear]);
	//画像サイズの取得
	DirectX::SimpleMath::Vector2 texSize = textureManager.GetTextureSize(tex[isclear]);


	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();
	DirectX::SimpleMath::Vector2 windowCenterPosition(size.right / 2.0f, size.bottom / 2.0f);
	
	//画像描画
	m_spriteBatch->Draw(clearTexture.Get(), windowCenterPosition, nullptr, DirectX::Colors::White, 0.0f, texSize/2.0f,2.0f);
	
	//PUSH_SPASE_KEYの表示座標
	DirectX::SimpleMath::Vector2 pushSpeseKeyPos = DirectX::SimpleMath::Vector2::Zero;
	//PUSH_SPASE_KEYの色
	DirectX::SimpleMath::Vector4 pushColor{ 1.0f,1.0f,1.0f,m_alpha };
	//PUSH_SPASE_KEYの表示
	m_spriteBatch->Draw(textureManager.LoadTexture(L"PushSpaceKey.png").Get(), pushSpeseKeyPos, nullptr, pushColor, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	//画像描画終了
	m_spriteBatch->End();

	//フェード描画
	m_fade->Render();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void ResultScene::Finalize()
{
}
