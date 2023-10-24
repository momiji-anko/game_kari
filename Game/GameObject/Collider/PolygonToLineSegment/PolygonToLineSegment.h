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
	/// <param name="normalVector">�@���x�N�g���̎Q��</param>
	/// <returns>true=�������Ă���@false=�����Ă��Ȃ�</returns>
	static bool DetectCollision(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const std::vector<DirectX::SimpleMath::Vector3>& linePos, DirectX::SimpleMath::Vector3& normalVector, DirectX::SimpleMath::Vector3& actorPos)
	{
		//�e���_���W
		DirectX::SimpleMath::Vector3 vertex0 = vertices[0];
		DirectX::SimpleMath::Vector3 vertex1 = vertices[1];
		DirectX::SimpleMath::Vector3 vertex2 = vertices[2];

		//�����̗��[���W
		DirectX::SimpleMath::Vector3 startLine = linePos[0];
		DirectX::SimpleMath::Vector3 endLine = linePos[1];

		DirectX::SimpleMath::Vector3 lineVec = endLine - startLine;

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

			float md = lineVec.Dot(normal);

			//�Z�o�������ς��|���Z���O�ȏ�ł���΃|���S���̒��ɐ���������
			if ((checkDot1 * checkDot2 >= 0 && checkDot2 * checkDot3 >= 0 && checkDot3 * checkDot1 >= 0)&& md > 1.0)
			{
				normalVector = normal;
				//actorPos = ActorPolygonPenetration(endLine, vertices, normal);
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// �J���������Ɣ|���S���̓����蔻��
	/// </summary>
	/// <param name="vertices">���_���W</param>
	/// <param name="linePos">�����̗��[���W</param>
	/// <param name="normalVector">�@���x�N�g���̎Q��</param>
	/// <returns>true=�������Ă���@false=�����Ă��Ȃ�</returns>
	static bool DetectCollision_Camera(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const std::vector<DirectX::SimpleMath::Vector3>& linePos, DirectX::SimpleMath::Vector3& normalVector, DirectX::SimpleMath::Vector3& actorPos)
	{
		//�e���_���W
		DirectX::SimpleMath::Vector3 vertex0 = vertices[0];
		DirectX::SimpleMath::Vector3 vertex1 = vertices[1];
		DirectX::SimpleMath::Vector3 vertex2 = vertices[2];

		//�����̗��[���W
		DirectX::SimpleMath::Vector3 startLine = linePos[0];
		DirectX::SimpleMath::Vector3 endLine = linePos[1];

		DirectX::SimpleMath::Vector3 lineVec = endLine - startLine;

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
			if ((checkDot1 * checkDot2 >= 0 && checkDot2 * checkDot3 >= 0 && checkDot3 * checkDot1 >= 0)&& lineVec.Dot(normal) > 0.5)
			{
				normalVector = normal;
				actorPos = v3Pos;
				return true;
			}
		}
		return false;
	}



	/// <summary>
	/// �A�N�^�[�̃X���C�h�x�N�g�����v�Z����
	/// </summary>
	/// <param name="normalVec">�|���S���̖@���x�N�g��</param>
	/// <param name="actorVel">�A�N�^�[�̈ړ���</param>
	/// <returns>�X���C�h�x�N�g��</returns>
	static DirectX::SimpleMath::Vector3 SlideVecCalculation(const DirectX::SimpleMath::Vector3& normalVec, const DirectX::SimpleMath::Vector3& actorVel)
	{
		DirectX::SimpleMath::Vector3 normalVector = normalVec;
		//�@���x�N�g���̐��K��
		normalVector.Normalize();
 
		if (std::abs(normalVector.y) >= 0.7)
			return DirectX::SimpleMath::Vector3::Zero;

		//�v���C���[�̈ړ��x�N�g���Ɩ@���x�N�g���̓��ς̌v�Z
		float dot = actorVel.Dot(normalVector);
		//�X���C�h����ʌv�Z
		DirectX::SimpleMath::Vector3 slideAmount = { actorVel.x - dot,actorVel.y - dot,actorVel.z - dot };
		//�X���C�h�x�N�g�����v�Z
		DirectX::SimpleMath::Vector3 slideVec = normalVector * slideAmount;
		//�X���C�h�x�N�g����Ԃ�
		return slideVec;
	}

	/// <summary>
	/// �X�e�[�W�̃|���S���ƃA�N�^�[�̂߂荞�ݏ���
	/// </summary>
	/// <param name="actor">�A�N�^�[/param>
	/// <param name="polygonVertexPos">�|���S���̒��_���W</param>
	/// <param name="normalVec">�@���x�N�g��</param>
	static DirectX::SimpleMath::Vector3 ActorPolygonPenetration(const DirectX::SimpleMath::Vector3& actor, const std::vector<DirectX::SimpleMath::Vector3>& polygonVertexPos, const DirectX::SimpleMath::Vector3& normalVec)
	{
		//�A�N�^�[�������߂荞�܂���p�ϐ�
		float actorPenetration = 0.1f;

		//�߂荞�ݏ���
		//�A�N�^�[�̍��W�擾
		DirectX::SimpleMath::Vector3 actorPos = actor;
		//�A�N�^�[�̗���Y���W�v�Z
		actorPos.y = polygonVertexPos[0].y + (-normalVec.x * (actorPos.x - polygonVertexPos[0].x) - normalVec.z * (actorPos.z - polygonVertexPos[0].z)) / normalVec.y;
		//�A�N�^�[���K�N�K�N����̂ŏ����������ɂ߂荞�܂���
		actorPos.y -= actorPenetration;
		
		return actorPos;
	}

};