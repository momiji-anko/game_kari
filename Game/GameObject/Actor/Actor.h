#pragma once
#include"IActor.h"
#include"Game/GameObject/Collider/AABBFor3D/AABBFor3D.h"
#include"Game/GameObject/Collider/Capsule/Capsule.h"
#include<SimpleMath.h>

class Actor :public IActor
{
public:
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const DirectX::SimpleMath::Vector3& position)override final { m_position = position; }
	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���݂̍��W</returns>
	DirectX::SimpleMath::Vector3 GetPosition() const override final { return m_position; }

	/// <summary>
	/// �ړ��ʂ̐ݒ�
	/// </summary>
	/// <param name="velcity">�ړ���</param>
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) override final { m_velocity = velocity; }
	/// <summary>
	/// �ړ��ʂ̎擾
	/// </summary>
	/// <returns>�ړ���</returns>
	DirectX::SimpleMath::Vector3 GetVelocity() const override final { return m_velocity; }

	/// <summary>
	/// ���f���̐ݒ�
	/// </summary>
	/// <param name="model">���f���̐��|�C���^</param>
	void SetModel(DirectX::Model* model) override final { m_model = model; }
	/// <summary>
	/// ���f���̎擾
	/// </summary>
	/// <returns>���f���̐��|�C���^</returns>
	DirectX::Model* GetModel() const override final { return m_model; }

	/// <summary>
	/// �A�N�e�B�u�̐ݒ�
	/// </summary>
	/// <param name="active">true = �A�N�e�B�u�Afalse = ��A�N�e�B�u</param>
	void SetActive(bool active)override final { m_active = active; }
	/// <summary>
	/// �A�N�e�B�u��Ԃ̎擾
	/// </summary>
	/// <returns>true = �A�N�e�B�u�Afalse = ��A�N�e�B�u</returns>
	bool GetActive() const override final { return m_active; }
	/// <summary>
	/// �A�N�e�B�u��Ԃ̊m�F
	/// </summary>
	/// <returns>true = �A�N�e�B�u�Afalse = ��A�N�e�B�u</returns>
	bool IsActive() const override final { return m_active; }

	/// <summary>
	/// �����蔻��AABB�̎擾
	/// </summary>
	/// <param name="AABBobject">AABB�̃��j�[�N�|�C���^</param>
	void SetAABB(std::unique_ptr<AABBFor3D>& AABBObject) override final { m_AABBObject = std::move(AABBObject); }
	/// <summary>
	/// �����蔻��AABB�̐ݒ�
	/// </summary>
	/// <returns></returns>
	AABBFor3D* GetAABB() const override final { return m_AABBObject.get(); }

	/// <summary>
	/// �����蔻��J�v�Z���̐ݒ�
	/// </summary>
	/// <param name="capsule">�J�v�Z���̃��j�[�N�|�C���^</param>
	void SetCapsule(std::unique_ptr<Capsule>& capsule) override final { m_capsule = std::move(capsule); }
	/// <summary>
	/// �����蔻��J�v�Z���̎擾
	/// </summary>
	/// <returns>�J�v�Z���̐��|�C���^</returns>
	Capsule* GetCapsule() const override final { return m_capsule.get(); }

	/// <summary>
	/// �g�k�̐ݒ�
	/// </summary>
	/// <param name="scale">�g�k</param>
	void SetScale(const DirectX::SimpleMath::Vector3& scale) override final { m_scale = scale; }
	/// <summary>
	/// �g�k�̎擾
	/// </summary>
	/// <returns>�g�k</returns>
	DirectX::SimpleMath::Vector3 GetScale() const override final { return m_scale; }

	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	/// <param name="rotation">�p�x(�N�H�[�^�j�I��)</param>
	void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) override final { m_rotation = rotation; }
	/// <summary>
	/// �p�x�̐ݒ�
	/// </summary>
	/// <param name="rotation">�p�x(Vector3)</param>
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) override final { m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z); }
	/// <summary>
	/// �p�x�̎擾
	/// </summary>
	/// <returns></returns>
	DirectX::SimpleMath::Quaternion GetRotation() const override final { return m_rotation; }

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Actor();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Actor() = default;

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
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotataion,
		DirectX::Model* model,
		bool active
	);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	virtual void Update(const DX::StepTimer& timer);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	virtual void Render(const Camera * camera);

	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// ���[���h�s��v�Z
	/// </summary>
	void CalculateWorldMatrix();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	virtual void Reset();

	/// <summary>
	/// �e����
	/// </summary>
	/// <param name="shadow">�V���h�E�}�b�v�̐��|�C���^</param>
	/// <param name="view">�r���[�s��</param>
	/// <param name="projection">�v���W�F�N�V�����s��</param>
	virtual void CreateShadow(ShadowMap * shadow, const DirectX::SimpleMath::Matrix & view, const DirectX::SimpleMath::Matrix & projection);

private:
	//���W
	DirectX::SimpleMath::Vector3    m_position;
	//�ړ���
	DirectX::SimpleMath::Vector3    m_velocity;

	//���f���̃|�C���^�[
	DirectX::Model* m_model;

	//�A�N�e�B�u
	bool m_active;

	//AABB�����蔻��
	std::unique_ptr<AABBFor3D> m_AABBObject;
	//�J�v�Z�������蔻��
	std::unique_ptr<Capsule>   m_capsule;

	//���[���h�s��
	DirectX::SimpleMath::Matrix     m_world;
	//�g�k
	DirectX::SimpleMath::Vector3    m_scale;
	//�p�x
	DirectX::SimpleMath::Quaternion m_rotation;

};