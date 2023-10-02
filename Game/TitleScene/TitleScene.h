/*
タイトルシーン
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include"Game/IScene.h"
#include "Game/GameMain.h"
#include"Game/Shader/Fade.h"
#include"StageSelect.h"

class TitleScene : public IScene
{
public:

	// コンストラクタ
	TitleScene(GameMain* parent);

	// デストラクタ
	~TitleScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

private:
	//アルファの最大値
	static const float ALPHA_MAX_NUM;
	//アルファの最小値
	static const float ALPHA_MIN_NUM;

private:

	//タイトルの状態
	enum class TitleState
	{
		//フェードイン
		FADEIN,
		//タイトル
		TITLE,
		//ステージセレクト
		STAGESELECT,
		//モードセレクト
		MODESELECT,
		//フェードアウト
		FADEOUT,
	};

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//キーボードステートトラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;
	std::unique_ptr<StageSelect> m_stageSelect;

	// テクスチャ
	//PUSH_SPASE_KEYのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushTexture;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//PUSH_SPASE_KEYのアルファ値
	float m_alpha;
	//PUSH_SPASE_KEYのアルファ変化速度
	float m_alphaSpeed;

	//フェードインアウト
	std::unique_ptr<Fade> m_fadeInOut;

	//タイトルの状態
	TitleState m_titleSelect;

	GameMain* m_parent;
};