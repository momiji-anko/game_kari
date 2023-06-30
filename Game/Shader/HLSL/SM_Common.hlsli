
void AddSpecular(inout float4 color, float3 specular)
{
    color.rgb += specular * color.a;
}

float CalcShadow(float4 ZCalcTex)
{
    // ���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
    float ZValue = ZCalcTex.z / ZCalcTex.w;

    // �ˉe��Ԃ�XY���W���e�N�X�`�����W�ɕϊ�
    float2 TransTexCoord;
    TransTexCoord.x = (1.0f + ZCalcTex.x / ZCalcTex.w) * 0.5f;
    TransTexCoord.y = (1.0f - ZCalcTex.y / ZCalcTex.w) * 0.5f;

    // ���A��Z�l���o
    float SM_Z = SM_Texture.Sample(SM_Sampler, TransTexCoord).x;

    // �Z�o�_���V���h�E�}�b�v��Z�l�����傫����Ήe�Ɣ��f
    return (ZValue > SM_Z + 0.0001f) ? 0.5 : 1.0;
}
