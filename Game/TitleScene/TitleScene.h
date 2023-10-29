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
#include"Game/Shader/Fade.h"
#include"TitleText.h"

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


	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	//キーボードステートトラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;
	//ステージセレクト
	std::unique_ptr<StageSelect> m_stageSelect;
	//フェード
	std::unique_ptr<Fade> m_fade;


	//タイトル文字
	std::unique_ptr<TitleText> m_titleText;

	//親
	GameMain* m_parent;
	//カメラ
	std::unique_ptr<Camera> m_camera;
	//ステートマネージャー
	std::unique_ptr<Actor> m_stageManager;
	//カメラ角度
	float m_cameraAngle;
	//前のステージ選択番号
	int m_previousStageSelectNumber;


};