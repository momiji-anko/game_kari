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

using namespace DirectX;

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
ResultScene::ResultScene(GameMain* parent)
	:
	m_parent(parent)
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

	// テクスチャの読み込み
	CreateWICTextureFromFile(
		device,
		L"Resources/Textures/TridentLogo.png",
		nullptr,
		m_texture.ReleaseAndGetAddressOf()
	);

}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
void ResultScene::Update(const DX::StepTimer& timer)
{
	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	// マウス入力情報を取得する
	DirectX::Mouse::State mouseState = DirectX::Mouse::Get().GetState();

	if (keyState.Z || mouseState.rightButton)
	{
		m_parent->ChengeScene(m_parent->GetTitleScene());
	}

}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void ResultScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	TextureManager& textureManager = TextureManager::GetInstance();

	std::wstring tex[] =
	{
		L"youdead.png",
		L"clear.png",
	};
	
	int isclear = GameContext::GetInstance().ISClear();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> clearTexture = textureManager.LoadTexture(tex[isclear]);
	DirectX::SimpleMath::Vector2 texSize = textureManager.GetTextureSize(tex[isclear]);


	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();
	DirectX::SimpleMath::Vector2 windowCenterPosition(size.right / 2.0f, size.bottom / 2.0f);

	SimpleMath::Vector2 pos(640 - 128, 360 - 128);

	m_spriteBatch->Draw(clearTexture.Get(), windowCenterPosition, nullptr, DirectX::Colors::White, 0.0f, texSize/2.0f);

	m_spriteFont->DrawString(m_spriteBatch.get(), L"Result Scene", DirectX::XMFLOAT2(10, 10));

	m_spriteBatch->End();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void ResultScene::Finalize()
{
}
