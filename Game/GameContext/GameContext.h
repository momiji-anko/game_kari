#pragma once 
#include"Libraries/MyLibraries/Singleton.h"
#include<SpriteBatch.h>
#include<Keyboard.h>
#include<CommonStates.h>
#include"Game/GameObject/Collider/CollisionManager/CollisionManager.h"
#include"Game/GameObject/Enemy/EnemyManager.h"

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
	void SetSpriteBatch(DirectX::SpriteBatch* spriteBath) { m_spriteBath = spriteBath; }
	/// <summary>
	/// スプライトバッチの取得
	/// </summary>
	/// <returns>スプライトバッチの生ポインタ</returns>
	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBath; }

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
	void SetKeyboardStateTracker(DirectX::Keyboard::KeyboardStateTracker* keyboard) { m_keyboardTracker = keyboard; }
	/// <summary>
	/// キーボードステートトラッカーの取得
	/// </summary>
	/// <returns></returns>
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardStateTracker() { return m_keyboardTracker; }

	/// <summary>
	/// コリジョンマネージャーの設定
	/// </summary>
	/// <param name="manager">コリジョンマネージャーの生ポインタ</param>
	void SetCollisionManager(CollisionManager* manager) { m_collisionManager = manager; }
	/// <summary>
	/// コリジョンマネージャーの取得
	/// </summary>
	/// <returns>コリジョンマネージャーの生ポインタ</returns>
	CollisionManager* GetCollisionManager() { return m_collisionManager; }
	/// <summary>
	/// 敵マネージャー設定
	/// </summary>
	/// <param name="manager">敵マネージャー生ポインタ</param>
	void SetEnemyManager(EnemyManager* manager) { m_enemyManager = manager; }
	/// <summary>
	/// 敵マネージャー取得
	/// </summary>
	/// <returns>敵マネージャー生ポインタ</returns>
	EnemyManager* GetEnemyManager() { return m_enemyManager; }

	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& position) { m_playerPosition = position; }
	DirectX::SimpleMath::Vector3 GetPlayerPosition() { return m_playerPosition; }

	void SetCameraAngleY(float angle) { m_angleY = angle; }
	float GetCmeraAngleY() { return m_angleY; }

	void SetPlayerDeath(bool flag) { m_playerDeath = flag; }
	bool IsPlayerDeath() { return m_playerDeath; }

	void SetSelectStageNum(int stageNum) { m_selectStage = stageNum; }
	int GetSelectStageNum() { return m_selectStage; }

	void SetIsClear(bool isClear) { m_isClear = isClear; }
	bool ISClear() { return m_isClear; }

	void SetIsAllGetKey(bool isAllGetKeys) { m_isAllGetKeys = isAllGetKeys; }
	bool GetIsAllGetKey() { return m_isAllGetKeys; }

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameContext();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameContext();

private:
	//スプライトバッチ
	DirectX::SpriteBatch* m_spriteBath;
	//コモンステート
	DirectX::CommonStates* m_commonStates;
	//キーボードステートトラッカー
	DirectX::Keyboard::KeyboardStateTracker* m_keyboardTracker;
	//コリジョンマネージャー
	CollisionManager* m_collisionManager;

	DirectX::SimpleMath::Vector3 m_playerPosition;

	float m_angleY;

	bool m_playerDeath;

	int m_selectStage;

	bool m_isClear;

	bool m_isAllGetKeys;

	EnemyManager* m_enemyManager;
};