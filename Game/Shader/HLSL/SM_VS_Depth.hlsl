#include "SM_Header.hlsli"

// �V���h�E�}�b�v�쐬�p�̒��_�V�F�[�_�[
float4 main(VSInput vin) : SV_POSITION
{
	// ���W�ϊ��������Ȃ�
	return mul(vin.Position, WorldViewProj);
}
