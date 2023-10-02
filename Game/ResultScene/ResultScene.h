/*
タイトルシーン
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>


#include"Game/IScene.h"

class GameMain;

class ResultScene : public IScene
{
private:

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	GameMain* m_parent;
public:


	// コンストラクタ
	ResultScene(GameMain* parent);

	// デストラクタ
	~ResultScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;
};