/*
* �^�[�Q�b�g���w�肷�邱�ƂŁAeye�̍��W�������v�Z���Ēǂ�������J����	
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
	// �r���[�s����Z�o����
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

//�R���X�g���N�^
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
//�f�X�g���N�^
PlayerCamera::~PlayerCamera()
{
}

// ������
void PlayerCamera::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	//�J�����̈ʒu�̏�����
	SetEyePosition(DirectX::SimpleMath::Vector3(0.0f, 2.0f, 5.0f));
	SetTargetPosition(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	SetUpVec(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f));

	// �E�B���h�E�T�C�Y�̎擾
	RECT size = pDR->GetOutputSize();
	m_aspectRatio = float(size.right) / float(size.bottom);

	//�J�����̌�����͈͂̐ݒ�
	m_nearPlane = 1.0f;
	m_farPlane = 100000.0f;
	m_fov = 45.0f;

	GameContext::GetInstance().SetCameraAngleY(m_angleY);

}

void PlayerCamera::Update()
{
	// �ǂ�����Mouse�C���X�^���X������Ă���΁A�}�E�X�̏�Ԃ��擾�ł���(�Q�l�FImpl)
	DirectX::Mouse::State state = DirectX::Mouse::Get().GetState();

	// �}�E�X�̍��N���b�N���h���b�O�ŃJ�������W���X�V����
	if (state.leftButton)
	{
		DraggedDistance(state.x, state.y);
	}

	// �}�E�X�̍��W��O��̒l�Ƃ��ĕۑ�
	m_prevX = state.x;
	m_prevY = state.y;

	// �}�E�X�z�C�[���̃X�N���[���l���擾
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

// �I������
void PlayerCamera::Finalize()
{
}

void PlayerCamera::SetCameraTargetForce(DirectX::SimpleMath::Vector3 target)
{
	SetTargetPosition(target);

	SetEyePosition(GetTargetPosition() + TARGET_TO_EYE_VEC);
}

//�^�[�Q�b�g��ݒ�
void PlayerCamera::SetCameraTeaget(DirectX::SimpleMath::Vector3 target)
{
	m_refTargetPos = target;

	m_refTargetPos = m_refTargetPos + TARGET_TO_EYE_VEC;

}
//UP�x�N�g���̐ݒ�
void PlayerCamera::SetUpVec(DirectX::SimpleMath::Vector3 up)
{
	SetUpVector(up);
}

//view�̎擾
DirectX::SimpleMath::Matrix PlayerCamera::GetView()
{
	//View�̍Čv�Z
	SetViewMatrix(DirectX::SimpleMath::Matrix::CreateLookAt(GetEyePosition(), GetTargetPosition(), GetUpVector()));

	return GetViewMatrix();
}
//projection�̎擾
DirectX::SimpleMath::Matrix PlayerCamera::GetProjection()
{
	//Projection�̍Čv�Z
	float fieldOfView = DirectX::XMConvertToRadians(m_fov);

	SetProjectionMatrix(DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane));

	return GetProjectionMatrix();
}

//-------------------------------------------------------------------
// �}�E�X�|�C���^�̃h���b�O�J�n�ʒu����̕ψ�(���Βl)
//-------------------------------------------------------------------
void PlayerCamera::DraggedDistance(int x, int y)
{
	// �}�E�X�|�C���^�̑O�񂩂�̕ψ�
	// �Ȃ��A0.2f�͓K���ȕ␳�l -> �h���b�O�̈ړ��ʂ𒲐�����
	float dx = static_cast<float>(x - m_prevX) * 0.2f;
	float dy = static_cast<float>(y - m_prevY) * 0.2f;

	if (dx != 0.0f || dy != 0.0f)
	{
		// �}�E�X�|�C���^�̕ψʂ����ɁA�w���x���̉�]�p�����߂�
		// X��Y�̊֌W�ɒ��ӁI�I
		float angleX = dy * DirectX::XM_PI / 180.0f;
		float angleY = dx * DirectX::XM_PI / 180.0f;

		// �p�x�̍X�V
		m_angleX += angleX;
		m_angleY += angleY;


		m_angleX = std::max(std::min(CAMERA_X_ANGLE_AREA,m_angleX),-CAMERA_X_ANGLE_AREA);
	}
}

//���̑��̃f�[�^��ݒ�
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

//���̑��̃f�[�^���擾
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
