#pragma once 
#include"Camera.h"

/*
* �^�[�Q�b�g���w�肷�邱�ƂŁAeye�̍��W�������v�Z���Ēǂ�������J����
*
*
*/
#pragma once 
#include<SimpleMath.h>
#include<CommonStates.h>
#include"Camera.h"

class PlayerCamera : public Camera
{

private:	//�萔
	static const DirectX::SimpleMath::Vector3 TARGET_TO_EYE_VEC;
public:


private:	//�����o�֐�
	// �r���[�s��̎Z�o
	void CalculateViewMatrix() override;
public:

	//�R���X�g���N�^
	PlayerCamera();
	//�f�X�g���N�^
	~PlayerCamera();

	// ������
	void Initialize();

	//�X�V
	void Update();

	// �I������
	void Finalize();

	void DraggedDistance(int x, int y);

	//�^�[�Q�b�g���W�������I�ɌŒ肷��
	void  SetCameraTargetForce(DirectX::SimpleMath::Vector3 target);
	//�o�l���o���݃^�[�Q�b�g��ݒ�
	void SetCameraTeaget(DirectX::SimpleMath::Vector3 target);
	//UP�x�N�g���̐ݒ�
	void SetUpVec(DirectX::SimpleMath::Vector3 up);

	//view�̎擾
	DirectX::SimpleMath::Matrix GetView();
	//projection�̎擾
	DirectX::SimpleMath::Matrix GetProjection();

	//���̑��̃f�[�^��ݒ�
	void SetNearPlane(float nearVal);
	void SetFarPlane(float farVal);
	void SetDegreeFov(float fovVal);

	//���̑��̃f�[�^���擾
	float GetNearPlane();
	float GetFarPlane();
	float GetDegreeFov();

private:
	static const float CAMERA_X_ANGLE_AREA;

private:	//�����o�ϐ�


	// �c��]�p
	float m_angleX;								
	// ����]�p
	float m_angleY;								
	// �O��̃}�E�X���W(X,Y)
	int m_prevX, m_prevY;		
	// �}�E�X�z�C�[���̃X�N���[���l
	int m_scrollWheelValue;		

	//�J�������W
	DirectX::SimpleMath::Vector3 m_refEyePos;
	//�J���������_
	DirectX::SimpleMath::Vector3 m_refTargetPos;

	//��ʂ̃A�X�y�N�g��
	float m_aspectRatio;
	//�J���������ԋ߂��ʒu
	float m_nearPlane;
	//�J���������ԉ����ʒu
	float m_farPlane;
	//�J�����̉�p
	float m_fov;

public:

};