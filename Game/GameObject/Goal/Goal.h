#pragma once 
#include"Game/GameObject/Actor/Actor.h"

class Goal : public Actor
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Goal(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity, const DirectX::SimpleMath::Vector3& scale, const DirectX::SimpleMath::Vector3& rotation, DirectX::Model* model, bool active);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Goal();

	/// <summary>
	/// ������	
	/// </summary>
	void Initialize();

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
};