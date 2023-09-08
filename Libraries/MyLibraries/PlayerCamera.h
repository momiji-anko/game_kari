#pragma once 
#include"Camera.h"

/*
* ターゲットを指定することで、eyeの座標を自動計算して追いかけるカメラ
*
*
*/
#pragma once 
#include<SimpleMath.h>
#include<CommonStates.h>
#include"Camera.h"

class PlayerCamera : public Camera
{

private:	//定数
	static const DirectX::SimpleMath::Vector3 TARGET_TO_EYE_VEC;
public:


private:	//メンバ関数
	// ビュー行列の算出
	void CalculateViewMatrix() override;
public:

	//コンストラクタ
	PlayerCamera();
	//デストラクタ
	~PlayerCamera();

	// 初期化
	void Initialize();

	//更新
	void Update();

	// 終了処理
	void Finalize();

	void DraggedDistance(int x, int y);

	//ターゲット座標を強制的に固定する
	void  SetCameraTargetForce(DirectX::SimpleMath::Vector3 target);
	//バネ演出込みターゲットを設定
	void SetCameraTeaget(DirectX::SimpleMath::Vector3 target);
	//UPベクトルの設定
	void SetUpVec(DirectX::SimpleMath::Vector3 up);

	//viewの取得
	DirectX::SimpleMath::Matrix GetView();
	//projectionの取得
	DirectX::SimpleMath::Matrix GetProjection();

	//その他のデータを設定
	void SetNearPlane(float nearVal);
	void SetFarPlane(float farVal);
	void SetDegreeFov(float fovVal);

	//その他のデータを取得
	float GetNearPlane();
	float GetFarPlane();
	float GetDegreeFov();

private:
	static const float CAMERA_X_ANGLE_AREA;

private:	//メンバ変数


	// 縦回転角
	float m_angleX;								
	// 横回転角
	float m_angleY;								
	// 前回のマウス座標(X,Y)
	int m_prevX, m_prevY;		
	// マウスホイールのスクロール値
	int m_scrollWheelValue;		

	//カメラ座標
	DirectX::SimpleMath::Vector3 m_refEyePos;
	//カメラ注視点
	DirectX::SimpleMath::Vector3 m_refTargetPos;

	//画面のアスペクト比
	float m_aspectRatio;
	//カメラから一番近い位置
	float m_nearPlane;
	//カメラから一番遠い位置
	float m_farPlane;
	//カメラの画角
	float m_fov;

public:

};