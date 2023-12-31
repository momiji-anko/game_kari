
#include"pch.h"
#include"StageSelect.h"
#include"Libraries/MyLibraries/TextureManager.h"
#include"Libraries/MyLibraries/FileLoadManager.h"
#include"DeviceResources.h"
#include"Game/GameContext/GameContext.h"
#include"Libraries/MyLibraries/MemoryLeakDetector.h"

/// <summary>
/// コンストラクタ
/// </summary>
StageSelect::StageSelect()
	:
	m_texturePosition{ DirectX::SimpleMath::Vector2::Zero },
	m_selectStageNum{ 0 },
	m_stageMaxNum{ 0 }
{
}

/// <summary>
/// デストラクタ
/// </summary>
StageSelect::~StageSelect()
{
	
}

/// <summary>
/// 初期化
/// </summary>
void StageSelect::Initialize()
{

	//デバイスリソース取得
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();

	//テクスチャマネージャーの取得
	TextureManager& textureManager = TextureManager::GetInstance();

	//テクスチャの読み込み
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageTextTexture = textureManager.LoadTexture(L"stageText.png");
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageNumTexture = textureManager.LoadTexture(L"stageNum.png");
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageArrowTexture = textureManager.LoadTexture(L"Arrow.png");


	//ステージ画像のY座標
	float stageSelectPosY = static_cast<float>(size.bottom / 2.0f + size.bottom / 4.0);
	//ステージ画像のX座標
	float stagePositionX = static_cast<float>(size.right / 2.0f);
	//ステージ画像の座標設定
	m_texturePosition = { stagePositionX,stageSelectPosY };

	//ファイルマネージャーの取得
	FileLoadManager& fileLoadManager = FileLoadManager::GetInstance();
	//ファイルの数取得
	m_stageMaxNum = static_cast<int>(fileLoadManager.LoadFile(L"Resources/Stage/Json/").size());


}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">タイマー</param>
/// <returns>次の選択画面に行くか</returns>
bool StageSelect::Update(const DX::StepTimer& timer)
{
	UNREFERENCED_PARAMETER(timer);

	//ゲームコンテキスト取得
	GameContext& gameContext = GameContext::GetInstance();
	//ゲームコンテキストからキーボードステートトラッカーの取得
	DirectX::Keyboard::KeyboardStateTracker* keyboard = gameContext.GetKeyboardStateTracker();

	//ボタンを押したか
	bool isKeyPush = false;

	// キー入力情報を取得する
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();
	//キーボードステートトラッカーの更新
	keyboard->Update(keyState);


	//ステージ選択
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		//押したらtrueにする
		isKeyPush = true;

		//ステージの番号を減らす
		m_selectStageNum -= 1;

	}
	else if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		//押したらtrueにする
		isKeyPush = true;

		//ステージの番号を増やす
		m_selectStageNum += 1;
	}


	//押したらステージマネージャーのステージを変える
	if (isKeyPush)
	{
		//範囲チェック
		m_selectStageNum = std::min(std::max(0, m_selectStageNum), m_stageMaxNum - 1);
	}

	//スペースキーを押すと次の選択画面に行くに移動
	if (keyboard->IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		gameContext.SetSelectStageNum(m_selectStageNum);
		return true;
	}

	return false;
}

/// <summary>
/// 描画
/// </summary>
void StageSelect::Draw()
{
	//ステージ選択のUI表示
	StageSelectUIDraw();
}

/// <summary>
/// 終了処理
/// </summary>
void StageSelect::Finalize()
{

}

/// <summary>
/// ステージ選択のUI表示
/// </summary>
void StageSelect::StageSelectUIDraw()
{
	//テクスチャマネージャーの取得
	TextureManager& textureManager = TextureManager::GetInstance();

	//テクスチャの読み込み
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageTextTexture = textureManager.LoadTexture(L"stageText.png");
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageNumTexture = textureManager.LoadTexture(L"stageNum.png");
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> stageArrowTexture = textureManager.LoadTexture(L"Arrow.png");

	//ステージ番号テクスチャの画像サイズを取得
	DirectX::SimpleMath::Vector2 stageNumTextureSize = textureManager.GetTextureSize(L"stageNum.png");
	//ステージテキストテクスチャの画像サイズを取得
	DirectX::SimpleMath::Vector2 stageTextTextureSize = textureManager.GetTextureSize(L"stageText.png");
	//アローテクスチャの画像サイズを取得
	DirectX::SimpleMath::Vector2 stageArrowTextureSize = textureManager.GetTextureSize(L"Arrow.png");

	//ゲームコンテキスト取得
	GameContext& gameContext = GameContext::GetInstance();
	//ゲームコンテキストからスプライトバッチ取得
	DirectX::SpriteBatch* spriteBatch = gameContext.GetSpriteBatch();

	//数字の一マスのサイズ
	float stageNumSize = (stageNumTextureSize.x) / 10;

	//切り取り位置
	RECT texRect =
	{
		static_cast<LONG>(stageNumSize * m_selectStageNum + 1),
		static_cast <LONG>(0),
		static_cast<LONG>(stageNumSize * (m_selectStageNum + 1) - (stageNumSize / 5)),
		static_cast<LONG>(stageNumTextureSize.y)
	};

	//拡大率
	float textureScale = 1.5f;

	//カラー
	DirectX::SimpleMath::Vector4 color = DirectX::Colors::Red;

	//ステージ文字の描画
	spriteBatch->Draw(stageTextTexture.Get(), m_texturePosition, nullptr, color, 0.0f, stageTextTextureSize / 2.0f, textureScale);

	//数字の位置
	DirectX::SimpleMath::Vector2 numPosition = m_texturePosition + DirectX::SimpleMath::Vector2(stageTextTextureSize.x + stageNumTextureSize.x / 10, 0);

	//数字の描画
	spriteBatch->Draw(stageNumTexture.Get(), numPosition, &texRect, color, 0.0f, DirectX::SimpleMath::Vector2(stageNumSize, stageNumTextureSize.y / 2.0f), textureScale);

	//選択しているステージ番号が１より高ければ左矢印表示
	if (m_selectStageNum > 0)
	{
		spriteBatch->Draw(stageArrowTexture.Get(), m_texturePosition - DirectX::SimpleMath::Vector2(stageArrowTextureSize.x + stageTextTextureSize.x, 0), nullptr, DirectX::Colors::White, DirectX::XM_PI, stageArrowTextureSize / 2, textureScale);
	}

	//選択しているステージ番号がステージの最大数より低くくれば
	if (m_selectStageNum < m_stageMaxNum - 1)
	{
		spriteBatch->Draw(stageArrowTexture.Get(), numPosition + DirectX::SimpleMath::Vector2(stageArrowTextureSize.x, 0), nullptr, DirectX::Colors::White, 0.0f, stageArrowTextureSize / 2, textureScale);
	}
}

