#pragma once 
#include"Game/GameObject/Actor/Actor.h"


class Stage : public Actor
{
public:

	void SetVertexPosition(const std::vector<DirectX::SimpleMath::Vector3>& vertexPositon) { m_vertexPosition = vertexPositon; }
	std::vector<DirectX::SimpleMath::Vector3> GetVertexPosition()const { return m_vertexPosition; }

	void SetIndex(const std::vector<int>& index) { m_index = index; }
	std::vector<int> GetIndex()const { return m_index; }

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Stage();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Stage();

	/// <summary>
	/// ������	
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="velocity">�ړ���</param>
	/// <param name="scale">�g�k</param>
	/// <param name="rotation">�X�P�[��</param>
	/// <param name="model">���f��</param>
	/// <param name="active">�A�N�e�B�u</param>
	void Initialize(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Vector3& velocity,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Vector3& rotation,
		DirectX::Model* model,
		bool active
	)override;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void Update(const DX::StepTimer& timer)override;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	void Render(const Camera* camera)override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset()override;

private:

	std::vector<DirectX::SimpleMath::Vector3> m_vertexPosition;

	std::vector<int> m_index;
};
