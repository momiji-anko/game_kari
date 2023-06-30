#pragma once
#include<SimpleMath.h>
#include<Model.h>
#include"StepTimer.h"

//�O���錾
class Camera;
class ShadowMap;
class AABBFor3D;
class Capsule;

/// <summary>
/// �A�N�^�[�̃C���^�[�t�F�[�X�N���X
/// </summary>
class IActor
{
public:
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& position) = 0;
	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���݂̍��W</returns>
	virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;

	/// <summary>
	/// �ړ��ʂ̐ݒ�
	/// </summary>
	/// <param name="velcity">�ړ���</param>
	virtual void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) = 0;
	/// <summary>
	/// �ړ��ʂ̎擾
	/// </summary>
	/// <returns>�ړ���</returns>
	virtual DirectX::SimpleMath::Vector3 GetVelocity() const = 0;

	/// <summary>
	/// ���f���̐ݒ�
	/// </summary>
	/// <param name="model">���f���̐��|�C���^</param>
	virtual void SetModel(DirectX::Model* model) = 0;
	/// <summary>
	/// ���f���̎擾
	/// </summary>
	/// <returns>���f���̐��|�C���^</returns>
	virtual DirectX::Model* GetModel() const = 0;

	/// <summary>
	/// �A�N�e�B�u�̐ݒ�
	/// </summary>
	/// <param name="active">true = �A�N�e�B�u�Afalse = ��A�N�e�B�u</param>
	virtual void SetActive(bool active) = 0;
	/// <summary>
	/// �A�N�e�B�u��Ԃ̎擾
	/// </summary>
	/// <returns>true = �A�N�e�B�u�Afalse = ��A�N�e�B�u</returns>
	virtual bool GetActive() const = 0;
	/// <summary>
	/// �A�N�e�B�u��Ԃ̊m�F
	/// </summary>
	/// <returns>true = �A�N�e�B�u�Afalse = ��A�N�e�B�u</returns>
	virtual bool IsActive() const = 0;

	/// <summary>
	/// �����蔻��AABB�̎擾
	/// </summary>
	/// <param name="AABBobject">AABB�̃��j�[�N�|�C���^</param>
	virtual void SetAABB(std::unique_ptr<AABBFor3D>& AABBObject) = 0;
	/// <summary>
	/// �����蔻��AABB�̐ݒ�
	/// </summary>
	/// <returns></returns>
	virtual AABBFor3D* GetAABB() const = 0;

	/// <summary>
	/// �����蔻��J�v�Z���̐ݒ�
	/// </summary>
	/// <param name="capsule">�J�v�Z���̃��j�[�N�|�C���^</param>
	virtual void SetCapsule(std::unique_ptr<Capsule>& capsule) = 0;
	/// <summary>
	/// �����蔻��J�v�Z���̎擾
	/// </summary>
	/// <returns>�J�v�Z���̐��|�C���^</returns>
	virtual Capsule* GetCapsule() const = 0;

	/// <summary>
	/// ���[���h�s��̐ݒ�
	/// </summary>
	/// <param name="world">���[���h�s��</param>
	virtual void SetWorldMatrix(const DirectX::SimpleMath::Matrix& world) = 0;
	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	virtual DirectX::SimpleMath::Matrix GetWorldMatrix()const = 0;

	/// <summary>
	/// �g�k�̐ݒ�
	/// </summary>
	/// <param name="scale">�g�k</param>
	virtual void SetScale(const DirectX::SimpleMath::Vector3& scale) = 0;
	/// <summary>
	/// �g�k�̎擾
	/// </summary>
	/// <returns>�g�k</returns>
	virtual DirectX::SimpleMath::Vector3 GetScale() const = 0;

	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	/// <param name="rotation">�p�x(�N�H�[�^�j�I��)</param>
	virtual void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) = 0;
	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	/// <param name="rotation">�p�x(Vector3)</param>
	virtual void SetRotation(const DirectX::SimpleMath::Vector3& rotation) = 0;
	/// <summary>
	/// �p�x�̎擾
	/// </summary>
	/// <returns></returns>
	virtual DirectX::SimpleMath::Quaternion GetRotation() const = 0;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	IActor() = default;
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IActor() = default;

	/// <summary>
	/// ������	
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="velcity">�ړ���</param>
	/// <param name="scale">�g�k</param>
	/// <param name="rotataion">�X�P�[��</param>
	/// <param name="model">���f��</param>
	/// <param name="active">�A�N�e�B�u</param>
	virtual void Initialize(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& velcity,
		const DirectX::SimpleMath::Vector3 & scale,
		const DirectX::SimpleMath::Vector3 & rotataion,
		DirectX::Model* model,
		bool active
	) = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	virtual void Update(const DX::StepTimer& timer) = 0;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	virtual void Render(const Camera* camera) = 0;

	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// ���[���h�s��v�Z
	/// </summary>
	virtual void CalculateWorldMatrix() = 0;

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	virtual void Reset() = 0;

	/// <summary>
	/// �e����
	/// </summary>
	/// <param name="shadow">�V���h�E�}�b�v�̐��|�C���^</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="projection">�v���W�F�N�V�����s��</param>
	virtual void CreateShadow(ShadowMap* shadow, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection) = 0;

};