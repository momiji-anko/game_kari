#pragma once
#include"StepTimer.h"
#include<vector>
#include<SimpleMath.h>

/// <summary>
/// �^�C�g���̕���
/// </summary>
class TitleText
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	TitleText();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~TitleText();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="timer">�^�C�}�[</param>
	void  Update(const DX::StepTimer& timer);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// ���`���
	/// </summary>
	/// <param name="start">�J�n�̐��l</param>
	/// <param name="end">�I���̐��l</param>
	/// <param name="t">���݂̈ʒu</param>
	/// <returns>���݂̐��l</returns>
	float Lerp(float start, float end, float t)
	{
		return start + (end - start) * t;
	}

	/// <summary>
	/// �C�[�W���O
	/// </summary>
	float easeOutCubic(float x)
	{
		return static_cast <float>(1 - pow(1 - x, 3));
	}


private:

	//�^�C�g���̈ړ�����
	static const float MOVE_TIME;
	//�^�C�g���̈ړ��p�x
	static const float MOVE_RORATION;
	//�^�C�g���̏����p�x
	static const float RORATION_ZERO;
	//�^�C�g���̉�]���[�`���̃��Z�b�g
	static const int ROTATION_RESET_ROUTINE;
	//�J�����̉�]���x
	static const float CAMERA_ROT_SPEED;
	//�A���t�@�̍ő�l
	static const float MOVED_TEXTURE_ALPHA;
	//�A���t�@�̍ŏ��l
	static const float MOVEING_TEXTURE_ALPHA;


	//�^�C�g���̃e�N�X�`���̃t�@�C���p�X
	std::wstring m_titleTextureFilePath;
	//�^�C�g���̃e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titileTexture;
	//�^�C�g���̓����̃^�C��
	float m_titleTimer_s;
	//�^�C�g���̃��[�`��
	int m_titleRoutine;
	//�^�C�g���̃|�W�V����
	std::vector<DirectX::SimpleMath::Vector2> m_titlePosition;
	//�^�C�g���̊p�x
	float m_titleRotetion;
	//�^�C�g���̃A���t�@�l
	float m_titleAlpha;

};