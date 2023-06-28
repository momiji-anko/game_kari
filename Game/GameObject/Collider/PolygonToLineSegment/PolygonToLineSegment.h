#pragma once
#include<SimpleMath.h>
#include<vector>

class PolygonToLineSegmentCollision
{
public:
	/// <summary>
	/// �����Ɣ|���S���̓����蔻��
	/// </summary>
	/// <param name="vertices">���_���W</param>
	/// <param name="linePos">�����̗��[���W</param>
	/// <param name="normalVector">�@���x�N�g���̃|�C���^</param>
	/// <returns>true=�������Ă���@false=�����Ă��Ȃ�</returns>
	static bool DetectCollision(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const std::vector<DirectX::SimpleMath::Vector3>& linePos, DirectX::SimpleMath::Vector3* normalVector)
	{
		//�e���_���W
		DirectX::SimpleMath::Vector3 vertex0 = vertices[0];
		DirectX::SimpleMath::Vector3 vertex1 = vertices[1];
		DirectX::SimpleMath::Vector3 vertex2 = vertices[2];

		//�����̗��[���W
		DirectX::SimpleMath::Vector3 startLine = linePos[0];
		DirectX::SimpleMath::Vector3 endLine = linePos[1];

		//�@���x�N�g���Z�o�̂��߂̃f�[�^
		//���_�O���璸�_1�̃x�N�g��
		DirectX::SimpleMath::Vector3 v1 = vertex1 - vertex0;
		//���_1���璸�_2�̃x�N�g��
		DirectX::SimpleMath::Vector3 v2 = vertex2 - vertex1;

		//�@���x�N�g��
		DirectX::SimpleMath::Vector3 normal;
		//�@���x�N�g���̎Z�o
		normal = v1.Cross(v2);
		DirectX::SimpleMath::Vector3 polygonPosAverage = (vertex0 + vertex1 + vertex2) / 3;

		//�|���S���̒��S�_��������̗��[�ւ̃x�N�g�����v�Z
		DirectX::SimpleMath::Vector3 polygonPosAverageToStartLine = startLine - polygonPosAverage;
		DirectX::SimpleMath::Vector3 polygonPosAverageToEndLine = endLine - polygonPosAverage;

		///�����𕽖ʂ̏Փ˔���========================================================================

		//�|���S���̒��S�_��������̗��[�ւ̃x�N�g���Ɩ@���x�N�g���œ��ς��Z�o����
		float dot1 = polygonPosAverageToStartLine.Dot(normal);
		float dot2 = polygonPosAverageToEndLine.Dot(normal);
		//�v�Z�������ς��|���Z���A�O�ȉ��ł���Γ������Ă���
		if (dot1 * dot2 <= 0)
		{

			//�|���S����������̗��[�̋����̌v�Z
			float d1 = std::abs(polygonPosAverageToStartLine.Dot(normal));
			float d2 = std::abs(polygonPosAverageToEndLine.Dot(normal));
			//������̌v�Z
			float a = d1 / (d1 + d2);

			//���_�O��������̗��[�̃x�N�g��
			DirectX::SimpleMath::Vector3 vertex0TostartLine = startLine - vertex0;
			DirectX::SimpleMath::Vector3 vertex0ToendLine = endLine - vertex0;
			//���_�O��������̓����_�v�Z
			DirectX::SimpleMath::Vector3 v3 = ((1 - a) * vertex0TostartLine) + (a * vertex0ToendLine);
			//�����_�̍��W�v�Z
			DirectX::SimpleMath::Vector3 v3Pos = vertex0 + v3;

			//�|���S����ɓ_�����邩���肷��

			//V3Pos����e���_�̍��W�ւ̃x�N�g���쐬
			DirectX::SimpleMath::Vector3 vertex0ToV3Pos = v3;
			DirectX::SimpleMath::Vector3 vertex1ToV3Pos = v3Pos - vertex1;
			DirectX::SimpleMath::Vector3 vertex2ToV3Pos = v3Pos - vertex2;

			//�����ɂȂ�悤�Ƀx�N�g�����쐬����
			//���_�O���璸�_�P�ւ̃x�N�g��
			DirectX::SimpleMath::Vector3 vertex0ToVertex1 = vertex1 - vertex0;
			//���_�P���璸�_�Q�ւ̃x�N�g��
			DirectX::SimpleMath::Vector3 vertex1ToVertex2 = vertex2 - vertex1;
			//���_�Q���璸�_�O�ւ̃x�N�g��
			DirectX::SimpleMath::Vector3 vertex2ToVertex0 = vertex0 - vertex2;

			//V3Pos����O���_�̍��W�ւ̃x�N�g���ƒ��_�O���璸�_�P�ւ̃x�N�g���̊O�ώZ�o
			DirectX::SimpleMath::Vector3 cross1 = vertex0ToV3Pos.Cross(vertex0ToVertex1);
			//V3Pos����P���_�̍��W�ւ̃x�N�g���ƒ��_�P���璸�_�Q�ւ̃x�N�g���̊O�ώZ�o
			DirectX::SimpleMath::Vector3 cross2 = vertex1ToV3Pos.Cross(vertex1ToVertex2);
			//V3Pos����Q���_�̍��W�ւ̃x�N�g���ƒ��_�Q���璸�_�O�ւ̃x�N�g���̊O�ώZ�o
			DirectX::SimpleMath::Vector3 cross3 = vertex2ToV3Pos.Cross(vertex2ToVertex0);

			//�Z�o�����O�ϓ��m����ςŌv�Z
			float checkDot1 = cross1.Dot(cross2);
			float checkDot2 = cross2.Dot(cross3);
			float checkDot3 = cross3.Dot(cross1);

			//�Z�o�������ς��|���Z���O�ȏ�ł���΃|���S���̒��ɐ���������
			if (checkDot1 * checkDot2 >= 0 && checkDot2 * checkDot3 >= 0 && checkDot3 * checkDot1 >= 0)
			{
				*normalVector = normal;
				return true;
			}
		}
		return false;
	}

};