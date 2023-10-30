/*
リザルトシーン
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>


#include"Game/IScene.h"
#include"Game/Shader/Fade.h"

class GameMain;

class ResultScene : public IScene
{

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
private:
	//アルファの最大値
	static const float ALPHA_MAX_NUM;
	//アルファの最小値
	static const float ALPHA_MIN_NUM;
private:

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//フェード
	std::unique_ptr<Fade> m_fade;
	//親
	GameMain* m_parent;

	//PUSH_SPASE_KEYのアルファ値
	float m_alpha;
	//PUSH_SPASE_KEYのアルファ変化速度
	float m_alphaSpeed;
};