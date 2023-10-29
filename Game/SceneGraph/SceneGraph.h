#pragma once
#include"Game/GameObject/Actor/Actor.h"
#include<vector>

class SceneGraph 
{

public:
	/// <summary>
	/// �R���e�L�X�g
	/// </summary>
	SceneGraph();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SceneGraph();

	/// <summary>
	/// ������	
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void Update(const DX::StepTimer& timer);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="camera">�J�����̐��|�C���^</param>
	void Render(const Camera* camera);

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

	/// <summary>
	/// �m�[�h��ǉ�����
	/// </summary>
	/// <param name="sceneNode">�m�[�h</param>
	void AttachNode(std::unique_ptr<Actor> sceneNode);
private:
	//�V�[���O���t
	std::vector<std::unique_ptr<Actor>> m_sceneGraph;
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
};