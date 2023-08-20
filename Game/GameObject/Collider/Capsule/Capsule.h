#pragma once

#include <SimpleMath.h>
#include <cassert>
#include <algorithm>
#include <functional>

//	�J�v�Z���̍\����
struct Capsule
{
	DirectX::SimpleMath::Vector3 a;		//	�c�̐����̊J�n�_
	DirectX::SimpleMath::Vector3 b;		//	�c�̐����̏I���_
	float r;							//	���a
};




class  CapsuleCollition
{
public:
	CapsuleCollition() = default;
	~ CapsuleCollition() = default;

	/// <summary>
	/// �N�����v�֐�
	/// </summary>
	/// <param name="n">�ۂߏ������s���������l</param>
	/// <param name="min">�ۂߏ������s���ŏ��l</param>
	/// <param name="max">�ۂߏ������s���ő�l</param>
	/// <returns>�ۂߏ������s�������ʂƂȂ鐔�l</returns>
	static float Clamp(float n, float minNum, float maxNum)
	{
		
		return std::min(std::max(minNum, n), maxNum);
	}

	/// <summary>
	/// �Q�̐����̍ŒZ�����̕�����Ԃ��֐�
	/// </summary>
	/// <param name="p1">���� 1 �̎n�_</param>
	/// <param name="q1">���� 1 �̏I�_</param>
	/// <param name="p2">���� 2 �̎n�_</param>
	/// <param name="q2">���� 2 �̏I�_</param>
	/// <param name="s">�����P��̍ŒZ�ʒu��\���W��</param>
	/// <param name="t">���� 2 ��̍ŒZ�ʒu��\���W��</param>
	/// <param name="c1">�����P��̍ŒZ�����̈ʒu </param>
	/// <param name="c2">���� 2 ��̍ŒZ�����̈ʒu</param>
	/// <returns>�Q�̐����̍ŒZ�����̕���</returns>
	static float ClosestPtSegmentSegment(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& q1, const DirectX::SimpleMath::Vector3& p2, const DirectX::SimpleMath::Vector3& q2, float& s, float& t, DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2)
	{
		// p1��q1 �̃x�N�g�����Z�o
		DirectX::SimpleMath::Vector3 d1 = q1 - p1;
		// p2��q2 �̃x�N�g�����Z�o
		DirectX::SimpleMath::Vector3 d2 = q2 - p2;
		// ��̃J�v�Z�����������́A�n�_�Ԃ̃x�N�g���𔼌a�̍��v�Ƃ��ĎZ�o
		DirectX::SimpleMath::Vector3 r = p1 - p2;
		// p1��q1 �̃x�N�g���̒����� 2 ����Z�o
		float a = d1.Dot(d1);
		// p2��q2 �̃x�N�g���̒����� 2 ����Z�o
		float e = d2.Dot(d2);
		// p1��q1 �̃x�N�g���Ɣ��a�̍��v�ƂȂ�x�N�g������ς���
		float f = d2.Dot(r);
		if (a <= FLT_EPSILON && e <= FLT_EPSILON)
		{
			// a �� e �̗������ق� 0 ��������A��̃J�v�Z���͕��s�ł���
			s = t = 0.0f;
			// ���Ƃ��Ċe�n�_���l������
			c1 = p1;
			c2 = p2;
			// ���擾�����_���m�̋��������߂�
			return (c1 - c2).Dot(c1 - c2);
		}
		if (a <= FLT_EPSILON)
		{
			// a ���ق� 0 �������ꍇ�́Af/e �� t �Ƃ��Čv�Z����
			// �������A0�`1 �̊Ԃɕ␳����
			s = 0.0f;
			t = f / e;
			t = Clamp(t, 0.0f, 1.0f);
		}
		else
		{
			// p1��q1 �̃x�N�g���Ɣ��a�̍��v����ς���
			float c = d1.Dot(r);
			if (e <= FLT_EPSILON)
			{
				// e ���ق� 0 �������ꍇ�́As �Ƃ���-c/a ���v�Z����B
				// �������A0�`1 �̊Ԃɕ␳����
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else
			{
				// ���� 0 ���Z���N����댯�͂Ȃ��̂ŁA���ʂ�̌v�Z���s��
				// �e�J�v�Z���̃x�N�g���œ��ς���
				float b = d1.Dot(d2);
				// ���ꂼ��̃J�v�Z���̒����� 2 ����|�����l�ƁA
				// �e�J�v�Z���̃x�N�g���œ��ς������ʂ� 2 ����ׂ�
				float denom = a * e - b * b;
				if (denom != 0.0f)
				{
					// ����������Ă���΁A���ʂ�̌v�Z���s��
					s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
				}
				else
				{
					// ��̃J�v�Z���̌������ق� 90 �x�̏ꍇ�As �� 0 �Ƃ���
					s = 0.0f;
				}
				// b * s + f ���v�Z���Č��؂���
				float tnom = (b * s + f);
				if (tnom < 0.0f)
				{
					// �}�C�i�X�̒l�ɂȂ��Ă�����A-c / a �̒l�� s �Ƃ���
					t = 0.0f;
					s = Clamp(-c / a, 0.0f, 1.0f);
				}
				else if (tnom > e)
				{
					// e ���傫����΁A(b - c) / a �̒l�� s �Ƃ���
					t = 1.0f;
					s = Clamp((b - c) / a, 0.0f, 1.0f);
				}
				else
				{
					// ���ʂ�̌v�Z���s��
					t = tnom / e;
				}
			}
		}
		// ���ꂼ��̃J�v�Z���ɂ�����A��ԋ߂��_���Z�o����
		c1 = p1 + d1 * s;
		c2 = p2 + d2 * t;

		// ��ԋ߂���_�̋����̓���Ԃ�
		return (c1 - c2).Dot(c1 - c2);
	}


	/// <summary>
	/// �J�v�Z���ƃJ�v�Z���̏Փ˔���֐�
	/// </summary>
	/// <param name="capsule1">�����蔻�����肽���J�v�Z�� 1 ��</param>
	/// <param name="capsule2">�����蔻�����肽���J�v�Z�� 2 ��</param>
	/// <param name="c1">�����P��̍ŒZ�����̈ʒu</param>
	/// <param name="c2">�����Q��̍ŒZ�����̈ʒu</param>
	/// <returns>true= ���������Afalse=�������ĂȂ�</returns>
	static bool DetectCollition_Capsule2Capsule(Capsule capsule1, Capsule capsule2, DirectX::SimpleMath::Vector3& c1, DirectX::SimpleMath::Vector3& c2)
	{
		float s, t;
		// �J�v�Z���̒��S�̐����Ԃ̋����̕������v�Z
		// s, t, c1, c2 �͖߂�l
		float dist2 = ClosestPtSegmentSegment(capsule1.a, capsule1.b, capsule2.a, capsule2.b, s, t, c1, c2);
		float radius = capsule1.r + capsule2.r;

		return dist2 <= radius * radius;
	}

};

