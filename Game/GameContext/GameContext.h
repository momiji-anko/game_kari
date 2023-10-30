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

	/// <summary>
	/// プレイヤーの座標設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& position) { m_playerPosition = position; }
	/// <summary>
	/// プレイヤーの座標取得
	/// </summary>
	/// <returns>座標</returns>
	DirectX::SimpleMath::Vector3 GetPlayerPosition() { return m_playerPosition; }

	/// <summary>
	/// カメラのY軸角度を設定
	/// </summary>
	/// <param name="angle">角度</param>
	void SetCameraAngleY(float angle) { m_angleY = angle; }
	/// <summary>
	/// カメラのY軸角度取得
	/// </summary>
	/// <returns>角度</returns>
	float GetCmeraAngleY() { return m_angleY; }

	/// <summary>
	/// プレイヤーが死亡しているか設定
	/// </summary>
	/// <param name="flag">死亡しているか</param>
	void SetPlayerDeath(bool flag) { m_playerDeath = flag; }
	/// <summary>
	/// プレイヤーが死亡しているか取得
	/// </summary>
	/// <returns>死亡しているか</returns>
	bool IsPlayerDeath() { return m_playerDeath; }

	/// <summary>
	/// 選択したステージ番号
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	void SetSelectStageNum(int stageNum) { m_selectStage = stageNum; }
	/// <summary>
	/// ステージ番号取得
	/// </summary>
	/// <returns>ステージ番号</returns>
	int GetSelectStageNum() { return m_selectStage; }

	/// <summary>
	/// クリアしているか設定
	/// </summary>
	/// <param name="isClear">クリアしているか</param>
	void SetIsClear(bool isClear) { m_isClear = isClear; }
	/// <summary>
	/// クリアしているか取得
	/// </summary>
	/// <returns>クリアしているか</returns>
	bool ISClear() { return m_isClear; }

	/// <summary>
	/// すべての鍵を取得しているか設定
	/// </summary>
	/// <param name="isAllGetKeys">すべて取得しているか</param>
	void SetIsAllGetKey(bool isAllGetKeys) { m_isAllGetKeys = isAllGetKeys; }
	/// <summary>
	/// すべての鍵を取得しているか取得
	/// </summary>
	/// <returns>すべて取得しているか</returns>
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

	//カメラのY軸角度
	float m_angleY;
	//プレイヤーが死亡しているか
	bool m_playerDeath;
	//選択しているステージの番号
	int m_selectStage;
	//クリアしているか
	bool m_isClear;
	//すべての鍵を取得しているか
	bool m_isAllGetKeys;
	//敵のマネージャー
	EnemyManager* m_enemyManager;
};