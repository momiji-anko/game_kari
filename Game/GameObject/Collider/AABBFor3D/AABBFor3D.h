/*
	AABBFor3D.h
	3D��Ԃ�AABB�̓����蔻���u�����Ȃ����߂�
	�@�\���v���O��������N���X�̃w�b�_�̃t�@�C��

*/
#pragma once

#include<CommonStates.h>
#include<PrimitiveBatch.h>
#include<SimpleMath.h>
#include<VertexTypes.h>
#include<Effects.h>


//AABB�̓����蔻��
class AABBFor3D
{
public:
	//AABB�p�̓����蔻��f�[�^��ݒ肷��
	void SetData(const DirectX::SimpleMath::Vector3& minPos,const DirectX::SimpleMath::Vector3& maxPos);

public:
	//�R���X�g���N�^
	AABBFor3D();
	//�f�X�g���N�^
	~AABBFor3D();

	//������
	void Initialize();
	//�`��
	void Draw(const DirectX::SimpleMath::Matrix& world, const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection,const DirectX::XMFLOAT4& color);

	//�����蔻����s��
	bool DetectCollition(AABBFor3D* other);

private:

	void DrawBox(const DirectX::XMFLOAT4& color);

private:
	//���S���W
	DirectX::SimpleMath::Vector3 m_center;
	//�e�ӂ܂ł̋���
	DirectX::SimpleMath::Vector3 m_r;

	//�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	//�v���~�e�B�u�o�b�`
	std::unique_ptr < DirectX::PrimitiveBatch < DirectX::VertexPositionColor>> m_primitiveBach;

	//�x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	//�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

};