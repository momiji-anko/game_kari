#pragma once 
#include"Libraries/MyLibraries/Singleton.h"
#include<SpriteBatch.h>
#include<Keyboard.h>
#include<CommonStates.h>

/// <summary>
/// ゲームコンテキスト
/// </summary>
class GameContext :public Singleton<GameContext>
{
public:
	/// <summary>
	/// スプライトバッチの設定
	/// </summary>
	/// <param name="spriteBath">スプライトバッチの生ポインタ</param>
	void SetSpriteBath(DirectX::SpriteBatch* spriteBath) { m_spriteBath = spriteBath; }
	/// <summary>
	/// スプライトバッチの取得
	/// </summary>
	/// <returns>スプライトバッチの生ポインタ</returns>
	DirectX::SpriteBatch* GetSpriteBath() { return m_spriteBath; }

	/// <summary>
	/// コモンステートの設定
	/// </summary>
	/// <param name="commonState">コモンステートの生ポインタ</param>
	void SetCommonState(DirectX::CommonStates* commonState) { m_commonStates = commonState; }
	/// <summary>
	/// コモンステートの取得
	/// </summary>
	/// <returns>コモンステートの生ポインタ</returns>
	DirectX::CommonStates* GetCommonState() { return m_commonStates; }

	/// <summary>
	/// キーボードステートトラッカーの設定
	/// </summary>
	/// <param name="keyboard">キーボードステートトラッカーの生ポインタ</param>
	void SetKeyBoardStateTracker(DirectX::Keyboard::KeyboardStateTracker* keyboard) { m_keyboardTracker = keyboard; }
	/// <summary>
	/// キーボードステートトラッカーの取得
	/// </summary>
	/// <returns></returns>
	DirectX::Keyboard::KeyboardStateTracker* GetKeyBoardStateTracker() { return m_keyboardTracker; }
private:
	//スプライトバッチ
	DirectX::SpriteBatch* m_spriteBath;
	//コモンステート
	DirectX::CommonStates* m_commonStates;
	//キーボードステートトラッカー
	DirectX::Keyboard::KeyboardStateTracker* m_keyboardTracker;

};