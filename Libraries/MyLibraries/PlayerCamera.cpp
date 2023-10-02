/*
* ターゲットを指定することで、eyeの座標を自動計算して追いかけるカメラ	
*
*
*/

#include"pch.h"
#include<Windows.h>
#include<Mouse.h>
#include"PlayerCamera.h"
#include"Game/GameContext/GameContext.h"

#include"DeviceResources.h"

const DirectX::SimpleMath::Vector3 PlayerCamera::TARGET_TO_EYE_VEC =
DirectX::SimpleMath::Vector3(0.0f, 5.0f, 10.0f);

const float PlayerCamera::CAMERA_X_ANGLE_AREA = DirectX::XMConvertToRadians(70);


void PlayerCamera::CalculateViewMatrix()
{
	// ビュー行列を算出する
	DirectX::SimpleMath::Matrix rotY = DirectX::SimpleMath::Matrix::CreateRotationY(m_angleY);
	DirectX::SimpleMath::Matrix rotX = DirectX::SimpleMath::Matrix::CreateRotationX(m_angleX);

	DirectX::SimpleMath::Matrix rt = rotY * rotX;

	DirectX::SimpleMath::Vector3    eye(0.0f, 0.0f, 1.0f);
	DirectX::SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);

	eye = DirectX::SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (TARGET_TO_EYE_VEC.Length() - m_scrollWheelValue / 100);
	up = DirectX::SimpleMath::Vector3::Transform(up, rt.Invert());

	std::vector<DirectX::SimpleMath::Vector3> collisionLine = {  GetTargetPosition(),eye + GetTargetPosition() };
	DirectX::SimpleMath::Vector3 normal = DirectX::SimpleMath::Vector3::Zero;
	DirectX::SimpleMath::Vector3 pos = DirectX::SimpleMath::Vector3::Zero;

	if (GameContext::GetInstance().GetCollisionManager()->DetectCollisionCameraLine2Polygon(collisionLine, normal, pos))
	{
		
		eye = pos;
	}
	else
	{
		
		eye = eye + GetTargetPosition();
	}

	if (!GameContext::GetInstance().GetCollisionManager()->DetectCollisionPlayer2FallDeathAABB())
	{
		SetEyePosition(eye);
		SetUpVec(up);
	}

	SetViewMatrix(DirectX::SimpleMath::Matrix::CreateLookAt(GetEyePosition(), GetTargetPosition(), GetUpVector()));

}

//コンストラクタ
PlayerCamera::PlayerCamera()
	:
	m_aspectRatio(0.0f),
	m_nearPlane(0.0f),
	m_farPlane(0.0f),
	m_fov(0.0f),
	m_refEyePos(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)),
	m_refTargetPos(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f)),
	m_angleX(0.0f), 
	m_angleY(0.0f), 
	m_prevX(0), 
	m_prevY(0), 
	m_scrollWheelValue(1)
{
}
//デストラクタ
PlayerCamera::~PlayerCamera()
{
}

// 初期化
void PlayerCamera::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//カメラの位置の初期化
	SetEyePosition(DirectX::SimpleMath::Vector3(0.0f, 2.0f, 5.0f));
	SetTargetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	SetUpVec(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f));

	// ウィンドウサイズの取得
	RECT size = pDR->GetOutputSize();
	m_aspectRatio = float(size.right) / float(size.bottom);

	//カメラの見える範囲の設定
	m_nearPlane = 1.0f;
	m_farPlane = 100000.0f;
	m_fov = 45.0f;

	GameContext::GetInstance().SetCameraAngleY(m_angleY);

}

void PlayerCamera::Update()
{
	// どこかでMouseインスタンスが作られていれば、マウスの状態が取得できる(参考：Impl)
	DirectX::Mouse::State state = DirectX::Mouse::Get().GetState();

	// マウスの左クリック＆ドラッグでカメラ座標を更新する
	if (state.leftButton)
	{
		DraggedDistance(state.x, state.y);
	}

	// マウスの座標を前回の値として保存
	m_prevX = state.x;
	m_prevY = state.y;

	// マウスホイールのスクロール値を取得
	m_scrollWheelValue = state.scrollWheelValue;
	m_scrollWheelValue = std::max(std::min(0, m_scrollWheelValue), -1000);

	if (m_scrollWheelValue > 0)
	{
		m_scrollWheelValue = 0;
		DirectX::Mouse::Get().ResetScrollWheelValue();
	}

	SetEyePosition(GetEyePosition() + (m_refEyePos - GetEyePosition()) * 0.05f);
	SetTargetPosition(GetTargetPosition() + (m_refTargetPos - GetTargetPosition()) * 0.05f);
	
	SetCameraTeaget(GameContext::GetInstance().GetPlayerPosition());
	SetCameraTargetForce(GameContext::GetInstance().GetPlayerPosition());

	GameContext::GetInstance().SetCameraAngleY(m_angleY);
	
	CalculateViewMatrix();

}

// 終了処理
void PlayerCamera::Finalize()
{
}

void PlayerCamera::SetCameraTargetForce(DirectX::SimpleMath::Vector3 target)
{
	SetTargetPosition(target);

	SetEyePosition(GetTargetPosition() + TARGET_TO_EYE_VEC);
}

//ターゲットを設定
void PlayerCamera::SetCameraTeaget(DirectX::SimpleMath::Vector3 target)
{
	m_refTargetPos = target;

	m_refTargetPos = m_refTargetPos + TARGET_TO_EYE_VEC;

}
//UPベクトルの設定
void PlayerCamera::SetUpVec(DirectX::SimpleMath::Vector3 up)
{
	SetUpVector(up);
}

//viewの取得
DirectX::SimpleMath::Matrix PlayerCamera::GetView()
{
	//Viewの再計算
	SetViewMatrix(DirectX::SimpleMath::Matrix::CreateLookAt(GetEyePosition(), GetTargetPosition(), GetUpVector()));

	return GetViewMatrix();
}
//projectionの取得
DirectX::SimpleMath::Matrix PlayerCamera::GetProjection()
{
	//Projectionの再計算
	float fieldOfView = DirectX::XMConvertToRadians(m_fov);

	SetProjectionMatrix(DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane));

	return GetProjectionMatrix();
}

//-------------------------------------------------------------------
// マウスポインタのドラッグ開始位置からの変位(相対値)
//-------------------------------------------------------------------
void PlayerCamera::DraggedDistance(int x, int y)
{
	// マウスポインタの前回からの変位
	// なお、0.2fは適当な補正値 -> ドラッグの移動量を調整する
	float dx = static_cast<float>(x - m_prevX) * 0.2f;
	float dy = static_cast<float>(y - m_prevY) * 0.2f;

	if (dx != 0.0f || dy != 0.0f)
	{
		// マウスポインタの変位を元に、Ｘ軸Ｙ軸の回転角を求める
		// XとYの関係に注意！！
		float angleX = dy * DirectX::XM_PI / 180.0f;
		float angleY = dx * DirectX::XM_PI / 180.0f;

		// 角度の更新
		m_angleX += angleX;
		m_angleY += angleY;


		m_angleX = std::max(std::min(CAMERA_X_ANGLE_AREA,m_angleX),-CAMERA_X_ANGLE_AREA);
	}
}

//その他のデータを設定
void PlayerCamera::SetNearPlane(float nearVal)
{
	m_nearPlane = nearVal;
}
void PlayerCamera::SetFarPlane(float farVal)
{
	m_farPlane = farVal;
}
void PlayerCamera::SetDegreeFov(float fovVal)
{
	m_fov = fovVal;
}

//その他のデータを取得
float PlayerCamera::GetNearPlane()
{
	return m_nearPlane;
}

float PlayerCamera::GetFarPlane()
{
	return m_farPlane;
}

float PlayerCamera::GetDegreeFov()
{
	return m_fov;
}
